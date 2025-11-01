Você tem razão, a formatação do Markdown pode ser instável no console. Peço desculpas por isso.

Aqui está o **texto puro** para o `README.md`. Por favor, copie e cole este conteúdo diretamente no seu arquivo, substituindo tudo o que estava lá. A formatação simples com indentação deve funcionar perfeitamente.

--- (Início do README.md) ---

# 🚀 Montador de Passagem Única & Cadeia de Ferramentas - Disciplina de Software Básico

## 📖 Visão Geral

Este projeto é a implementação de uma cadeia de ferramentas de software básico para uma linguagem Assembly hipotética, desenvolvido como requisito para a disciplina de **Software Básico** da Universidade de Brasília (UnB).

O sistema evoluiu de um simples montador para uma cadeia completa, consistindo em:

1.  **Pré-Processador:** Expande macros, incluindo chamadas aninhadas e com parâmetros.
2.  **Montador (`compilador.exe`):** Um montador de passagem única que opera em dois modos:
    * **Modo Absoluto:** Gera código objeto absoluto (`.o1`, `.o2`) para um único arquivo.
    * **Modo Relocável (`-c`):** Gera código objeto relocável (`.obj`) para múltiplos módulos, suportando `PUBLIC` e `EXTERN`.
3.  **Ligador (`ligador.exe`):** Um ligador de duas passagens que combina múltiplos arquivos `.obj`, resolve referências externas e gera um único executável (`.exe`) e um arquivo de mapa (`.map`).
4.  **Simulador (`simulador.exe`):** Um simulador de CPU que carrega o executável final na memória (com suporte a offset) e o executa, salvando o log de saída em um arquivo (`.log`).

## ✨ Funcionalidades Principais

* **Pré-processador Robusto:** Suporte para definição e expansão de macros aninhadas.
* **Montador de Modo Duplo:**
    * **Modo Absoluto:** Gera arquivos `.o1` e `.o2` conforme os requisitos originais.
    * **Modo Relocável:** Entende `PUBLIC` e `EXTERN` e gera Tabelas de Uso, Definição e Relocação.
* **Ligador de Duas Passagens:**
    * Constrói uma Tabela Global de Símbolos.
    * Resolve referências externas entre módulos usando as Tabelas de Uso e Definição.
    * Gera um executável final e um `.map` com o log detalhado da ligação.
* **Simulador de CPU:**
    * Carrega o programa em um endereço de memória (padrão `0` ou um offset definido pelo usuário).
    * Simula uma CPU com Acumulador (`ACC`) e Contador de Programa (`PC`).
    * Executa o código e salva todos os `OUTPUT`s e erros em um arquivo `.log`.
* **Detecção Abrangente de Erros:** O sistema detecta erros léxicos, sintáticos (ex: `OPCODE` inexistente), semânticos (ex: símbolo duplicado) e de ligação (ex: símbolo externo não resolvido).

-----

## 📂 Estrutura do Projeto

.
├── codigo_fonte/
│   ├── main.cpp            (Orquestrador do Montador)
│   ├── pre_processador.cpp
│   ├── montador.cpp        (Lógica do Montador - Modos Absoluto e Relocável)
│   ├── ligador.cpp         (Lógica do Ligador)
│   ├── simulador.cpp       (Lógica do Carregador e Simulador)
│   ├── estruturas.h        (Structs de Símbolos, Pendências, Relocação)
│   └── utilitarios.h       (Funções 'trim' e 'obter_nome_base')
│
├── executavel/
│   ├── compilador.exe      (Montador/Pré-processador)
│   ├── ligador.exe
│   └── simulador.exe
│
├── exemplos/
│   ├── fibonacci.asm       (Exemplo de arquivo único)
│   ├── modulo_a.asm        (Exemplo de múltiplos módulos)
│   ├── modulo_b.asm
│   └── modulo_c.asm
│
└── README.md

-----

## ⚙️ Como Compilar

Siga este passo a passo para compilar os **três executáveis** do sistema.

**1. Pré-requisito:**
   - Ter o compilador G++ (MinGW-w64 no Windows ou `build-essential` no Linux) instalado.

**2. Compilando (PowerShell/Bash):**
   - Abra um terminal na **pasta raiz** do projeto.
   - Crie a pasta `executavel` se ela não existir.
   - Execute os três comandos de compilação separadamente:

     # Compila o Montador
     g++ codigo_fonte/main.cpp codigo_fonte/pre_processador.cpp codigo_fonte/montador.cpp -o executavel/compilador.exe

     # Compila o Ligador
     g++ codigo_fonte/ligador.cpp -o executavel/ligador.exe

     # Compila o Simulador
     g++ codigo_fonte/simulador.cpp -o executavel/simulador.exe

-----

## 🚀 Guia de Uso

Existem dois fluxos de trabalho principais:

### 1. Fluxo de Trabalho: Modo Absoluto (Arquivo Único)

Este fluxo usa apenas o `compilador.exe` e o `simulador.exe` para montar e rodar um único arquivo `.asm` (cumprindo os requisitos originais da disciplina).

**1. Montar (Modo Absoluto):**
   ./executavel/compilador.exe exemplos/fibonacci.asm
   
   * **Saída:** `executavel/fibonacci.pre`, `executavel/fibonacci.o1`, `executavel/fibonacci.o2`.

**2. Executar:**
   ./executavel/simulador.exe executavel/fibonacci.o2
   
   * **Saída:** `fibonacci.log` (contendo os `OUTPUT`s do programa).

---

### 2. Fluxo de Trabalho: Cadeia Completa (Múltiplos Módulos)

Este é o fluxo avançado que usa toda a cadeia de ferramentas.

**1. Montar Módulos (Modo Relocável):**
   Use o flag `-c` para compilar cada módulo `.asm` em um arquivo `.obj`.
   
   ./executavel/compilador.exe -c exemplos/modulo_a.asm
   ./executavel/compilador.exe -c exemplos/modulo_b.asm
   ./executavel/compilador.exe -c exemplos/modulo_c.asm
   
   * **Saída:** `executavel/modulo_a.obj`, `executavel/modulo_b.obj`, `executavel/modulo_c.obj`.

**2. Ligar Módulos:**
   Use o `ligador.exe` para combinar os arquivos `.obj` em um executável final (`.exe`) e gerar o arquivo de mapa (`.map`).
   
   ./executavel/ligador.exe -o executavel/area.exe -m executavel/area.map executavel/modulo_a.obj executavel/modulo_b.obj executavel/modulo_c.obj
   
   * **Saída:** `executavel/area.exe` (o programa final) e `executavel/area.map` (o log de ligação).

**3. Simular o Executável Final:**
   Use o `simulador.exe` para carregar e rodar o programa ligado.
   
   ./executavel/simulador.exe executavel/area.exe
   
   * **O que acontece:** O programa pedirá os 6 `INPUT`s.
   * **Saída:** `area.log` (contendo os `OUTPUT`s ou erros de execução).

**4. (Opcional) Simular com Offset:**
   Para carregar o programa no endereço de memória `2000`:
   
   ./executavel/simulador.exe executavel/area.exe 2000

--- (Fim do README.md) ---

Isso conclui a **Sugestão 2**.

Agora, podemos implementar a última melhoria de "clareza" que discutimos: adicionar os menus de ajuda (`-h` ou `--help`) aos três executáveis?