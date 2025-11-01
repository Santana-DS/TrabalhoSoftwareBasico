; Módulo C: Cálculo da Área
MOD_C: BEGIN

    ; Símbolos que este módulo "exporta"
    PUBLIC INICIO_C
    PUBLIC AREA
    
    ; Símbolos que este módulo "importa"
    EXTERN INICIO_B
    EXTERN NUM1, NUM2, NUM3 ; Importa o endereço base dos "vetores"
    EXTERN L1, L2, L3

    ; --- Definição da Macro ---
    ; Macro calcula um termo da fórmula de Shoelace: (x1*y2 - y1*x2)
    ; Parâmetros: &P1_X, &P1_Y, &P2_X, &P2_Y, &DESTINO
    CALC_TERMO_SHOELACE: MACRO &P1_X, &P1_Y, &P2_X, &P2_Y, &DESTINO
        LOAD &P1_X      ; ACC = P1_X
        MULT &P2_Y      ; ACC = P1_X * P2_Y
        STORE TEMP1
        LOAD &P1_Y      ; ACC = P1_Y
        MULT &P2_X      ; ACC = P1_Y * P2_X
        STORE TEMP2
        LOAD TEMP1      ; ACC = (P1_X * P2_Y)
        SUB TEMP2       ; ACC = (P1_X * P2_Y) - (P1_Y * P2_X)
        STORE &DESTINO
    ENDMACRO

    ; --- Início do Código ---
INICIO_C:
    ; 1. Chama a macro 3 vezes usando os "vetores"
    CALC_TERMO_SHOELACE NUM1, NUM1+1, NUM2, NUM2+1, L1
    CALC_TERMO_SHOELACE NUM2, NUM2+1, NUM3, NUM3+1, L2
    CALC_TERMO_SHOELACE NUM3, NUM3+1, NUM1, NUM1+1, L3

    ; 2. Soma os termos
    LOAD L1
    ADD L2
    ADD L3

    ; 3. Divide por 2 (ignora valor absoluto por simplicidade)
    DIV DOIS
    STORE AREA

    JMP INICIO_B        ; Pula para o módulo B mostrar o resultado

    ; --- Declaração de Dados ---
    AREA:  SPACE 1
    DOIS:  CONST 2
    TEMP1: SPACE 1
    TEMP2: SPACE 1
END