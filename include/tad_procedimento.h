#ifndef TAD_PROCEDIMENTO_H
#define TAD_PROCEDIMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad_fila.h"

// Estrutura de Procedimento
typedef struct {
    char nome[50];          // Nome do procedimento
    double tempo_medio;     // Tempo médio por unidade (em horas)
    int unidades_totais;    // Número total de unidades
    int unidades_ocupadas;  // Número de unidades atualmente ocupadas
    double tempo_ocupado;   // Tempo total de ocupação (estatísticas)
    double tempo_ocioso;    // Tempo total ocioso (estatísticas)
    Fila* fila;             // Fila de espera associada ao procedimento
} Procedimento;

// Funções para manipulação de Procedimentos
Procedimento* inicializa_procedimento(const char* nome, double tempo_medio, int unidades_totais);
int aloca_unidade(Procedimento* procedimento);
void libera_unidade(Procedimento* procedimento, double tempo_duracao);
void atualiza_tempo_ocioso(Procedimento* procedimento, double tempo_atual);
void exibe_procedimento(const Procedimento* procedimento);
void finaliza_procedimento(Procedimento* procedimento);

#endif
