#include "tad_procedimento.h"

// Inicializa um Procedimento
Procedimento* inicializa_procedimento(const char* nome, double tempo_medio, int unidades_totais) {
    Procedimento* procedimento = (Procedimento*)malloc(sizeof(Procedimento));
    if (!procedimento) {
        fprintf(stderr, "Erro ao alocar memória para o Procedimento.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(procedimento->nome, nome, 49);
    procedimento->nome[49] = '\0';
    procedimento->tempo_medio = tempo_medio;
    procedimento->unidades_totais = unidades_totais;
    procedimento->unidades_ocupadas = 0;
    procedimento->tempo_ocupado = 0.0;
    procedimento->tempo_ocioso = 0.0;
    procedimento->fila = inicializa_fila();

    return procedimento;
}

// Aloca uma unidade, se disponível
int aloca_unidade(Procedimento* procedimento) {
    if (procedimento->unidades_ocupadas < procedimento->unidades_totais) {
        procedimento->unidades_ocupadas++;
        return 1; // Unidade alocada com sucesso
    }
    return 0; // Nenhuma unidade disponível
}

// Libera uma unidade após o atendimento
void libera_unidade(Procedimento* procedimento, double tempo_duracao) {
    if (procedimento->unidades_ocupadas > 0) {
        procedimento->unidades_ocupadas--;
        procedimento->tempo_ocupado += tempo_duracao;
    } else {
        fprintf(stderr, "Erro: Tentativa de liberar unidade inexistente. procedimento: %s \n", procedimento->nome);
    }
}

// Atualiza o tempo ocioso com base no tempo atual
void atualiza_tempo_ocioso(Procedimento* procedimento, double tempo_atual) {
    double unidades_ociosas = procedimento->unidades_totais - procedimento->unidades_ocupadas;
    procedimento->tempo_ocioso += unidades_ociosas * tempo_atual;
}

// Exibe informações sobre o Procedimento
void exibe_procedimento(const Procedimento* procedimento) {
    printf("Procedimento: %s\n", procedimento->nome);
    printf("Tempo médio: %.2f horas\n", procedimento->tempo_medio);
    printf("Unidades totais: %d\n", procedimento->unidades_totais);
    printf("Unidades ocupadas: %d\n", procedimento->unidades_ocupadas);
    printf("Tempo total ocupado: %.2f horas\n", procedimento->tempo_ocupado);
    printf("Tempo total ocioso: %.2f horas\n", procedimento->tempo_ocioso);
    printf("Fila de espera:\n");
    exibe_fila(procedimento->fila);
}

// Finaliza e libera memória do Procedimento
void finaliza_procedimento(Procedimento* procedimento) {
    if (procedimento) {
        finaliza_fila(procedimento->fila);
        free(procedimento);
    }
}
