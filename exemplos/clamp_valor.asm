; --- CLAMP (LIMITA) UM VALOR A UM INTERVALO [MIN, MAX] ---
; Testa com um VALOR_TESTE de -5, no intervalo [0, 100].
; Resultado esperado em VALOR_TESTE: 0

; --- Dados ---
VALOR_TESTE:    CONST -5
MIN:            CONST 0
MAX:            CONST 100

; --- Código ---
; Compara com o máximo: VALOR_TESTE - MAX
LOAD VALOR_TESTE
SUB MAX
; Se for positivo, VALOR_TESTE > MAX. Pula para corrigir.
JMPP CORRIGE_MAXIMO

VERIFICA_MINIMO:
    ; Compara com o mínimo: VALOR_TESTE - MIN
    LOAD VALOR_TESTE
    SUB MIN
    ; Se for negativo, VALOR_TESTE < MIN. Pula para corrigir.
    JMPN CORRIGE_MINIMO
    ; Se não for nem maior que MAX, nem menor que MIN, o valor está ok.
    JMP FIM

CORRIGE_MAXIMO:
    LOAD MAX
    STORE VALOR_TESTE
    JMP FIM ; O valor foi corrigido, pode terminar

CORRIGE_MINIMO:
    LOAD MIN
    STORE VALOR_TESTE
    ; Não precisa de JMP FIM, pois ele é a próxima seção de código

FIM:
    STOP