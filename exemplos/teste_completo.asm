; --- TESTE COMPLETO ---
; Um programa que calcula 5 * (2^5) usando um loop e macros.

; Macro 1: Carrega um valor, multiplica por outro e armazena de volta.
MULT_E_ARMAZENA: MACRO &DESTINO, &FATOR
    LOAD &DESTINO
    MULT &FATOR
    STORE &DESTINO
ENDMACRO

; Macro 2: Apenas para encapsular a instrução de parada.
FINALIZAR: MACRO
    STOP
ENDMACRO

; Início da Seção de Código
; Pula sobre as variáveis para iniciar a execução no lugar certo.
JMP INICIO      

; --- Seção de Dados ---
CONTADOR:   CONST 5
FATOR_MULT: CONST 2
RESULTADO:  SPACE
UM:         CONST 1

; --- Seção de Execução ---
INICIO:
    COPY CONTADOR, RESULTADO ; Copia o valor inicial 5 para RESULTADO

LOOP:
    ; Chama a macro para a operação principal (RESULTADO = RESULTADO * FATOR_MULT)
    MULT_E_ARMAZENA RESULTADO, FATOR_MULT

    ; Decrementa o contador
    LOAD CONTADOR
    SUB UM
    STORE CONTADOR
    
    ; Se o contador ainda for positivo, volta para o loop
    JMPP LOOP       

; Fim do programa
FINALIZAR