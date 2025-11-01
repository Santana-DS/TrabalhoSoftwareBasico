; Macro para inicializar uma variavel com um valor
; e somar outro valor a ela.
INICIA_E_SOMA: MACRO &VAR, &VALOR_INICIAL, &VALOR_SOMA
    LOAD &VALOR_INICIAL
    STORE &VAR
    LOAD &VAR
    ADD &VALOR_SOMA
    STORE &VAR
ENDMACRO

; O programa comeca aqui
START:
    ; Chamada da macro para operar na variavel N1
    INICIA_E_SOMA N1, 10, 5

    ; Chamada da macro para operar na variavel N2
    INICIA_E_SOMA N2, 0, 100

    STOP

N1: SPACE
N2: SPACE