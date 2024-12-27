# ufmg_dcc205_tp_02 - Sistema de Escalonamento Hospitalar

## Descrição
Este projeto implementa um **Sistema de Escalonamento Hospitalar** baseado na simulação de eventos discretos (SED). O objetivo é simular o fluxo de atendimento no Hospital Zambs (HZ), desde a admissão dos pacientes até a alta hospitalar, otimizando o uso de recursos e avaliando o custo-benefício dos procedimentos.

Os pacientes são classificados em três graus de urgência (verde, amarelo e vermelho) e passam por diferentes estágios de atendimento, incluindo triagem, atendimento, medidas hospitalares, testes laboratoriais, exames de imagem e aplicação de instrumentos/medicamentos.

## Componentes do Sistema
- **Simulação de eventos discretos**: Implementação baseada no "método das três fases".
- **Tipos Abstratos de Dados (TADs)**:
  - **Paciente**: Representa os estados e estatísticas de atendimento de um paciente.
  - **Procedimento**: Modela os serviços disponíveis e sua utilização.
  - **Fila**: Controla os pacientes aguardando em cada procedimento.
  - **Escalonador**: Gerencia eventos utilizando uma fila de prioridade baseada em um `minheap`.

## Arquitetura
O sistema organiza pacientes em filas distintas por grau de urgência e escalona os eventos considerando as prioridades. Os tempos de espera e atendimento são utilizados para calcular estatísticas de desempenho.

## Entrada de Dados
O sistema recebe um arquivo de entrada com:
1. Configuração do hospital (número de unidades e tempos médios de execução por procedimento).
2. Lista de pacientes e suas características (incluindo identificação, grau de urgência e procedimentos a realizar).

## Saída do Programa
Gera um relatório com:
- Tempo total de permanência de cada paciente no hospital.
- Tempos de espera e atendimento por paciente.
- Estatísticas gerais de utilização dos recursos.

## Pontos Extras
O projeto permite otimizações, como:
- **Fragmentação de procedimentos** para reduzir contenção.
- **Mudança na ordem de execução** com base na demanda para melhorar a eficiência.

## Entrega
- **Código**: Deve ser implementado em C ou C++ (restrições específicas se aplicam) e organizado em uma estrutura com Makefile.
- **Documentação**: Arquivo PDF detalhando o método, análise de complexidade, experimentos e resultados.

### Observação
A implementação e instruções de instalação devem ser preenchidas posteriormente.

## FAQ
Consulte o documento oficial para dúvidas frequentes sobre o trabalho.

---
**Nota:** Este projeto deve ser compilado e executado em um ambiente Linux compatível com C++11.
