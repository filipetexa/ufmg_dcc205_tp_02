#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad_paciente.h"
#include "tad_fila.h"

void test_inicializa_paciente() {
    printf("=== Teste: inicializa_paciente ===\n");
    Paciente* paciente = inicializa_paciente("1234567890", 1, 2024, 12, 27, 15, 2, 5, 3, 2, 1);
    if (paciente == NULL) {
        printf("Falhou: Paciente não inicializado corretamente.\n");
        return;
    }
    if (strcmp(paciente->identificador, "1234567890") != 0 || paciente->grau_urgencia != 2) {
        printf("Falhou: Dados do paciente estão incorretos.\n");
    } else {
        printf("Passou.\n");
    }
    libera_paciente(paciente);
}

void test_atualiza_estado_paciente() {
    printf("=== Teste: atualiza_estado_paciente ===\n");
    Paciente* paciente = inicializa_paciente("1234567890", 1, 2024, 12, 27, 15, 2, 5, 3, 2, 1);
    atualiza_estado_paciente(paciente, 3);
    if (paciente->estado_atual != 3) {
        printf("Falhou: Estado do paciente não atualizado corretamente.\n");
    } else {
        printf("Passou.\n");
    }
    libera_paciente(paciente);
}

void test_registra_tempo() {
    printf("=== Teste: registra_tempo ===\n");
    Paciente* paciente = inicializa_paciente("1234567890", 1, 2024, 12, 27, 15, 2, 5, 3, 2, 1);
    registra_tempo_espera(paciente, 1.5);
    registra_tempo_atendimento(paciente, 2.0);
    if (paciente->tempo_espera != 1.5 || paciente->tempo_atendimento != 2.0) {
        printf("Falhou: Tempo de espera ou atendimento não registrado corretamente.\n");
    } else {
        printf("Passou.\n");
    }
    libera_paciente(paciente);
}

// Testes Fila
void test_fila() {
    printf("=== Teste: TAD Fila ===\n");

    Fila* fila = inicializa_fila();

    // Criar pacientes para teste
    Paciente* p1 = inicializa_paciente("0001", 0, 2024, 1, 1, 10, 1, 1, 1, 1, 1);
    Paciente* p2 = inicializa_paciente("0002", 0, 2024, 1, 1, 11, 2, 2, 2, 2, 2);

    // Testar enfileiramento
    enfileira(fila, p1);
    enfileira(fila, p2);

    if (fila->tamanho != 2) {
        printf("Falhou: Tamanho da fila incorreto após enfileirar.\n");
    } else {
        printf("Passou: Enfileiramento.\n");
    }

    // Testar desenfileiramento
    Paciente* removido = desenfileira(fila);
    if (removido != p1) {
        printf("Falhou: Desenfileiramento incorreto.\n");
    } else {
        printf("Passou: Desenfileiramento.\n");
    }

    // Testar fila vazia
    desenfileira(fila); // Remove o último
    if (!fila_vazia(fila)) {
        printf("Falhou: Fila deveria estar vazia.\n");
    } else {
        printf("Passou: Verificação de fila vazia.\n");
    }

    // Finalizar fila
    finaliza_fila(fila);
    printf("Finalizados testes para TAD Fila.\n");
}


int main() {
    printf("==== Iniciando Testes ====\n");

    test_inicializa_paciente();
    test_atualiza_estado_paciente();
    test_registra_tempo();

    test_fila();

    // Novos testes serão adicionados aqui conforme implementamos mais funcionalidades.

    printf("==== Testes Finalizados ====\n");
    return 0;
}
