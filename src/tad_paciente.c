#include "tad_paciente.h"
#include "tad_fila.h"

// Função para inicializar um paciente
Paciente* inicializa_paciente(char* id, int alta, int ano, int mes, int dia, int hora, int grau,
                              int medidas, int testes, int exames, int instrumentos) {
    Paciente* paciente = (Paciente*)malloc(sizeof(Paciente));
    if (paciente == NULL) {
        fprintf(stderr, "Erro ao alocar memória para paciente\n");
        exit(EXIT_FAILURE);
    }
    strncpy(paciente->identificador, id, 10);
    paciente->identificador[10] = '\0';
    paciente->alta = alta;
    paciente->ano = ano;
    paciente->mes = mes;
    paciente->dia = dia;
    paciente->hora = hora;
    paciente->grau_urgencia = grau;
    paciente->medidas_hospitalares = medidas;
    paciente->testes_laboratorio = testes;
    paciente->exames_imagem = exames;
    paciente->instrumentos_medicamentos = instrumentos;
    paciente->estado_atual = 0; // Estado inicial
    paciente->tempo_espera = 0.0;
    paciente->tempo_atendimento = 0.0;
    return paciente;
}

// Atualiza o estado atual de um paciente
void atualiza_estado_paciente(Paciente* paciente, int novo_estado) {
    if (paciente == NULL) return;
    paciente->estado_atual = novo_estado;
}

// Registra tempo de espera para um paciente
void registra_tempo_espera(Paciente* paciente, double tempo) {
    if (paciente == NULL) return;
    paciente->tempo_espera += tempo;
}

// Registra tempo de atendimento para um paciente
void registra_tempo_atendimento(Paciente* paciente, double tempo) {
    if (paciente == NULL) return;
    paciente->tempo_atendimento += tempo;
}

// Libera memória de um paciente
void libera_paciente(Paciente* paciente) {
    if (paciente != NULL) {
        free(paciente);
    }
}

void paciente_entra_fila(Paciente* paciente, Fila* fila, double horario_atual) {
    // Seta o horário de entrada na fila
    paciente->hora_entrada_fila = horario_atual;

    // Insere o paciente na fila
    enfileira(fila, paciente);
}

Paciente* paciente_sai_fila(Fila* fila, double horario_atual) {
    // Remove o paciente da fila
    Paciente* paciente = desenfileira(fila);

    // Calcula o tempo de espera
    if (paciente->hora_entrada_fila > 0) {
        double tempo_espera_na_fila = horario_atual - paciente->hora_entrada_fila;
        paciente->tempo_espera += tempo_espera_na_fila;

        // Reseta o horário de entrada na fila
        paciente->hora_entrada_fila = 0.0;
    }

    return paciente;
}


// Exibe informações de um paciente
void exibe_paciente(const Paciente* paciente) {
    if (paciente == NULL) return;
    printf("ID: %s\n", paciente->identificador);
    printf("Alta: %d\n", paciente->alta);
    printf("Data de admissão: %04d-%02d-%02d %02d:00\n", paciente->ano, paciente->mes, paciente->dia, paciente->hora);
    printf("Grau de urgência: %d\n", paciente->grau_urgencia);
    printf("Medidas hospitalares: %d\n", paciente->medidas_hospitalares);
    printf("Testes de laboratório: %d\n", paciente->testes_laboratorio);
    printf("Exames de imagem: %d\n", paciente->exames_imagem);
    printf("Instrumentos/Medicamentos: %d\n", paciente->instrumentos_medicamentos);
    printf("Estado atual: %d\n", paciente->estado_atual);
    printf("Tempo de espera: %.2f\n", paciente->tempo_espera);
    printf("Tempo de atendimento: %.2f\n", paciente->tempo_atendimento);
}
