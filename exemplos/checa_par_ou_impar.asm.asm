; --- VERIFICA SE 7 É PAR OU ÍMPAR ---
; Resultado esperado: EH_PAR = 0, EH_IMPAR = 1

NUM:      CONST 7
DOIS:     CONST 2
TEMP:     SPACE
EH_PAR:   SPACE
EH_IMPAR: SPACE

; Calcula o resto de NUM / 2
LOAD NUM
DIV DOIS
MULT DOIS
STORE TEMP
LOAD NUM
SUB TEMP

; Se o resto (no acumulador) for zero, é par
JMPZ PAR

; É ímpar
LOAD DOIS      ; Usa o 2 como 1+1, só por diversão
SUB DOIS
ADD DOIS
SUB NUM        ; Acumulador = 0 - (7-2-2) = -3
; Vamos fazer de um jeito mais simples...
LOAD NUM       ; Acumulador = 7
DIV DOIS       ; Acumulador = 3
MULT DOIS      ; Acumulador = 6
LOAD NUM       ; Acumulador = 7
SUB TEMP       ; Acumulador = 7-6 = 1
STORE EH_IMPAR
JMP FIM

PAR:
LOAD TEMP
SUB TEMP       ; Zera o acumulador
STORE EH_PAR

FIM:
STOP