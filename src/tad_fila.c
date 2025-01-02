#include "tad_paciente_fila.h"

// Inicializa uma fila vazia
Fila* inicializa_fila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    if (fila == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a fila.\n");
        exit(EXIT_FAILURE);
    }
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->tempo_total_espera = 0.0;
    return fila;
}

// Enfileira um paciente na fila
void enfileira(Fila* fila, Paciente* paciente) {
    Nodo* novo_nodo = (Nodo*)malloc(sizeof(Nodo));
    if (novo_nodo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para o nodo.\n");
        exit(EXIT_FAILURE);
    }
    novo_nodo->paciente = paciente;
    novo_nodo->proximo = NULL;

    if (fila->fim != NULL) {
        fila->fim->proximo = novo_nodo;
    } else {
        fila->inicio = novo_nodo;
    }
    fila->fim = novo_nodo;
    fila->tamanho++;
}

// Desenfileira o paciente mais antigo da fila
Paciente* desenfileira(Fila* fila) {
    if (fila_vazia(fila)) {
        return NULL;
    }
    Nodo* nodo_removido = fila->inicio;
    Paciente* paciente = nodo_removido->paciente;
    fila->inicio = nodo_removido->proximo;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    free(nodo_removido);
    fila->tamanho--;
    return paciente;
}

// Verifica se a fila está vazia
int fila_vazia(Fila* fila) {
    return fila->tamanho == 0;
}

// Finaliza a fila e libera os recursos
void finaliza_fila(Fila* fila) {
    while (!fila_vazia(fila)) {
        Paciente* paciente = desenfileira(fila);
        libera_paciente(paciente);
    }
    free(fila);
}

// Exibe os pacientes da fila (para debug)
void exibe_fila(Fila* fila) {
    Nodo* atual = fila->inicio;
    printf("Fila (tamanho: %d):\n", fila->tamanho);
    while (atual != NULL) {
        exibe_paciente(atual->paciente);
        atual = atual->proximo;
    }
}
