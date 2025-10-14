; --- VERIFICA SE 10 É MÚLTIPLO DE 2 ---
; Resultado esperado em RESULTADO: 1 (verdadeiro)

A:          CONST 10
B:          CONST 2
RESULTADO:  SPACE
ZERO:       CONST 0
UM:         CONST 1
QUOCIENTE:  SPACE
TEMP:       SPACE

; --- Código ---
; Calcula o resto de A / B
LOAD A
DIV B
STORE QUOCIENTE
LOAD QUOCIENTE
MULT B
STORE TEMP
LOAD A
SUB TEMP

; Se o resultado (resto) no acumulador for zero, pula para a seção 'EH_MULTIPLO'
JMPZ EH_MULTIPLO

; Se não for zero (não é múltiplo), armazena 0 em RESULTADO e pula para o fim
LOAD ZERO
STORE RESULTADO
JMP FIM

; --- SEÇÃO QUE ESTAVA FALTANDO ---
; Esta seção só é executada se o pulo JMPZ for tomado
EH_MULTIPLO:
    LOAD UM
    STORE RESULTADO

; Rótulo final para onde o programa pula para terminar
FIM:
    STOP
