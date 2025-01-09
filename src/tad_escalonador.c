#include "tad_escalonador.h"


/*
Vamos implementar uma priorização entre eventos que tem o mesmo horario
A proposta do professor é que seja feito pelo ID do paciente.

Ou seja quem tem o menor ID deve ser priorizado
*/

// Troca dois eventos no array
static void troca_eventos(Evento* a, Evento* b) {
    Evento temp = *a;
    *a = *b;
    *b = temp;
}

// Compara dois eventos, considerando tempo e prioridade
static int compara_eventos(const Evento* a, const Evento* b) {
    if (a->tempo < b->tempo) {
        return -1; // a vem antes de b
    } else if (a->tempo > b->tempo) {
        return 1; // b vem antes de a
    } else {
        // Se tempos forem iguais, considerar prioridade
        return b->prioridade - a->prioridade; // Maior prioridade vem antes
    }
}

// Ajusta o heap para manter a propriedade de min-heap ao inserir (heapify cima)
static void heapify_cima(Escalonador* escalonador, int index) {
    int pai = (index - 1) / 2;
    while (index > 0 &&
           compara_eventos(&escalonador->heap[index], &escalonador->heap[pai]) < 0) {
        troca_eventos(&escalonador->heap[index], &escalonador->heap[pai]);
        index = pai;
        pai = (index - 1) / 2;
    }
}

// Ajusta o heap para manter a propriedade de min-heap ao remover (heapify baixo)
static void heapify_baixo(Escalonador* escalonador, int index) {
    int menor = index;
    int esq = 2 * index + 1;
    int dir = 2 * index + 2;

    if (esq < escalonador->tamanho &&
        compara_eventos(&escalonador->heap[esq], &escalonador->heap[menor]) < 0) {
        menor = esq;
    }
    if (dir < escalonador->tamanho &&
        compara_eventos(&escalonador->heap[dir], &escalonador->heap[menor]) < 0) {
        menor = dir;
    }
    if (menor != index) {
        troca_eventos(&escalonador->heap[index], &escalonador->heap[menor]);
        heapify_baixo(escalonador, menor);
    }
}

// Inicializa o escalonador com uma capacidade definida
Escalonador* inicializa_escalonador(int capacidade) {
    Escalonador* escalonador = (Escalonador*)malloc(sizeof(Escalonador));
    if (!escalonador) {
        fprintf(stderr, "Erro ao alocar memória para o escalonador.\n");
        exit(EXIT_FAILURE);
    }

    escalonador->heap = (Evento*)malloc(capacidade * sizeof(Evento));
    if (!escalonador->heap) {
        fprintf(stderr, "Erro ao alocar memória para o heap do escalonador.\n");
        free(escalonador);
        exit(EXIT_FAILURE);
    }

    escalonador->tamanho = 0;
    escalonador->capacidade = capacidade;
    return escalonador;
}

// Insere um evento no escalonador
void insere_evento(Escalonador* escalonador, double tempo, int prioridade, void* referencia) {
    if (escalonador->tamanho == escalonador->capacidade) {
        fprintf(stderr, "Erro: Escalonador cheio.\n");
        return;
    }

    Evento novo_evento = { .tempo = tempo, .prioridade = prioridade, .referencia = referencia };
    escalonador->heap[escalonador->tamanho] = novo_evento;
    escalonador->tamanho++;
    heapify_cima(escalonador, escalonador->tamanho - 1);
}

// Remove o próximo evento do escalonador
Evento remove_proximo_evento(Escalonador* escalonador) {
    if (escalonador_vazio(escalonador)) {
        fprintf(stderr, "Erro: Escalonador vazio.\n");
        exit(EXIT_FAILURE);
    }

    Evento proximo_evento = escalonador->heap[0];
    escalonador->heap[0] = escalonador->heap[escalonador->tamanho - 1];
    escalonador->tamanho--;
    heapify_baixo(escalonador, 0);
    return proximo_evento;
}

// Verifica se o escalonador está vazio
int escalonador_vazio(Escalonador* escalonador) {
    return escalonador->tamanho == 0;
}

// Finaliza o escalonador e libera a memória
void finaliza_escalonador(Escalonador* escalonador) {
    if (escalonador) {
        free(escalonador->heap);
        free(escalonador);
    }
}
