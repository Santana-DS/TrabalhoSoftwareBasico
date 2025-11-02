; --- CALCULA A POTENCIA (BASE^EXPOENTE) ---
; Este exemplo calcula 3^4. Resultado esperado em RESULTADO: 81

; --- Dados ---
BASE:       CONST 3
EXPOENTE:   CONST 4
RESULTADO:  SPACE
UM:         CONST 1

; --- Código ---
; Inicia o resultado com 1, pois qualquer número elevado a 0 é 1.
LOAD UM
STORE RESULTADO

LOOP:
    ; Se o expoente chegar a zero, o cálculo terminou.
    LOAD EXPOENTE
    JMPZ FIM

    ; RESULTADO = RESULTADO * BASE
    LOAD RESULTADO
    MULT BASE
    STORE RESULTADO

    ; Decrementa o expoente
    LOAD EXPOENTE
    SUB UM
    STORE EXPOENTE

    ; Volta para o início do loop
    JMP LOOP

FIM:
    STOP