#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tad_paciente.h"
#include "tad_fila.h"
#include "tad_procedimento.h"
#include "tad_escalonador.h"

void calcular_data_alta(int ano, int mes, int dia, int hora, int minutos_extra, char* data_formatada, size_t tamanho) {
    struct tm t = {0};
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = minutos_extra;
    time_t tempo = mktime(&t);
    strftime(data_formatada, tamanho, "%a %b %d %H:%M:%S %Y", localtime(&tempo));
}

int carregar_dados(const char* filename, Fila* fila_pacientes, Procedimento** procedimentos, int* num_pacientes) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de entrada.\n");
        return -1;
    }

    double tempos[6];
    int unidades[6];
    for (int i = 0; i < 6; i++) {
        if (fscanf(file, "%lf %d", &tempos[i], &unidades[i]) != 2) {
            fprintf(stderr, "Erro ao ler os dados dos procedimentos.\n");
            fclose(file);
            return -1;
        }
    }

    const char* nomes[] = {"Triagem", "Atendimento", "Medidas Hospitalares", "Testes de Laboratório", "Exames de Imagem", "Instrumentos/Medicamentos"};
    for (int i = 0; i < 6; i++) {
        procedimentos[i] = inicializa_procedimento(nomes[i], tempos[i], unidades[i]);
    }

    if (fscanf(file, "%d", num_pacientes) != 1 || *num_pacientes <= 0) {
        fprintf(stderr, "Erro ao ler o número de pacientes.\n");
        fclose(file);
        return -1;
    }

    for (int i = 0; i < *num_pacientes; i++) {
        char id[11];
        int alta, ano, mes, dia, hora, grau, medidas, testes, exames, instrumentos;
        if (fscanf(file, "%s %d %d %d %d %d %d %d %d %d %d",
                   id, &alta, &ano, &mes, &dia, &hora, &grau, &medidas, &testes, &exames, &instrumentos) != 11) {
            fprintf(stderr, "Erro ao ler os dados do paciente %d.\n", i + 1);
            fclose(file);
            return -1;
        }

        Paciente* paciente = inicializa_paciente(id, alta, ano, mes, dia, hora, grau, medidas, testes, exames, instrumentos);
        enfileira(fila_pacientes, paciente);
    }

    fclose(file);
    return 0;
}

void processar_eventos(Escalonador* escalonador, Procedimento** procedimentos, Fila* fila_resultados) {
    double tempo_atual = 0.0;

    while (!escalonador_vazio(escalonador)) {
        Evento evento = remove_proximo_evento(escalonador);
        tempo_atual = evento.tempo;

        if (evento.tipo == 0) {
            Paciente* paciente = (Paciente*)evento.referencia;
            printf("Paciente %s chegou ao hospital no tempo %.2f.\n", paciente->identificador, tempo_atual);

            enfileira(procedimentos[0]->fila, paciente);
            paciente->estado_atual = 0; // Começa na triagem
        }

        for (int i = 0; i < 6; i++) {
            while (!fila_vazia(procedimentos[i]->fila) && unidade_disponivel(procedimentos[i])) {
                Paciente* paciente = desenfileira(procedimentos[i]->fila);

                if (paciente->estado_atual != i) {
                    continue;
                }

                paciente->tempo_espera += (tempo_atual - paciente->hora_atual);
                paciente->hora_atual = tempo_atual;

                double tempo_servico = procedimentos[i]->tempo_medio;
                double tempo_fim = tempo_atual + tempo_servico;

                paciente->tempo_atendimento += tempo_servico;
                registra_ocupacao(procedimentos[i], 0, tempo_atual, tempo_servico);

                if (i == 5) { // Último procedimento
                    printf("Paciente %s finalizou todos os procedimentos no tempo %.2f.\n", paciente->identificador, tempo_fim);
                    enfileira(fila_resultados, paciente);
                } else { // Avança para o próximo procedimento
                    paciente->estado_atual++;
                    printf("Paciente %s avançando para o procedimento %d no tempo %.2f.\n", paciente->identificador, paciente->estado_atual, tempo_fim);
                    enfileira(procedimentos[i + 1]->fila, paciente);
                }
            }
        }
    }
}

void gerar_saida(Fila* fila_resultados) {
    if (fila_resultados->inicio == NULL) {
        printf("Nenhum paciente finalizou o atendimento.\n");
        return;
    }

    Nodo* atual = fila_resultados->inicio;
    while (atual != NULL) {
        Paciente* paciente = atual->paciente;

        double tempo_permanencia = paciente->tempo_atendimento + paciente->tempo_espera;
        double tempo_atendimento = paciente->tempo_atendimento;
        double tempo_espera = paciente->tempo_espera;

        char data_admissao[100];
        char data_alta[100];

        calcular_data_alta(paciente->ano, paciente->mes, paciente->dia, paciente->hora, 0, data_admissao, sizeof(data_admissao));
        calcular_data_alta(paciente->ano, paciente->mes, paciente->dia, paciente->hora + (int)tempo_permanencia,
                           (int)((tempo_permanencia - (int)tempo_permanencia) * 60), data_alta, sizeof(data_alta));

        printf("%s %s %s %.2f %.2f %.2f\n", paciente->identificador, data_admissao, data_alta, tempo_permanencia, tempo_atendimento, tempo_espera);

        atual = atual->proximo;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo de entrada>\n", argv[0]);
        return -1;
    }

    Fila* fila_pacientes = inicializa_fila();
    Procedimento* procedimentos[6];
    int num_pacientes;

    if (carregar_dados(argv[1], fila_pacientes, procedimentos, &num_pacientes) != 0) {
        return -1;
    }

    for (int i = 0; i < 6; i++) {
        procedimentos[i]->fila = inicializa_fila();
    }

    Escalonador* escalonador = inicializa_escalonador(num_pacientes);

    while (!fila_vazia(fila_pacientes)) {
        Paciente* paciente = desenfileira(fila_pacientes);
        insere_evento(escalonador, paciente->hora, 0, paciente);
    }

    Fila* fila_resultados = inicializa_fila();
    processar_eventos(escalonador, procedimentos, fila_resultados);

    gerar_saida(fila_resultados);

    for (int i = 0; i < 6; i++) {
        finaliza_procedimento(procedimentos[i]);
    }
    finaliza_fila(fila_pacientes);
    finaliza_fila(fila_resultados);
    finaliza_escalonador(escalonador);

    return 0;
}
