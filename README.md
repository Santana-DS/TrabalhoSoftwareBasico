# 🚀 Montador de Passagem Única & Cadeia de Ferramentas - Disciplina de Software Básico

## 📖 Visão Geral

Este projeto, desenvolvido para a disciplina de **Software Básico** da Universidade de Brasília (UnB), implementa uma cadeia de ferramentas completa para uma linguagem Assembly hipotética.

O projeto foi construído em duas fases principais:

1. **Montador de Passagem Única (Requisito da Disciplina):** O núcleo do projeto é um montador que implementa o algoritmo de passagem única com lista de pendências, capaz de processar um único arquivo `.asm`, expandir macros e gerar os arquivos de saída `.pre`, `.o1` e `.o2`.

2. **Cadeia de Ferramentas Estendida (Funcionalidades Adicionais):** O projeto foi expandido para uma cadeia de ferramentas completa, incluindo um ligador e um simulador, permitindo o desenvolvimento com múltiplos módulos.

## ✨ Funcionalidades

### 📜 Funcionalidades Originais (Requisitos da Disciplina)

* **Pré-processador de Macros:** Suporte completo para definição e expansão de macros com argumentos, incluindo macros aninhadas.
* **Montador de Passagem Única:** Implementa o algoritmo clássico de *single-pass* com Lista de Pendências para resolver referências a rótulos futuros (*forward references*) dentro de um único arquivo.
* **Geração de Saídas (`.o1`, `.o2`):** Gera o arquivo `.o1` com o código objeto intermediário e a lista de pendências, e o `.o2` com o código objeto absoluto resolvido.

### ✨ Funcionalidades Adicionais (Cadeia de Ferramentas Completa)

* **Montador de Modo Duplo (`compilador.exe`):**
  * Opera em **Modo Absoluto** (padrão) para os requisitos da disciplina.
  * Opera em **Modo Relocável** (com *flag* `-c`) para gerar arquivos `.obj` para o ligador.
  * Suporte às diretivas `PUBLIC` e `EXTERN` para referências cruzadas entre módulos.
  * Gera Tabelas de Uso, Definições e Relocação (Interna e Externa) nos arquivos `.obj`.

* **Ligador (`ligador.exe`):**
  * Implementa um **ligador de duas passagens** que combina múltiplos arquivos `.obj`.
  * Constrói uma Tabela Global de Símbolos para resolver referências externas.
  * Gera um **Arquivo de Mapa (`.map`)** detalhado com o log completo da ligação.
  * Gera um arquivo executável relocável (`.exe`) final.

* **Simulador de CPU (`simulador.exe`):**
  * **Carregador Relocável:** Carrega o `.exe` final e aplica o "fator de correção" (endereço base) a todos os operandos marcados, permitindo que o programa seja carregado em *qualquer* endereço de memória.
  * **Memória Esparsa:** Utiliza `std::map` para simular a memória, permitindo endereços muito grandes (ex: `211028097`) sem causar estouro de memória.
  * **Execução de CPU:** Simula uma CPU com Acumulador (`ACC`) e Contador de Programa (`PC`) para executar o código carregado.
  * **Log de Saída:** Salva *todo* o resultado da execução (desmontagem da memória, `OUTPUT`s e erros) em um arquivo `.log`.
  * **Modos de Depuração:** Suporta os *flags* `-d` (trace de execução) e `-dump` (apenas desmontagem).

* **Interface de Usuário:**
  * Todos os três executáveis (`compilador.exe`, `ligador.exe`, `simulador.exe`) possuem menus de ajuda (`-h` ou `--help`).

-----

## 📂 Estrutura do Projeto

```
codigo_fonte/
│
├── main.cpp                 # Orquestrador do Montador
├── pre_processador.cpp
├── montador.cpp             # Lógica do Montador - Modos Absoluto e Relocável
├── ligador.cpp              # Lógica do Ligador
├── simulador.cpp            # Lógica do Carregador e Simulador
├── estruturas.h             # Structs de Símbolos, Pendências, Relocação
└── utilitarios.h            # Funções auxiliares ('trim', 'obter_nome_base')

executavel/
├── compilador.exe
├── ligador.exe
└── simulador.exe

exemplos/
├── fibonacci.asm
├── modulo_a.asm
├── modulo_b.asm
└── modulo_c.asm

instrucoes.txt
README.md
```

-----

## ⚙️ Como Compilar

Siga este passo a passo para compilar os **três executáveis** do sistema.

### 1. Pré-requisito

- Ter o compilador G++ (MinGW-w64 no Windows ou `build-essential` no Linux) instalado.

### 2. Compilando (PowerShell/Bash)

#### 🧱 1. Compila o Montador (`compilador.exe`)
```bash
g++ codigo_fonte/main.cpp codigo_fonte/pre_processador.cpp codigo_fonte/montador.cpp -o executavel/compilador.exe
```

#### 🔗 2. Compila o Ligador (`ligador.exe`)
```bash
g++ codigo_fonte/ligador.cpp -o executavel/ligador.exe
```

#### ⚙️ 3. Compila o Simulador (`simulador.exe`)
```bash
g++ codigo_fonte/simulador.cpp -o executavel/simulador.exe
```

-----

## 🚀 Guia de Uso

O sistema suporta dois fluxos de trabalho distintos.

### 1. Fluxo de Trabalho 1: Montador Absoluto (Requisito da Disciplina)

Processa um único arquivo `.asm` e gera os arquivos `.o1` e `.o2`.

```bash
./executavel/compilador.exe exemplos/fibonacci.asm
```

**Saídas Geradas:**
- `executavel/fibonacci.pre`
- `executavel/fibonacci.o1`
- `executavel/fibonacci.o2`

> ⚠️ O arquivo `.o2` é apenas para o modo absoluto e não é compatível com o `simulador.exe`.

-----

### 2. Fluxo de Trabalho 2: Cadeia de Ferramentas Completa

#### Etapa 1: Montar Módulos (Modo Relocável)

```bash
./executavel/compilador.exe -c exemplos/modulo_a.asm
./executavel/compilador.exe -c exemplos/modulo_b.asm
./executavel/compilador.exe -c exemplos/modulo_c.asm
```

**Saídas Geradas:** `executavel/modulo_a.obj`, `executavel/modulo_b.obj`, `executavel/modulo_c.obj`

#### Etapa 2: Ligar Módulos

```bash
./executavel/ligador.exe -o executavel/area.exe -m executavel/area.map executavel/modulo_a.obj executavel/modulo_b.obj executavel/modulo_c.obj
```

**Saídas Geradas:** `executavel/area.exe`, `executavel/area.map`

#### Etapa 3: Simular o Executável Final

```bash
./executavel/simulador.exe executavel/area.exe 8097
# ou
./executavel/simulador.exe executavel/area.exe 211028097
```

**Saída Gerada:** `area.log` (com desmontagem e execução).

#### Etapa 4: Depuração (Opcional)

```bash
# Gera trace detalhado
./executavel/simulador.exe -d executavel/area.exe 8097

# Apenas desmonta a memória
./executavel/simulador.exe -dump executavel/area.exe 8097
```

