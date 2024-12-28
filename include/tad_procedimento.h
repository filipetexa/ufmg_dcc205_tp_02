#ifndef TAD_PROCEDIMENTO_H
#define TAD_PROCEDIMENTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad_fila.h"

// Estrutura de uma unidade de serviço
typedef struct {
    int ocupado;      // 0: ocioso, 1: ocupado
    double tempo_ocioso; // Tempo total ocioso
    double tempo_ocupado; // Tempo total ocupado
} Unidade;

typedef struct {
    char nome[50];
    double tempo_medio;
    int num_unidades;
    Unidade* unidades;  
    Fila* fila;         
    int total_atendimentos;
} Procedimento;


// Funções para manipulação de procedimentos
Procedimento* inicializa_procedimento(const char* nome, double tempo_medio, int num_unidades);
int aloca_unidade(Procedimento* procedimento, double tempo_atual);
void libera_unidade(Procedimento* procedimento, int unidade, double tempo_atual);
void exibe_procedimento(const Procedimento* procedimento);
void finaliza_procedimento(Procedimento* procedimento);
int unidade_disponivel(Procedimento* procedimento);
void registra_ocupacao(Procedimento* procedimento, int unidade, double tempo_inicio, double duracao);

#endif
