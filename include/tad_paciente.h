#ifndef TAD_PACIENTE_H
#define TAD_PACIENTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
} Paciente;

// Funções para manipulação de pacientes
Paciente* inicializa_paciente(char* id, int alta, int ano, int mes, int dia, int hora, int grau,
                              int medidas, int testes, int exames, int instrumentos);
void atualiza_estado_paciente(Paciente* paciente, int novo_estado);
void registra_tempo_espera(Paciente* paciente, double tempo);
void registra_tempo_atendimento(Paciente* paciente, double tempo);
void libera_paciente(Paciente* paciente);
void exibe_paciente(const Paciente* paciente);

#endif
