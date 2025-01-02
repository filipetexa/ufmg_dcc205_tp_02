#ifndef TAD_PACIENTE_FILA_H
#define TAD_PACIENTE_FILA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
NOTA: As TADs paciente e fila foram colocadas no mesmo arquivo de cabeçalho porque estavam 
gerando uma dependencia circular entre elas. Sendo assim, coloca-las no mesmo .h foi uma solução rapida
e facil para contornar o problema.
*/

// Estrutura para armazenar informações de um paciente
typedef struct {
    
    char identificador[11]; // Identificador único
    int alta;              // 0: Não teve alta, 1: Teve alta
    int ano;
    int mes;
    int dia;
    int hora;
    int grau_urgencia;     // 0: Verde, 1: Amarelo, 2: Vermelho
    int medidas_hospitalares;
    int testes_laboratorio;
    int exames_imagem;
    int instrumentos_medicamentos;
    int estado_atual;      // Estado atual do paciente
    double tempo_espera;   // Tempo total de espera
    double tempo_atendimento; // Tempo total em atendimento
    double hora_entrada_fila; // Horário de entrada na fila atual
} Paciente;


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

// Funções para manipulação de pacientes
Paciente* inicializa_paciente(char* id, int alta, int ano, int mes, int dia, int hora, int grau,
                              int medidas, int testes, int exames, int instrumentos);
void atualiza_estado_paciente(Paciente* paciente, int novo_estado);
void registra_tempo_espera(Paciente* paciente, double tempo);
void registra_tempo_atendimento(Paciente* paciente, double tempo);
void libera_paciente(Paciente* paciente);
void exibe_paciente(const Paciente* paciente);
void paciente_entra_fila(Paciente* paciente, Fila* fila, double horario_atual);
Paciente* paciente_sai_fila(Fila* fila, double horario_atual);


// Funções para manipulação da fila
Fila* inicializa_fila();
void enfileira(Fila* fila, Paciente* paciente);
Paciente* desenfileira(Fila* fila);
int fila_vazia(Fila* fila);
void finaliza_fila(Fila* fila);
void exibe_fila(Fila* fila);

#endif