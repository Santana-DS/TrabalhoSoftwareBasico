; --- ENCONTRA O MAIOR ENTRE DOIS NÚMEROS (A=10, B=20) ---
; Resultado esperado em MAX: 20

A:   CONST 10
B:   CONST 20
MAX: SPACE

; Compara A e B (calcula A - B)
LOAD A
SUB B

; Se A-B for positivo, significa que A é maior.
JMPP A_EH_MAIOR

; Se não, B é maior ou igual.
LOAD B
STORE MAX
JMP FIM

A_EH_MAIOR:
LOAD A
STORE MAX

FIM:
STOP