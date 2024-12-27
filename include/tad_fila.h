#ifndef TAD_FILA_H
#define TAD_FILA_H

#include <stdio.h>
#include <stdlib.h>
#include "tad_paciente.h"

// Estrutura para os nós da fila
typedef struct Nodo {
    Paciente* paciente;
    struct Nodo* proximo;
} Nodo;

// Estrutura da fila
typedef struct {
    Nodo* inicio;
    Nodo* fim;
    int tamanho;
    double tempo_total_espera; // Para estatísticas
} Fila;

// Funções para manipulação da fila
Fila* inicializa_fila();
void enfileira(Fila* fila, Paciente* paciente);
Paciente* desenfileira(Fila* fila);
int fila_vazia(Fila* fila);
void finaliza_fila(Fila* fila);
void exibe_fila(Fila* fila);

#endif
