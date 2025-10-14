; --- CALCULA O VALOR ABSOLUTO DE -15 ---
; Resultado esperado em NUM: 15

NUM:  CONST -15
ZERO: CONST 0

LOAD NUM
JMPP FIM    ; Se for positivo, já terminou
JMPZ FIM    ; Se for zero, já terminou

; Se for negativo, calcula 0 - NUM
LOAD ZERO
SUB NUM
STORE NUM

FIM:
STOP