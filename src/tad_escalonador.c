#include "tad_escalonador.h"

// Função auxiliar para trocar dois eventos
void troca_eventos(Evento* a, Evento* b) {
    Evento temp = *a;
    *a = *b;
    *b = temp;
}

// Ajusta o heap para manter a propriedade de min-heap (heapify)
void heapify_cima(Escalonador* escalonador, int index) {
    int pai = (index - 1) / 2;
    while (index > 0 && escalonador->heap[index].tempo < escalonador->heap[pai].tempo) {
        troca_eventos(&escalonador->heap[index], &escalonador->heap[pai]);
        index = pai;
        pai = (index - 1) / 2;
    }
}

void heapify_baixo(Escalonador* escalonador, int index) {
    int menor = index;
    int esq = 2 * index + 1;
    int dir = 2 * index + 2;

    if (esq < escalonador->tamanho && escalonador->heap[esq].tempo < escalonador->heap[menor].tempo) {
        menor = esq;
    }
    if (dir < escalonador->tamanho && escalonador->heap[dir].tempo < escalonador->heap[menor].tempo) {
        menor = dir;
    }
    if (menor != index) {
        troca_eventos(&escalonador->heap[index], &escalonador->heap[menor]);
        heapify_baixo(escalonador, menor);
    }
}

// Inicializa um escalonador com capacidade dada
Escalonador* inicializa_escalonador(int capacidade) {
    Escalonador* escalonador = (Escalonador*)malloc(sizeof(Escalonador));
    if (escalonador == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o escalonador.\n");
        exit(EXIT_FAILURE);
    }
    escalonador->heap = (Evento*)malloc(capacidade * sizeof(Evento));
    if (escalonador->heap == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o heap.\n");
        free(escalonador);
        exit(EXIT_FAILURE);
    }
    escalonador->tamanho = 0;
    escalonador->capacidade = capacidade;
    return escalonador;
}

// Insere um evento no escalonador
void insere_evento(Escalonador* escalonador, double tempo, int tipo, void* referencia) {
    if (escalonador->tamanho == escalonador->capacidade) {
        fprintf(stderr, "Erro: Escalonador cheio.\n");
        return;
    }
    escalonador->heap[escalonador->tamanho].tempo = tempo;
    escalonador->heap[escalonador->tamanho].tipo = tipo;
    escalonador->heap[escalonador->tamanho].referencia = referencia;
    escalonador->tamanho++;
    heapify_cima(escalonador, escalonador->tamanho - 1);
}

// Remove o próximo evento do escalonador
Evento remove_proximo_evento(Escalonador* escalonador) {
    if (escalonador_vazio(escalonador)) {
        fprintf(stderr, "Erro: Escalonador vazio.\n");
        exit(EXIT_FAILURE);
    }
    Evento proximo = escalonador->heap[0];
    escalonador->heap[0] = escalonador->heap[escalonador->tamanho - 1];
    escalonador->tamanho--;
    heapify_baixo(escalonador, 0);
    return proximo;
}

// Verifica se o escalonador está vazio
int escalonador_vazio(Escalonador* escalonador) {
    return escalonador->tamanho == 0;
}

// Finaliza o escalonador e libera a memória
void finaliza_escalonador(Escalonador* escalonador) {
    if (escalonador != NULL) {
        free(escalonador->heap);
        free(escalonador);
    }
}
