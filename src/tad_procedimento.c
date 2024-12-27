#include "tad_procedimento.h"

// Inicializa um procedimento
Procedimento* inicializa_procedimento(const char* nome, double tempo_medio, int num_unidades) {
    Procedimento* procedimento = (Procedimento*)malloc(sizeof(Procedimento));
    if (procedimento == NULL) {
        fprintf(stderr, "Erro ao alocar memória para procedimento.\n");
        exit(EXIT_FAILURE);
    }

    strncpy(procedimento->nome, nome, 49);
    procedimento->nome[49] = '\0';
    procedimento->tempo_medio = tempo_medio;
    procedimento->num_unidades = num_unidades;
    procedimento->unidades = (Unidade*)malloc(num_unidades * sizeof(Unidade));

    if (procedimento->unidades == NULL) {
        fprintf(stderr, "Erro ao alocar memória para unidades do procedimento.\n");
        free(procedimento);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_unidades; i++) {
        procedimento->unidades[i].ocupado = 0;
        procedimento->unidades[i].tempo_ocioso = 0.0;
        procedimento->unidades[i].tempo_ocupado = 0.0;
    }

    procedimento->total_atendimentos = 0;
    return procedimento;
}

// Aloca uma unidade para atender um paciente
int aloca_unidade(Procedimento* procedimento, double tempo_atual) {
    for (int i = 0; i < procedimento->num_unidades; i++) {
        if (!procedimento->unidades[i].ocupado) {
            procedimento->unidades[i].ocupado = 1;
            procedimento->unidades[i].tempo_ocioso += tempo_atual;
            procedimento->total_atendimentos++;
            return i; // Retorna o índice da unidade alocada
        }
    }
    return -1; // Nenhuma unidade disponível
}

// Libera uma unidade após atendimento
void libera_unidade(Procedimento* procedimento, int unidade, double tempo_atual) {
    if (unidade < 0 || unidade >= procedimento->num_unidades) {
        fprintf(stderr, "Erro: Unidade inválida.\n");
        return;
    }
    procedimento->unidades[unidade].ocupado = 0;
    procedimento->unidades[unidade].tempo_ocupado += tempo_atual;
}

// Exibe informações do procedimento
void exibe_procedimento(const Procedimento* procedimento) {
    printf("Procedimento: %s\n", procedimento->nome);
    printf("Tempo médio: %.2f\n", procedimento->tempo_medio);
    printf("Número de unidades: %d\n", procedimento->num_unidades);
    printf("Total de atendimentos: %d\n", procedimento->total_atendimentos);
    for (int i = 0; i < procedimento->num_unidades; i++) {
        printf("Unidade %d - Ocupado: %d, Tempo Ocioso: %.2f, Tempo Ocupado: %.2f\n",
               i, procedimento->unidades[i].ocupado,
               procedimento->unidades[i].tempo_ocioso,
               procedimento->unidades[i].tempo_ocupado);
    }
}

// Finaliza o procedimento e libera a memória
void finaliza_procedimento(Procedimento* procedimento) {
    if (procedimento != NULL) {
        free(procedimento->unidades);
        free(procedimento);
    }
}
