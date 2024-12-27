#ifndef TAD_ESCALONADOR_H
#define TAD_ESCALONADOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de um evento no escalonador
typedef struct {
    double tempo;        // Tempo do evento
    int tipo;            // Tipo do evento (ex: chegada, atendimento)
    void* referencia;    // Referência ao paciente ou procedimento
} Evento;

// Estrutura do escalonador
typedef struct {
    Evento* heap;        // Array de eventos
    int tamanho;         // Número de eventos na fila
    int capacidade;      // Capacidade máxima do heap
} Escalonador;

// Funções para manipulação do escalonador
Escalonador* inicializa_escalonador(int capacidade);
void insere_evento(Escalonador* escalonador, double tempo, int tipo, void* referencia);
Evento remove_proximo_evento(Escalonador* escalonador);
int escalonador_vazio(Escalonador* escalonador);
void finaliza_escalonador(Escalonador* escalonador);

#endif
