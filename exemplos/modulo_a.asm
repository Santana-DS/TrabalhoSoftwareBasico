; Módulo A: Entrada e Controle Principal
MOD_A: BEGIN

    ; Símbolos que este módulo "exporta"
    PUBLIC PONTO_FINAL
    PUBLIC NUM1, NUM2, NUM3 ; Exporta o endereço base de cada "vetor"
    
    ; Símbolos que este módulo "importa"
    EXTERN INICIO_C

    ; --- Início do Código ---
    INPUT NUM1      ; Lê X1 (no endereço NUM1)
    INPUT NUM1+1    ; Lê Y1 (no endereço NUM1+1)
    INPUT NUM2      ; Lê X2 (no endereço NUM2)
    INPUT NUM2+1    ; Lê Y2 (no endereço NUM2+1)
    INPUT NUM3      ; Lê X3 (no endereço NUM3)
    INPUT NUM3+1    ; Lê Y3 (no endereço NUM3+1)

    JMP INICIO_C    ; Pula para o módulo C iniciar o cálculo

PONTO_FINAL:
    STOP            ; Módulo B pulará de volta para cá

    ; --- Declaração de Dados ---
    ; 3 vetores de 2 dimensões
    NUM1: SPACE 2   ; Endereços NUM1 (x) e NUM1+1 (y)
    NUM2: SPACE 2   ; Endereços NUM2 (x) e NUM2+1 (y)
    NUM3: SPACE 2   ; Endereços NUM3 (x) e NUM3+1 (y)
END