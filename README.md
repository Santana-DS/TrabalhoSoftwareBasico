-----

# 🚀 Montador de Passagem Única - Disciplina de Software Básico

## 📖 Visão Geral

Este projeto é a implementação de um **Montador** (Assembler) de passagem única para uma linguagem Assembly hipotética, desenvolvido como requisito para a disciplina de **Software Básico** da Universidade de Brasília (UnB). O programa simula as duas primeiras fases de um sistema de processamento de linguagens: o **Pré-processamento** (com foco em macros) e a **Montagem** (tradução de Assembly para "código de máquina").

O objetivo principal foi solidificar os conceitos fundamentais sobre a arquitetura de computadores, a estrutura de linguagens de baixo nível e os algoritmos que permitem a transformação de código legível por humanos em código executável por uma máquina.

## ✨ Funcionalidades Principais

  * **Pré-processador Robusto:**
      * Suporte para definição de até duas macros por arquivo.
      * Expansão de macros com até dois argumentos.
      * **Resolução de Macros Aninhadas:** Capacidade de uma macro chamar outra, com o pré-processador resolvendo as expansões em múltiplos passos.
  * **Montador de Passagem Única:**
      * Implementação do algoritmo clássico de *single-pass* com **Lista de Pendências** para resolver referências a rótulos futuros (*forward references*).
      * Suporte para diretivas como `SPACE` (reserva de memória) e `CONST` (definição de constantes).
  * **Detecção Abrangente de Erros:**
      * **Erros Léxicos:** Validação de nomes de rótulos (não podem começar com números, caracteres especiais limitados).
      * **Erros Sintáticos:** Verificação de número de operandos, instruções inexistentes e múltiplos rótulos por linha.
      * **Erros Semânticos:** Detecção de rótulos duplicados e uso de rótulos não declarados.
  * **Flexibilidade e Tolerância:**
      * O parser é *case-insensitive* (entende `LOAD`, `load` e `LoAd` da mesma forma).
      * Ignora espaços, tabulações e quebras de linha desnecessárias.

-----

## 🧠 A Jornada Conceitual: O Que Aprendemos?

Este projeto foi uma imersão profunda nos mecanismos que operam "sob o capô" da computação. Aqui estão os principais conceitos que exploramos e implementamos.

### 1\. Compiladores vs. Montadores

Embora o trabalho use o termo "compilador", o que construímos é, tecnicamente, um **Montador** (Assembler). A diferença é crucial:

  * Um **Compilador** traduz uma linguagem de alto nível (como C++) para uma de baixo nível (Assembly).
  * Um **Montador** traduz uma linguagem Assembly (com mnemônicos como `ADD`, `MULT`) para código de máquina (números, como `1`, `3`).
    Nosso projeto faz exatamente o segundo.

### 2\. Fase 1: O Pré-Processador e a Magia das Macros

Macros são ferramentas poderosas para automação e reutilização de código. Aprendemos que o pré-processamento é, em essência, uma **transformação textual inteligente**.

  * **O Desafio das Macros Aninhadas:** O requisito de uma macro poder chamar outra nos forçou a ir além de uma simples substituição. Uma única passagem de expansão não seria suficiente. A solução foi implementar um **loop de expansão iterativo**: o pré-processador reavalia o código gerado continuamente, expandindo macros camada por camada, até que nenhuma chamada de macro reste no código.

### 3\. Fase 2: O Coração do Projeto - O Montador de Passagem Única

O maior desafio de um montador de passagem única é responder à pergunta: "Como saber o endereço de um rótulo que ainda não foi lido?".

  * **A Lista de Pendências:** A solução para este problema é a elegante **Lista de Pendências**. Funciona como uma lista de "promessas" ou "tarefas a fazer".

    1.  Ao encontrar uma instrução que usa um rótulo futuro (ex: `JMP LOOP_FUTURO`), o montador não entra em pânico.
    2.  Ele gera o código de máquina com um espaço reservado (um `0`) no lugar do endereço.
    3.  Ele anota em sua Lista de Pendências: "Quando eu descobrir o endereço de `LOOP_FUTURO`, preciso voltar e preenchê-lo na posição X do código".
    4.  Ao final da leitura do arquivo, ele percorre essa lista e cumpre todas as promessas, preenchendo as lacunas.

  * **As Tabelas Fundamentais:** Para se organizar, o montador depende de estruturas de dados essenciais:

      * **Tabela de Instruções (TI):** Nosso "dicionário" da linguagem. Mapeia cada mnemônico (`ADD`, `LOAD`) para seu opcode, tamanho e número de operandos.
      * **Tabela de Símbolos (TS):** Nosso "catálogo de endereços". Armazena cada rótulo definido no código e o endereço de memória correspondente.

-----

## 📂 Estrutura do Projeto

```
.
├── codigo_fonte/
│   ├── main.cpp
│   ├── estruturas.h
│   ├── utilitarios.h
│   ├── pre_processador.h
│   ├── pre_processador.cpp
│   ├── montador.h
│   └── montador.cpp
│
├── executavel/
│   └── (Local do compilador.exe gerado)
│
├── exemplos/
│   ├── teste_completo.asm
│   ├── teste_sintaxe_flexivel.asm
│   ├── fibonacci.asm
│   ├── area_triangulo.asm
│   └── (outros testes .asm)
│
└── instrucoes.txt
```

-----

## ⚙️ Como Compilar e Executar

Siga este passo a passo para compilar e rodar o programa.

**1. Pré-requisito:**

  - Ter o compilador G++ (MinGW-w64 no Windows ou `build-essential` no Linux) instalado e acessível no terminal.

**2. Compilando:**

  - Abra um terminal na **pasta raiz** do projeto.
  - Crie a pasta `executavel` se ela não existir.
  - Use o seguinte comando, que compila todos os arquivos `.cpp` de uma só vez:
    ```bash
    g++ codigo_fonte/*.cpp -o executavel/compilador
    ```

**3. Executando (Exemplo Prático):**

  - Para montar o arquivo `teste_completo.asm`, use o comando abaixo (ainda na pasta raiz):
    ```bash
    ./executavel/compilador.exe exemplos/teste_completo.asm
    ```

**4. Verificando as Saídas:**

  - Após a execução, os três arquivos de saída serão criados na **pasta raiz** do projeto, conforme a regra de execução:
      - `teste_completo.pre`
      - `teste_completo.o1`
      - `teste_completo.o2`

-----

## 📜 Arquivos de Saída

O programa gera três arquivos, cada um representando uma etapa do processo:

  * **`arquivo.pre`:** O código-fonte após a fase de pré-processamento. Todas as macros foram expandidas, resultando em um código Assembly "puro".
  * **`arquivo.o1`:** A saída intermediária da passagem única. Contém o código objeto com zeros nos locais de pendências e a lista de pendências em si, mostrando quais símbolos precisam ser resolvidos e em quais endereços.
  * **`arquivo.o2`:** O código objeto final. Todas as pendências foram resolvidas, e o arquivo contém a sequência de números pronta para ser "carregada" e "executada" por uma máquina hipotética.

-----

## 👨‍💻 Autores

  * **Lucas Santana da Silva** - 211028097
  * **Gabriel Francisco de Oliveira** - 202066571
  * **Guilherme Miranda de Matos** - 221006431