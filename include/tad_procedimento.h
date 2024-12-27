#ifndef TAD_PROCEDIMENTO_H
#define TAD_PROCEDIMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de uma unidade de serviço
typedef struct {
    int ocupado;      // 0: ocioso, 1: ocupado
    double tempo_ocioso; // Tempo total ocioso
    double tempo_ocupado; // Tempo total ocupado
} Unidade;

// Estrutura do procedimento
typedef struct {
    char nome[50];     // Nome do procedimento (ex: Triagem)
    double tempo_medio; // Tempo médio para concluir o serviço
    int num_unidades;   // Número de unidades disponíveis
    Unidade* unidades;  // Array de unidades
    int total_atendimentos; // Total de atendimentos realizados
} Procedimento;

// Funções para manipulação de procedimentos
Procedimento* inicializa_procedimento(const char* nome, double tempo_medio, int num_unidades);
int aloca_unidade(Procedimento* procedimento, double tempo_atual);
void libera_unidade(Procedimento* procedimento, int unidade, double tempo_atual);
void exibe_procedimento(const Procedimento* procedimento);
void finaliza_procedimento(Procedimento* procedimento);

#endif
