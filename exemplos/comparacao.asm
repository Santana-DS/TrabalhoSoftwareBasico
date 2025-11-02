; --- COMPARA DOIS NÚMEROS A E B ---
; Testa com A=10 e B=10. Resultado esperado em RESULTADO: 0
; Mude os valores de A e B para testar outros casos!

; --- Dados ---
A:          CONST 10
B:          CONST 10
RESULTADO:  SPACE
MAIOR:      CONST 1
MENOR:      CONST -1
IGUAL:      CONST 0

; --- Código ---
LOAD A
SUB B

; O resultado de A-B está no acumulador.
; Testamos as três possibilidades.
JMPP A_EH_MAIOR
JMPZ SAO_IGUAIS
; Se não pulou, só pode ser menor
JMP A_EH_MENOR 

A_EH_MAIOR:
    LOAD MAIOR
    STORE RESULTADO
    JMP FIM

SAO_IGUAIS:
    LOAD IGUAL
    STORE RESULTADO
    JMP FIM

A_EH_MENOR:
    LOAD MENOR
    STORE RESULTADO
    
FIM:
    STOP