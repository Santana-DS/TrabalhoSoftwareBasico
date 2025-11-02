# 🚀 Montador de Passagem Única & Cadeia de Ferramentas - Disciplina de Software Básico

## 📖 Visão Geral

Este projeto, desenvolvido para a disciplina de **Software Básico** da Universidade de Brasília (UnB), implementa uma cadeia de ferramentas completa para uma linguagem Assembly hipotética.

O projeto foi construído em duas fases principais:

1. **Montador de Passagem Única (Requisito da Disciplina):** O núcleo do projeto é um montador que implementa o algoritmo de passagem única com lista de pendências, capaz de processar um único arquivo `.asm`, expandir macros e gerar os arquivos de saída `.pre`, `.o1` e `.o2`.

2. **Cadeia de Ferramentas Estendida (Funcionalidades Adicionais):** O projeto foi expandido para uma cadeia de ferramentas completa, incluindo um ligador e um simulador, permitindo o desenvolvimento com múltiplos módulos.

---

## ✨ Funcionalidades

### 📜 Funcionalidades Originais (Requisitos da Disciplina)

- **Pré-processador de Macros:** Suporte completo para definição e expansão de macros com argumentos, incluindo macros aninhadas.
- **Montador de Passagem Única:** Implementa o algoritmo clássico de *single-pass* com Lista de Pendências para resolver referências a rótulos futuros (*forward references*) dentro de um único arquivo.
- **Geração de Saídas (`.o1`, `.o2`):** Gera o arquivo `.o1` com o código objeto intermediário e a lista de pendências, e o `.o2` com o código objeto absoluto resolvido.

### ✨ Funcionalidades Adicionais (Cadeia de Ferramentas Completa)

- **Montador de Modo Duplo (`compilador.exe`):**
  - Opera em **Modo Absoluto** (padrão) para os requisitos da disciplina.
  - Opera em **Modo Relocável** (com flag `-c`) para gerar arquivos `.obj` para o ligador.
  - Suporte às diretivas `PUBLIC` e `EXTERN` para referências cruzadas entre módulos.
  - Gera Tabelas de Uso, Definições e Relocação (Interna e Externa) nos arquivos `.obj`.

- **Ligador (`ligador.exe`):**
  - Implementa um **ligador de duas passagens** que combina múltiplos arquivos `.obj`.
  - Constrói uma Tabela Global de Símbolos para resolver referências externas.
  - Gera um **Arquivo de Mapa (`.map`)** detalhado com o log completo da ligação.
  - Gera um arquivo executável relocável (`.exe`) final.

- **Simulador de CPU (`simulador.exe`):**
  - **Carregador Relocável:** Carrega o `.exe` final e aplica o "fator de correção" (endereço base) a todos os operandos marcados, permitindo que o programa seja carregado em *qualquer* endereço de memória.
  - **Memória Esparsa:** Utiliza `std::map` para simular a memória, permitindo endereços muito grandes (ex: `211028097`) sem causar estouro de memória.
  - **Execução de CPU:** Simula uma CPU com Acumulador (`ACC`) e Contador de Programa (`PC`) para executar o código carregado.
  - **Log de Saída:** Salva *todo* o resultado da execução (desmontagem da memória, `OUTPUT`s e erros) em um arquivo `.log`.
  - **Modos de Depuração:** Suporta os flags `-d` (trace de execução) e `-dump` (apenas desmontagem).

- **Interface de Usuário:**
  - Todos os três executáveis (`compilador.exe`, `ligador.exe`, `simulador.exe`) possuem menus de ajuda (`-h` ou `--help`).

---

## 📂 Estrutura do Projeto

- **codigo_fonte/**
  - `main.cpp` (Orquestrador do Montador)
  - `pre_processador.cpp`
  - `montador.cpp` (Lógica do Montador - Modos Absoluto e Relocável)
  - `ligador.cpp` (Lógica do Ligador)
  - `simulador.cpp` (Lógica do Carregador e Simulador)
  - `estruturas.h` (Structs de Símbolos, Pendências, Relocação)
  - `utilitarios.h` (Funções utilitárias como 'trim' e 'obter_nome_base')
- **executavel/**
  - `compilador.exe` (Montador/Pré-processador)
  - `ligador.exe`
  - `simulador.exe`
- **exemplos/**
  - `fibonacci.asm` (Exemplo de arquivo único)
  - `modulo_a.asm`
  - `modulo_b.asm`
  - `modulo_c.asm`
- `instrucoes.txt`
- `README.md`

---

## ⚙️ Como Compilar

### 1. Pré-requisito

- Ter o compilador **G++** (MinGW-w64 no Windows ou `build-essential` no Linux) instalado.

### 2. Compilando (PowerShell/Bash)

Abra um terminal na **pasta raiz** do projeto.  
Crie a pasta `executavel` se ela não existir.  
Execute os comandos abaixo:

#### 🧱 Compila o Montador (`compilador.exe`)
```bash
g++ codigo_fonte/main.cpp codigo_fonte/pre_processador.cpp codigo_fonte/montador.cpp -o executavel/compilador.exe
```

#### 🔗 Compila o Ligador (`ligador.exe`)
```bash
g++ codigo_fonte/ligador.cpp -o executavel/ligador.exe
```

#### ⚙️ Compila o Simulador (`simulador.exe`)
```bash
g++ codigo_fonte/simulador.cpp -o executavel/simulador.exe
```

---

## 🚀 Guia de Uso

O sistema suporta dois fluxos de trabalho distintos.  
Para ver a ajuda de qualquer comando, use `-h` ou `--help`.

### 🧩 Fluxo 1: Montador Absoluto (Requisito da Disciplina)

Processa um único arquivo `.asm` para gerar `.o1` e `.o2`.
Supondo que "arquivo.asm" esteja na pasta "/executavel".

**Sintaxe:**
```bash
./executavel/compilador.exe ./executavel/[arquivo.asm]
```

**Exemplo:**
```bash
./executavel/compilador.exe exemplos/fibonacci.asm
```

**Saídas Geradas:**
- `executavel/fibonacci.pre` — Após o pré-processamento  
- `executavel/fibonacci.o1` — Código objeto intermediário e lista de pendências  
- `executavel/fibonacci.o2` — Código absoluto final, resolvido para o endereço 0

> ⚠️ O arquivo `.o2` é um formato simples e **não é compatível** com o `simulador.exe` atual, que espera o formato `.exe` relocável.

---

### 🧩 Fluxo 2: Cadeia Completa (Montar → Ligar → Simular)

#### Etapa 1: Montar Módulos (Modo Relocável)

Use o flag `-c` para gerar `.obj` relocáveis, habilitando `PUBLIC` e `EXTERN`.

**Sintaxe:**
```bash
./executavel/compilador.exe -c [modulo1.asm]
./executavel/compilador.exe -c [modulo2.asm]
```
**Exemplo:**
```bash
./executavel/compilador.exe -c exemplos/modulo_a.asm
./executavel/compilador.exe -c exemplos/modulo_b.asm
./executavel/compilador.exe -c exemplos/modulo_c.asm
```

#### Etapa 2: Ligar Módulos

Flags:
- `-o [arquivo.exe]` → nome do executável final  
- `-m [arquivo.map]` → nome do log de ligação

**Sintaxe:**
```bash
./executavel/ligador.exe -o [saida.exe] -m [saida.map] [modulo1.obj] [modulo2.obj] ...
```
**Exemplo:**
```bash
./executavel/ligador.exe -o executavel/area.exe -m executavel/area.map executavel/modulo_a.obj executavel/modulo_b.obj executavel/modulo_c.obj
```

**Saídas Geradas:**
- `executavel/area.exe` — Programa final  
- `executavel/area.map` — Log detalhado de ligação

#### Etapa 3: Simular o Executável Final

O `simulador.exe` carrega o programa e aplica o endereço base.

**Sintaxe:**
```bash
./executavel/simulador.exe [arquivo.exe] [endereco_base_opcional]
```
**Exemplo (carregando no endereço 211028097 (nº de matrícula)):**
```bash
./executavel/simulador.exe executavel/area.exe 211028097
```

**Saída Gerada:**  
`area.log` — **código máquina final ligado e carregado**, desmontagem da memória, log da execução.

#### Etapa 4: Depuração (Opcional)

Flags:
- `-d` → log detalhado (trace)
- `-dump` → apenas desmonta a memória

**Sintaxe:**
```bash
# Log detalhado da execução
./executavel/simulador.exe -d [arquivo.exe] [endereco_base]

# Apenas desmonta a memória
./executavel/simulador.exe -dump [arquivo.exe] [endereco_base]
```

**Exemplo:**
```bash
./executavel/simulador.exe -d executavel/area.exe 21028097
```

