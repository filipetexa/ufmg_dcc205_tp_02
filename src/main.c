#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tad_paciente_fila.h"
#include "tad_escalonador.h"
#include "tad_procedimento.h"

// Variáveis globais para parâmetros de entrada
int num_pacientes; // Número de pacientes
double tempos[6];  // Tempos médios por procedimento
int unidades[6];   // Número de unidades por procedimento

// Estruturas para filas globais
Fila *fila_triagem;             // Fila de espera para triagem
Fila *fila_atendimento[3];      // Filas para atendimento inicial (vermelho, amarelo, verde)
Fila *fila_procedimentos[4][3]; // Filas por procedimento e prioridade

// Procedimentos
Procedimento *triagem;
Procedimento *atendimento;
Procedimento *medidas;
Procedimento *testes;
Procedimento *imagem;
Procedimento *medicamentos;

double relogio = 0.0;
Escalonador *escalonador;

Paciente **Pacientes; // Array de ponteiros para os pacientes

/**
 * Inicializa todas as filas necessárias para a simulação.
 * - Fila de triagem.
 * - Filas de atendimento inicial (vermelho, amarelo, verde).
 * - Filas para cada procedimento (3 por procedimento: vermelho, amarelo, verde).
 */
void inicializa_filas()
{

    // Inicializa fila de triagem
    fila_triagem = inicializa_fila();

    // Atendimento inicial: vermelho, amarelo, verde
    for (int i = 0; i < 3; i++)
    {
        fila_atendimento[i] = inicializa_fila();
    }

    // Procedimentos (medidas, testes, imagens, medicamentos)
    for (int proc = 0; proc < 4; proc++)
    {
        for (int prioridade = 0; prioridade < 3; prioridade++)
        {
            fila_procedimentos[proc][prioridade] = inicializa_fila();
        }
    }
}

/**
 * Finaliza todas as filas da simulação.
 * - Libera a memória de todas as filas criadas.
 */
void finaliza_filas()
{
    // Finaliza a fila de triagem
    finaliza_fila(fila_triagem);

    // Finaliza as filas de atendimento inicial
    for (int i = 0; i < 3; i++)
    { // Vermelho, Amarelo, Verde
        finaliza_fila(fila_atendimento[i]);
    }

    // Finaliza as filas de procedimentos
    for (int proc = 0; proc < 4; proc++)
    { // Procedimentos: Medidas, Testes, Imagem, Medicamentos
        for (int prioridade = 0; prioridade < 3; prioridade++)
        { // Vermelho, Amarelo, Verde
            finaliza_fila(fila_procedimentos[proc][prioridade]);
        }
    }
}

/**
 * Função que lê os parâmetros e pacientes do arquivo de entrada.
 * - Lê tempos e unidades para cada procedimento.
 * - Lê os pacientes e inicializa os eventos de chegada no escalonador.
 * - Inicializa os procedimentos com base nos dados do arquivo.
 * Retorna o número de pacientes lidos.
 */
void carrega_parametros(const char *caminho_arquivo)
{
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo)
    {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", caminho_arquivo);
        exit(EXIT_FAILURE);
    }

    // Lê os tempos e unidades para cada procedimento
    fscanf(arquivo, "%lf %d", &tempos[0], &unidades[0]); // Triagem
    fscanf(arquivo, "%lf %d", &tempos[1], &unidades[1]); // Atendimento inicial
    fscanf(arquivo, "%lf %d", &tempos[2], &unidades[2]); // Medidas hospitalares
    fscanf(arquivo, "%lf %d", &tempos[3], &unidades[3]); // Testes laboratoriais
    fscanf(arquivo, "%lf %d", &tempos[4], &unidades[4]); // Exames de imagem
    fscanf(arquivo, "%lf %d", &tempos[5], &unidades[5]); // Instrumentos/medicamentos

    // Lê o número de pacientes
    fscanf(arquivo, "%d", &num_pacientes);

    fclose(arquivo);
}

void inicializa_pacientes(const char *caminho_arquivo)
{
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo)
    {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", caminho_arquivo);
        exit(EXIT_FAILURE);
    }

    // Pula os parâmetros iniciais já lidos
    for (int i = 0; i < 6; i++)
    {
        fscanf(arquivo, "%*f %*d"); // Ignora tempos e unidades
    }
    fscanf(arquivo, "%*d"); // Ignora o número de pacientes

    // Aloca o array de ponteiros para pacientes
    Pacientes = (Paciente **)malloc(num_pacientes * sizeof(Paciente *));
    if (Pacientes == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array de pacientes.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_pacientes; i++)
    {
        Pacientes[i] = NULL;
    }

    // Lê os dados dos pacientes e inicializa eventos de chegada
    for (int i = 0; i < num_pacientes; i++)
    {
        char id[11];
        int alta, ano, mes, dia, hora, grau, medidas, testes, exames, medicamentos;

        // Lê os dados de cada paciente
        fscanf(arquivo, "%s %d %d %d %d %d %d %d %d %d %d",
               id, &alta, &ano, &mes, &dia, &hora, &grau,
               &medidas, &testes, &exames, &medicamentos);

        // Cria o paciente e armazena no array
        Pacientes[i] = inicializa_paciente(id, alta, ano, mes, dia, hora, grau,
                                           medidas, testes, exames, medicamentos);

        // Insere o evento de chegada no escalonador
        insere_evento(escalonador, hora, 0, Pacientes[i]); // Tipo 0: Chegada
    }

    fclose(arquivo);
}

int determina_proximo_procedimento(Paciente *paciente)
{

    // Verifica alta direta
    if (paciente->alta == 1)
    {
        return 14; // Estado: Alta hospitalar
    }

    if (paciente->estado_atual <= 1)
    {
        // Se o paciente está no estágio inicial, avalia todos os procedimentos
        if (paciente->medidas_hospitalares > 0)
        {
            return 2; // Estado: Na fila de medidas hospitalares
        }
        else if (paciente->testes_laboratorio > 0)
        {
            return 3; // Estado: Na fila de testes laboratoriais
        }
        else if (paciente->exames_imagem > 0)
        {
            return 4; // Estado: Na fila de exames de imagem
        }
        else if (paciente->instrumentos_medicamentos > 0)
        {
            return 5; // Estado: Na fila para instrumentos/medicamentos
        }
        return 14; // Estado: Alta hospitalar
    }
    else
    {
        // Avança para o próximo procedimento baseado no estado atual
        if (paciente->estado_atual < 3 && paciente->medidas_hospitalares > 0)
        {
            return 3; // Estado: Na fila de medidas hospitalares
        }
        else if (paciente->estado_atual < 4 && paciente->testes_laboratorio > 0)
        {
            return 4; // Estado: Na fila de testes laboratoriais
        }
        else if (paciente->estado_atual < 5 && paciente->exames_imagem > 0)
        {
            return 5; // Estado: Na fila de exames de imagem
        }
        else if (paciente->estado_atual < 6 && paciente->instrumentos_medicamentos > 0)
        {
            return 6; // Estado: Na fila para instrumentos/medicamentos
        }
        return 14; // Estado: Alta hospitalar
    }
}

/**
 * Processa os eventos da simulação.
 * - Remove eventos do escalonador em ordem de tempo.
 * - Movimenta pacientes entre filas e procedimentos.
 * - Escalona novos eventos baseados no estado do sistema.
 * - Atualiza estatísticas durante o processamento.
 */
void processa_eventos()
{
    // Enquanto houver eventos no escalonador ou filas não estiverem vazias
    while (!escalonador_vazio(escalonador))
    {
        // Remove o próximo evento do escalonador
        Evento evento = remove_proximo_evento(escalonador);

        // Avança o relógio para o instante do próximo evento
        relogio = evento.tempo;

        // Identifica o paciente associado ao evento
        Paciente *paciente = (Paciente *)evento.referencia;

        int proximo_estado;


        if (strcmp(paciente->identificador, "0009600008") == 0)
        {
            // É o paciente 0009600008
            printf("Paciente encontrado!\n");
        }
        else
        {
            // Não é o paciente 0009600008
            printf("Não é o paciente 0009600008.\n");
        }
        /*
        */

        // Verifica o próximo estado do paciente
        switch (paciente->estado_atual)
        {
        case 0:
            // Tenta alocar o paciente diretamente em uma unidade de triagem
            if (aloca_unidade(triagem))
            {

                // Escalona o evento para o término da triagem
                insere_evento(escalonador, relogio + triagem->tempo_medio, 0, paciente);
                paciente->estado_atual = 1;
            }
            else
            {
                // Paciente não conseguiu atendimento imediato
                // Coloca o paciente na fila de triagem
                paciente_entra_fila(paciente, fila_triagem, relogio);
            }

            break;

        case 1:
            // Atualiza o tempo de triagem
            paciente->tempo_atendimento += triagem->tempo_medio;

            // Libera a unidade de triagem
            libera_unidade(triagem, relogio);

            // Move o paciente atual para o próximo estado
            if (aloca_unidade(atendimento))
            {
                // Espaço disponível no atendimento: escalona o término do atendimento
                insere_evento(escalonador, relogio + atendimento->tempo_medio, paciente->grau_urgencia, paciente); // Tipo 5: Atendimento concluído
                paciente->estado_atual = 2;                                                                        // Estado: Sendo atendido
            }
            else
            {
                // Sem espaço no atendimento: enfileira o paciente
                paciente_entra_fila(paciente, fila_atendimento[paciente->grau_urgencia], relogio);
            }

            // Verifica se há pacientes aguardando na fila de triagem
            if (!fila_vazia(fila_triagem))
            {
                // Remove o próximo paciente da fila
                Paciente *proximo_paciente = paciente_sai_fila(fila_triagem, relogio);

                // Aloca a unidade de triagem e escalona um evento para término
                aloca_unidade(triagem);
                insere_evento(escalonador, relogio + triagem->tempo_medio, paciente->grau_urgencia, proximo_paciente); // Tipo 3: Triagem concluída
                proximo_paciente->estado_atual = 1;                                                                    // Estado: Sendo triado
            }

            break;

        case 2:
            // Atualiza o tempo de atendimento
            paciente->tempo_atendimento += atendimento->tempo_medio;

            // Libera a unidade de atendimento
            libera_unidade(atendimento, relogio);

            // Determina o próximo estado do paciente
            proximo_estado = determina_proximo_procedimento(paciente);

            if (proximo_estado == 14)
            {
                // Paciente recebeu alta
                paciente->estado_atual = 14; // Estado: Alta hospitalar
                // Estatísticas e saída serão tratadas no gera_saida
            }
            else
            {
                // Próximo procedimento necessário
                Procedimento *procedimento = NULL;
                Fila *fila_procedimento = NULL;

                // Seleciona o procedimento e a fila corretos
                switch (proximo_estado)
                {
                case 3: // Medidas hospitalares
                    procedimento = medidas;
                    fila_procedimento = fila_procedimentos[0][paciente->grau_urgencia];
                    break;
                case 4: // Testes laboratoriais
                    procedimento = testes;
                    fila_procedimento = fila_procedimentos[1][paciente->grau_urgencia];
                    break;
                case 5: // Exames de imagem
                    procedimento = imagem;
                    fila_procedimento = fila_procedimentos[2][paciente->grau_urgencia];
                    break;
                case 6: // Instrumentos/medicamentos
                    procedimento = medicamentos;
                    fila_procedimento = fila_procedimentos[3][paciente->grau_urgencia];
                    break;
                }

                // Seta tempo de procedimento baseado no proximo estado
                double tempo_procedimento;

                if (proximo_estado == 3)
                {
                    tempo_procedimento = procedimento->tempo_medio * paciente->medidas_hospitalares;
                }
                else if (proximo_estado == 4)
                {
                    tempo_procedimento = procedimento->tempo_medio * paciente->testes_laboratorio;
                }
                else if (proximo_estado == 5)
                {
                    tempo_procedimento = procedimento->tempo_medio * paciente->exames_imagem;
                }
                else if (proximo_estado == 6)
                {
                    tempo_procedimento = procedimento->tempo_medio * paciente->instrumentos_medicamentos;
                }
                else
                {
                    // Caso improvável: Se houver algum problema, loga o erro
                    fprintf(stderr, "Erro: Paciente %s ainda esta em um estado indefinido. Estado atual: %d \n", paciente->identificador, proximo_estado);
                }

                // Aloca unidade ou coloca na fila
                if (aloca_unidade(procedimento))
                {
                    insere_evento(escalonador, relogio + tempo_procedimento, paciente->grau_urgencia, paciente);
                    paciente->estado_atual = proximo_estado; // Estado: Realizando o procedimento
                }
                else
                {
                    paciente_entra_fila(paciente, fila_procedimento, relogio);
                    paciente->estado_atual = proximo_estado; // Estado: Na fila do procedimento
                }
            }

            // Verifica as filas de atendimento para puxar o próximo paciente
            for (int prioridade = 0; prioridade < 3; prioridade++)
            { // Vermelho, Amarelo, Verde
                if (!fila_vazia(fila_atendimento[prioridade]))
                {
                    // Remove o paciente da fila de maior prioridade disponível
                    Paciente *proximo_paciente = paciente_sai_fila(fila_atendimento[prioridade], relogio);

                    // Aloca a unidade de atendimento e escalona o término do atendimento
                    aloca_unidade(atendimento);
                    insere_evento(escalonador, relogio + atendimento->tempo_medio, paciente->grau_urgencia, proximo_paciente); // Tipo 5: Atendimento concluído
                    proximo_paciente->estado_atual = 2;                                                                        // Estado: Sendo atendido

                    break; // Sai do loop após encontrar um paciente
                }
            }

            break;

        case 3:
        case 4:
        case 5:
        case 6: // Realizando procedimentos -> Concluído
            switch (paciente->estado_atual)
            {
            case 3: // Realizando medidas hospitalares
                paciente->tempo_atendimento += medidas->tempo_medio * paciente->medidas_hospitalares;
                libera_unidade(medidas, relogio);
                break;
            case 4: // Realizando testes laboratoriais
                paciente->tempo_atendimento += testes->tempo_medio * paciente->testes_laboratorio;
                libera_unidade(testes, relogio);
                break;

            case 5: // Realizando exames de imagem
                paciente->tempo_atendimento += imagem->tempo_medio * paciente->exames_imagem;
                libera_unidade(imagem, relogio);
                break;

            case 6: // Realizando instrumentos/medicamentos
                paciente->tempo_atendimento += medicamentos->tempo_medio * paciente->instrumentos_medicamentos;
                libera_unidade(medicamentos, relogio);
                break;

            default:
                fprintf(stderr, "Erro: Estado inválido do paciente %s. Estado: %d.\n", paciente->identificador, paciente->estado_atual);
                break;
            }

            // Determina o próximo estado do paciente
            proximo_estado = determina_proximo_procedimento(paciente);

            if (proximo_estado == 14)
            {
                // Paciente recebeu alta
                paciente->estado_atual = 14; // Estado: Alta hospitalar
            }
            else
            {
                // Próximo procedimento necessário
                Procedimento *procedimento = NULL;
                Fila *fila_procedimento = NULL;
                double tempo_procedimento = 0;
                int quantidade_procedimento = 0; // Variável local para a quantidade

                // Seleciona o procedimento, fila e tempo corretos
                switch (proximo_estado)
                {
                case 3: // Medidas hospitalares
                    procedimento = medidas;
                    fila_procedimento = fila_procedimentos[0][paciente->grau_urgencia];
                    quantidade_procedimento = paciente->medidas_hospitalares;
                    break;
                case 4: // Testes laboratoriais
                    procedimento = testes;
                    fila_procedimento = fila_procedimentos[1][paciente->grau_urgencia];
                    quantidade_procedimento = paciente->testes_laboratorio;
                    break;
                case 5: // Exames de imagem
                    procedimento = imagem;
                    fila_procedimento = fila_procedimentos[2][paciente->grau_urgencia];
                    quantidade_procedimento = paciente->exames_imagem;
                    break;
                case 6: // Instrumentos/medicamentos
                    procedimento = medicamentos;
                    fila_procedimento = fila_procedimentos[3][paciente->grau_urgencia];
                    quantidade_procedimento = paciente->instrumentos_medicamentos;
                    break;
                default:
                    fprintf(stderr, "Erro: Próximo estado inválido (%d) para o paciente %s.\n", proximo_estado, paciente->identificador);
                    break;
                }

                // Calcula o tempo do próximo procedimento
                tempo_procedimento = procedimento->tempo_medio * quantidade_procedimento;

                // Aloca unidade ou coloca na fila
                if (aloca_unidade(procedimento))
                {
                    insere_evento(escalonador, relogio + tempo_procedimento, paciente->grau_urgencia, paciente);
                    paciente->estado_atual = proximo_estado; // Estado: Realizando o procedimento
                }
                else
                {
                    paciente_entra_fila(paciente, fila_procedimento, relogio);
                    paciente->estado_atual = proximo_estado; // Estado: Na fila do procedimento
                }

                // Determina o índice do procedimento para verificar filas
                int procedimento_index = proximo_estado - 4;

                for (int prioridade = 0; prioridade < 3; prioridade++)
                {
                    if (!fila_vazia(fila_procedimentos[procedimento_index][prioridade]))
                    {
                        // Remove o próximo paciente da fila
                        Paciente *proximo_paciente = paciente_sai_fila(fila_procedimentos[procedimento_index][prioridade], relogio);

                        // Determina o estado do próximo paciente
                        int estado_proximo_paciente = proximo_paciente->estado_atual;
                        if (estado_proximo_paciente == 14)
                        {
                            // Paciente recebeu alta
                            proximo_paciente->estado_atual = 14; // Estado: Alta hospitalar
                        }
                        else
                        {
                            // Determina o procedimento, fila, tempo e quantidade para o próximo paciente
                            Procedimento *procedimento_proximo_paciente = NULL;
                            Fila *fila_procedimento_proximo_paciente = NULL;
                            double tempo_procedimento_proximo_paciente = 0;
                            int quantidade_procedimento_proximo_paciente = 0;

                            switch (estado_proximo_paciente)
                            {
                            case 3: // Medidas hospitalares
                                procedimento_proximo_paciente = medidas;
                                fila_procedimento_proximo_paciente = fila_procedimentos[0][proximo_paciente->grau_urgencia];
                                quantidade_procedimento_proximo_paciente = proximo_paciente->medidas_hospitalares;
                                break;
                            case 4: // Testes laboratoriais
                                procedimento_proximo_paciente = testes;
                                fila_procedimento_proximo_paciente = fila_procedimentos[1][proximo_paciente->grau_urgencia];
                                quantidade_procedimento_proximo_paciente = proximo_paciente->testes_laboratorio;
                                break;
                            case 5: // Exames de imagem
                                procedimento_proximo_paciente = imagem;
                                fila_procedimento_proximo_paciente = fila_procedimentos[2][proximo_paciente->grau_urgencia];
                                quantidade_procedimento_proximo_paciente = proximo_paciente->exames_imagem;
                                break;
                            case 6: // Instrumentos/medicamentos
                                procedimento_proximo_paciente = medicamentos;
                                fila_procedimento_proximo_paciente = fila_procedimentos[3][proximo_paciente->grau_urgencia];
                                quantidade_procedimento_proximo_paciente = proximo_paciente->instrumentos_medicamentos;
                                break;
                            default:
                                fprintf(stderr, "Erro: Estado inesperado %d para o paciente %s\n", estado_proximo_paciente, proximo_paciente->identificador);
                                continue;
                            }

                            // Calcula o tempo do próximo procedimento
                            tempo_procedimento_proximo_paciente = procedimento_proximo_paciente->tempo_medio * quantidade_procedimento_proximo_paciente;

                            // Verifica disponibilidade no procedimento ou insere na fila
                            if (aloca_unidade(procedimento_proximo_paciente))
                            {
                                // Insere evento no escalonador
                                insere_evento(escalonador, relogio + tempo_procedimento_proximo_paciente, proximo_paciente->grau_urgencia, proximo_paciente);
                                proximo_paciente->estado_atual = estado_proximo_paciente; // Atualiza estado do paciente
                            }
                            else
                            {
                                // Insere o paciente na fila do próximo procedimento
                                paciente_entra_fila(proximo_paciente, fila_procedimento_proximo_paciente, relogio);
                                proximo_paciente->estado_atual = estado_proximo_paciente; // Atualiza estado do paciente
                            }

                            break; // Sai do loop após processar o paciente
                        }
                    }
                }
            }

            break;
        case 14:
            break;

        default:
            fprintf(stderr, "Erro: Estado inválido do paciente %s. Estado: %d.\n", paciente->identificador, paciente->estado_atual);
        }
    }
}

/**
 * Gera e exibe a saída esperada da simulação.
 * - Mostra os dados de cada paciente que obteve alta.
 * - Exibe estatísticas gerais baseadas nas filas e procedimentos.
 */
void gera_saida()
{
    // Cabeçalho da tabela
    printf("ID           Admissão               Alta                 Tempo Total (h)   Tempo Atendimento (h)   Tempo Espera (h)\n");
    printf("---------------------------------------------------------------------------------------------------------------\n");

    // Percorre todos os pacientes no array global
    for (int i = 0; i < num_pacientes; i++)
    {
        Paciente *paciente = Pacientes[i]; // Acessa o paciente diretamente no array

        // Calcula o tempo total no hospital
        double tempo_total = paciente->tempo_atendimento + paciente->tempo_espera;

        // Formata a data de admissão
        char data_admissao[20];
        snprintf(data_admissao, sizeof(data_admissao), "%04d-%02d-%02d %02d:00:00",
                 paciente->ano, paciente->mes, paciente->dia, paciente->hora);

        // Calcula a data de alta
        struct tm data_alta = {0};
        data_alta.tm_year = paciente->ano - 1900; // Ano de 1900
        data_alta.tm_mon = paciente->mes - 1;     // Meses começam em 0
        data_alta.tm_mday = paciente->dia;
        data_alta.tm_hour = paciente->hora;

        // Converte o tempo total para segundos e adiciona à data de admissão
        time_t tempo_em_segundos = (time_t)(tempo_total * 3600); // Tempo total em segundos
        time_t data_alta_em_segundos = mktime(&data_alta) + tempo_em_segundos;
        struct tm *data_alta_calculada = localtime(&data_alta_em_segundos);

        // Formata a data de alta
        char data_alta_formatada[20];
        snprintf(data_alta_formatada, sizeof(data_alta_formatada), "%04d-%02d-%02d %02d:%02d:%02d",
                 data_alta_calculada->tm_year + 1900,
                 data_alta_calculada->tm_mon + 1,
                 data_alta_calculada->tm_mday,
                 data_alta_calculada->tm_hour,
                 data_alta_calculada->tm_min,
                 data_alta_calculada->tm_sec);

        // Imprime os dados formatados para o paciente
        printf("%-12s %-20s %-20s %-17.3f %-24.3f %-18.3f\n",
               paciente->identificador, data_admissao, data_alta_formatada,
               tempo_total, paciente->tempo_atendimento, paciente->tempo_espera);
    }
}

/**
 * Função principal da simulação.
 * - Lê o arquivo de entrada e inicializa as estruturas.
 * - Processa todos os eventos da simulação.
 * - Gera a saída final com as estatísticas.
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <caminho do arquivo de entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Inicializa todas as filas necessárias
    inicializa_filas();

    // Carrega os parâmetros do arquivo
    carrega_parametros(argv[1]);

    // Inicializa as estruturas baseadas nos parâmetros lidos
    escalonador = inicializa_escalonador(num_pacientes);

    // Inicializa os procedimentos
    triagem = inicializa_procedimento("Triagem", tempos[0], unidades[0]);
    atendimento = inicializa_procedimento("Atendimento", tempos[1], unidades[1]);
    medidas = inicializa_procedimento("Medidas Hospitalares", tempos[2], unidades[2]);
    testes = inicializa_procedimento("Testes Laboratoriais", tempos[3], unidades[3]);
    imagem = inicializa_procedimento("Exames de Imagem", tempos[4], unidades[4]);
    medicamentos = inicializa_procedimento("Instrumentos/Medicamentos", tempos[5], unidades[5]);

    inicializa_pacientes(argv[1]);

    // TODO: Processar eventos e gerar saída
    processa_eventos();
    gera_saida();

    // Finaliza recursos alocados
    finaliza_escalonador(escalonador);
    finaliza_filas();
    finaliza_procedimento(triagem);
    finaliza_procedimento(atendimento);
    finaliza_procedimento(medidas);
    finaliza_procedimento(testes);
    finaliza_procedimento(imagem);
    finaliza_procedimento(medicamentos);

    // Libera o array de pacientes
    free(Pacientes);

    return 0;
}
