#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tad_paciente.h"
#include "tad_fila.h"
#include "tad_procedimento.h"
#include "tad_escalonador.h"

// Testes para TAD Paciente
void test_paciente() {
    printf("=== Teste: TAD Paciente ===\n");

    // Teste de inicialização
    Paciente* paciente = inicializa_paciente("1234567890", 1, 2024, 12, 27, 15, 2, 5, 3, 2, 1);
    if (paciente == NULL) {
        printf("Falhou: Paciente não inicializado corretamente.\n");
    } else if (strcmp(paciente->identificador, "1234567890") != 0 || paciente->grau_urgencia != 2) {
        printf("Falhou: Dados do paciente estão incorretos.\n");
    } else {
        printf("Passou: Inicialização do paciente.\n");
    }

    // Teste de atualização de estado
    atualiza_estado_paciente(paciente, 3);
    if (paciente->estado_atual != 3) {
        printf("Falhou: Estado do paciente não atualizado corretamente.\n");
    } else {
        printf("Passou: Atualização de estado.\n");
    }

    // Teste de registro de tempos
    registra_tempo_espera(paciente, 1.5);
    registra_tempo_atendimento(paciente, 2.0);
    if (paciente->tempo_espera != 1.5 || paciente->tempo_atendimento != 2.0) {
        printf("Falhou: Tempo de espera ou atendimento não registrado corretamente.\n");
    } else {
        printf("Passou: Registro de tempos.\n");
    }

    // Liberar memória do paciente
    libera_paciente(paciente);
    printf("Finalizados testes para TAD Paciente.\n");
}

// Testes para TAD Fila
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

// Testes para TAD Procedimento
void test_procedimento() {
    printf("=== Teste: TAD Procedimento ===\n");

    Procedimento* triagem = inicializa_procedimento("Triagem", 0.2, 3);

    // Testar alocação de unidades
    int unidade = aloca_unidade(triagem, 1.0);
    if (unidade == -1) {
        printf("Falhou: Nenhuma unidade foi alocada.\n");
    } else {
        printf("Passou: Unidade %d alocada.\n", unidade);
    }

    // Testar liberação de unidade
    libera_unidade(triagem, unidade, 0.2);
    if (triagem->unidades[unidade].ocupado) {
        printf("Falhou: Unidade não foi liberada.\n");
    } else {
        printf("Passou: Unidade liberada.\n");
    }

    // Exibir estado do procedimento
    exibe_procedimento(triagem);

    // Finalizar procedimento
    finaliza_procedimento(triagem);
    printf("Finalizados testes para TAD Procedimento.\n");
}

// Testes para TAD Escalonador
void test_escalonador() {
    printf("=== Teste: TAD Escalonador ===\n");

    Escalonador* escalonador = inicializa_escalonador(5);

    // Testar inserção de eventos
    insere_evento(escalonador, 2.0, 1, NULL);
    insere_evento(escalonador, 1.0, 2, NULL);
    insere_evento(escalonador, 3.0, 3, NULL);

    Evento evento = remove_proximo_evento(escalonador);
    if (evento.tempo != 1.0) {
        printf("Falhou: Evento removido está fora de ordem.\n");
    } else {
        printf("Passou: Ordem de prioridade.\n");
    }

    // Testar se o escalonador está vazio
    remove_proximo_evento(escalonador);
    remove_proximo_evento(escalonador);
    if (!escalonador_vazio(escalonador)) {
        printf("Falhou: Escalonador deveria estar vazio.\n");
    } else {
        printf("Passou: Verificação de escalonador vazio.\n");
    }

    finaliza_escalonador(escalonador);
    printf("Finalizados testes para TAD Escalonador.\n");
}

int main() {
    printf("==== Iniciando Testes ====\n");

    test_paciente();
    test_fila();
    test_procedimento();
    test_escalonador();

    printf("==== Testes Finalizados ====\n");
    return 0;
}
