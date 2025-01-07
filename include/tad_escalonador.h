#ifndef TAD_ESCALONADOR_H
#define TAD_ESCALONADOR_H

#include <stdio.h>
#include <stdlib.h>

// Estrutura de um evento no escalonador
typedef struct {
    double tempo;        // Tempo do evento
    int prioridade;      // Prioridade do evento (ex: 0, 1, 2)
    void* referencia;    // Referência ao paciente ou procedimento
} Evento;

// Estrutura do escalonador
typedef struct {
    Evento* heap;        // Array de eventos
    int tamanho;         // Número de eventos na heap
    int capacidade;      // Capacidade máxima da heap
} Escalonador;

// Funções para manipulação do escalonador
Escalonador* inicializa_escalonador(int capacidade);
void insere_evento(Escalonador* escalonador, double tempo, int prioridade, void* referencia);
Evento remove_proximo_evento(Escalonador* escalonador);
int escalonador_vazio(Escalonador* escalonador);
void finaliza_escalonador(Escalonador* escalonador);

#endif
