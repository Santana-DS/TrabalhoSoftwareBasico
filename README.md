Você tem toda a razão. A quebra na formatação é inaceitável e peço desculpas por isso.

O motivo da quebra é que eu usei formatação Markdown "avançada" que não é universalmente compatível:

1.  **A Estrutura do Projeto:** Usei caracteres especiais (`├──`, `└──`) para desenhar a árvore. Muitos renderizadores de Markdown (incluindo, aparentemente, o seu) não conseguem processá-los e quebram a formatação.
2.  **Os Blocos de Código:** Usei ` ```bash ... ``` ` para destacar a sintaxe. O identificador `bash` pode ter causado a quebra no fechamento do bloco.

### A Correção (Outro Approach)

Vamos usar o "approach" mais robusto e simples possível. Substituí:

1.  A árvore de estrutura por uma lista de marcadores (bullet points) aninhada.
2.  Todos os blocos ` ```bash ` por blocos de código simples ` ``` `.

Aqui estão os dois arquivos de documentação com a formatação corrigida e à prova de quebras.

-----

### 1\. Conteúdo Corrigido para `README.md`

````
# 🚀 Montador de Passagem Única & Cadeia de Ferramentas - Disciplina de Software Básico

## 📖 Visão Geral

Este projeto, desenvolvido para a disciplina de **Software Básico** da Universidade de Brasília (UnB), implementa uma cadeia de ferramentas completa para uma linguagem Assembly hipotética.

O projeto foi construído em duas fases principais:

1.  **Montador de Passagem Única (Requisito da Disciplina):** O núcleo do projeto é um montador que implementa o algoritmo de passagem única com lista de pendências, capaz de processar um único arquivo `.asm`, expandir macros e gerar os arquivos de saída `.pre`, `.o1` e `.o2`.

2.  **Cadeia de Ferramentas Estendida (Funcionalidades Adicionais):** O projeto foi expandido para uma cadeia de ferramentas completa, incluindo um ligador e um simulador, permitindo o desenvolvimento com múltiplos módulos.

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

* **codigo_fonte/**
    * `main.cpp` (Orquestrador do Montador)
    * `pre_processador.cpp`
    * `montador.cpp` (Lógica do Montador - Modos Absoluto e Relocável)
    * `ligador.cpp` (Lógica do Ligador)
    * `simulador.cpp` (Lógica do Carregador e Simulador)
    * `estruturas.h` (Structs de Símbolos, Pendências, Relocação)
    * `utilitarios.h` (Funções 'trim' e 'obter_nome_base')
* **executavel/**
    * `compilador.exe` (Montador/Pré-processador)
    * `ligador.exe`
    * `simulador.exe`
* **exemplos/**
    * `fibonacci.asm` (Exemplo de arquivo único)
    * `modulo_a.asm` (Exemplo de múltiplos módulos)
    * `modulo_b.asm`
    * `modulo_c.asm`
* `instrucoes.txt`
* `README.md`

-----

## ⚙️ Como Compilar

Siga este passo a passo para compilar os **três executáveis** do sistema.

**1. Pré-requisito:**
   - Ter o compilador G++ (MinGW-w64 no Windows ou `build-essential` no Linux) instalado.

**2. Compilando (PowerShell/Bash):**
   - Abra um terminal na **pasta raiz** do projeto.
   - Crie a pasta `executavel` se ela não existir.
   - Execute os três comandos de compilação separadamente:

    ```
    # 1. Compila o Montador (compilador.exe)
    g++ codigo_fonte/main.cpp codigo_fonte/pre_processador.cpp codigo_fonte/montador.cpp -o executavel/compilador.exe

    # 2. Compila o Ligador (ligador.exe)
    g++ codigo_fonte/ligador.cpp -o executavel/ligador.exe

    # 3. Compila o Simulador (simulador.exe)
    g++ codigo_fonte/simulador.cpp -o executavel/simulador.exe
    ```

-----

## 🚀 Guia de Uso

O sistema suporta dois fluxos de trabalho distintos.

### 1. Fluxo de Trabalho 1: Montador Absoluto (Requisito da Disciplina)

Este fluxo demonstra a funcionalidade original do montador de passagem única, processando um único arquivo `.asm` para gerar os arquivos `.o1` e `.o2`.

**1. Montar (Modo Absoluto):**
   Execute o `compilador.exe` sem o *flag* `-c`.

````

./executavel/compilador.exe \<caminho/para/seu\_arquivo.asm\>

# Exemplo:

./executavel/compilador.exe exemplos/fibonacci.asm

```

* **Saídas Geradas:**
 * `executavel/fibonacci.pre` (Após o pré-processamento)
 * `executavel/fibonacci.o1` (Código objeto intermediário e lista de pendências)
 * `executavel/fibonacci.o2` (Código objeto absoluto final, resolvido para o endereço 0)

*(Nota: O arquivo `.o2` é um formato simples e não é compatível com o `simulador.exe` atual, que espera o formato `.exe` relocável do ligador).*

---

### 2. Fluxo de Trabalho 2: Cadeia de Ferramentas Completa (Ligar e Simular)

Este é o fluxo avançado que usa toda a cadeia para compilar, ligar e simular múltiplos módulos que dependem uns dos outros.

**Etapa 1: Montar Módulos (Modo Relocável)**
Use o *flag* `-c` para compilar cada módulo `.asm` em um arquivo `.obj`.

```

./executavel/compilador.exe -c \<caminho/para/modulo1.asm\>
./executavel/compilador.exe -c \<caminho/para/modulo2.asm\>

# ...

# Exemplo:

./executavel/compilador.exe exemplos/modulo\_a.asm
./executavel/compilador.exe exemplos/modulo\_b.asm
./executavel/compilador.exe exemplos/modulo\_c.asm

```
* **Saídas Geradas:** `executavel/modulo_a.obj`, `executavel/modulo_b.obj`, etc.

**Etapa 2: Ligar Módulos**
Use o `ligador.exe` para combinar os arquivos `.obj` em um executável final (`.exe`) e gerar o arquivo de mapa (`.map`).

```

./executavel/ligador.exe -o \<saida.exe\> -m \<saida.map\> \<modulo1.obj\> \<modulo2.obj\> ...

# Exemplo:

./executavel/ligador.exe -o executavel/area.exe -m executavel/area.map executavel/modulo\_a.obj executavel/modulo\_b.obj executavel/modulo\_c.obj

```
* **Saídas Geradas:** `executavel/area.exe` (o programa final) e `executavel/area.map` (o log de ligação).

**Etapa 3: Simular o Executável Final**
Use o `simulador.exe` para carregar e rodar o programa ligado. Você pode especificar um endereço de carregamento (offset).

```

./executavel/simulador.exe \<caminho/para/arquivo.exe\> [endereco\_base]

# Exemplo (Carregando em 8097):

./executavel/simulador.exe executavel/area.exe 8097

# Exemplo (Carregando em 211028097):

./executavel/simulador.exe executavel/area.exe 211028097

```
* **O que acontece:** O programa pedirá os `INPUT`s no console.
* **Saída Gerada:** `area.log` (contendo a desmontagem da memória e o log de execução, como `OUTPUT (do endereco 211028191): 50`).

**Etapa 4: Depuração (Opcional)**
Use os *flags* `-d` ou `-dump` para obter um log mais detalhado.

```

# Gera um log com o trace de cada instrução

./executavel/simulador.exe -d executavel/area.exe 8097

# Apenas desmonta a memória para o log (não executa)

./executavel/simulador.exe -dump executavel/area.exe 8097

```
```

-----

### 2\. Conteúdo Corrigido para `instrucoes.txt`

```
============================================================
Trabalho 1 de Software Basico - Montador, Ligador e Simulador
============================================================
https://github.com/Santana-DS/TrabalhoSoftwareBasico
============================================================

Grupo:
- Lucas Santana da Silva - 211028097
- Gabriel Francisco de Oliveira - 202066571
- Guilherme Miranda de Matos - 221006431

------------------------------------------------------------
Passo a Passo para Compilar e Executar
------------------------------------------------------------

1. PRÉ-REQUISITO:
   - Ter o compilador G++ instalado e funcionando no terminal.
     (Ex: MinGW-w64 no Windows, pacote 'build-essential' no Linux).

2. COMPILANDO A CADEIA DE FERRAMENTAS:
   - Abra um terminal na pasta raiz do projeto.
   - Se não existir, crie a pasta "executavel".
   - Execute os TRÊS comandos abaixo, um de cada vez:

     # 1. Compila o Montador (compilador.exe)
     g++ codigo_fonte/main.cpp codigo_fonte/pre_processador.cpp codigo_fonte/montador.cpp -o executavel/compilador.exe

     # 2. Compila o Ligador (ligador.exe)
     g++ codigo_fonte/ligador.cpp -o executavel/ligador.exe

     # 3. Compila o Simulador (simulador.exe)
     g++ codigo_fonte/simulador.cpp -o executavel/simulador.exe

------------------------------------------------------------
COMO USAR: FLUXO 1 (Requisito da Disciplina - Arquivo Único)
------------------------------------------------------------
Este fluxo gera os arquivos .o1 e .o2 (não executáveis).

   # Etapa A: Montar o arquivo .asm (sem -c)
   ./executavel/compilador.exe exemplos/fibonacci.asm
   
   # Saídas: executavel/fibonacci.pre, .o1, .o2

------------------------------------------------------------
COMO USAR: FLUXO 2 (Cadeia Completa - Múltimos Módulos)
------------------------------------------------------------
Este fluxo gera um programa executável a partir de vários módulos.

   # Etapa A: Montar todos os módulos (com o flag -c)
   ./executavel/compilador.exe -c exemplos/modulo_a.asm
   ./executavel/compilador.exe -c exemplos/modulo_b.asm
   ./executavel/compilador.exe -c exemplos/modulo_c.asm
   
   # Saídas: executavel/modulo_a.obj, .pre (etc.)

   # Etapa B: Ligar os módulos em um executável
   ./executavel/ligador.exe -o executavel/area.exe -m executavel/area.map executavel/modulo_a.obj executavel/modulo_b.obj executavel/modulo_c.obj
   
   # Saídas: executavel/area.exe (Executável)
   #         executavel/area.map (Log de Ligação)

   # Etapa C: Simular o executável final
   # (O endereço 8097 é opcional)
   ./executavel/simulador.exe executavel/area.exe 8097
   
   # O que acontece: O programa irá pedir os INPUTS no console.
   
   # Saída: area.log (Log de Desmontagem e Execução)

------------------------------------------------------------
COMO USAR: AJUDA (Menus -h)
------------------------------------------------------------
Para ver as opções de qualquer ferramenta, use -h ou --help.

   ./executavel/compilador.exe -h
   ./executavel/ligador.exe -h
   ./executavel/simulador.exe -h

------------------------------------------------------------
Nota Importante para Usuários Linux:
------------------------------------------------------------
O sistema de arquivos do Linux diferencia maiúsculas de minúsculas.
Este projeto utiliza nomes de arquivos e #includes em minúsculas (ex: "montador.h").
Certifique-se de que os nomes dos arquivos não foram alterados.
```