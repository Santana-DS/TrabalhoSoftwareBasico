; --- ORDENA TRÊS NÚMEROS (A=5, B=2, C=8) ---
; Resultado esperado: A=2, B=5, C=8

A: CONST 5
B: CONST 2
C: CONST 8
T: SPACE ; Temporário para troca

; Lógica de Bubble Sort para 3 elementos
; Passa 1: Compara A e B
LOAD A
SUB B
JMPN PULAR_TROCA_AB ; Se A <= B, não troca
; Troca A e B
LOAD A
STORE T
LOAD B
STORE A
LOAD T
STORE B

PULAR_TROCA_AB:
; Passa 2: Compara B e C
LOAD B
SUB C
JMPN PULAR_TROCA_BC
; Troca B e C
LOAD B
STORE T
LOAD C
STORE B
LOAD T
STORE C

PULAR_TROCA_BC:
; Passa 3: Re-compara A e B
LOAD A
SUB B
JMPN FIM
; Troca A e B
LOAD A
STORE T
LOAD B
STORE A
LOAD T
STORE B

FIM:
STOP