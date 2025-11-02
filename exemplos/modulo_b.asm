; Módulo B: Saída e Armazenamento dos Termos
MOD_B: BEGIN

    ; Símbolos que este módulo "exporta"
    PUBLIC INICIO_B
    PUBLIC L1, L2, L3
    
    ; Símbolos que este módulo "importa"
    EXTERN AREA
    EXTERN PONTO_FINAL

    ; --- Início do Código ---
INICIO_B:
    OUTPUT AREA       ; Mostra a área calculada em C
    JMP PONTO_FINAL   ; Pula de volta para A para terminar

    ; --- Declaração de Dados ---
    ; Variáveis da "reta" (termos da fórmula)
    L1: SPACE 1
    L2: SPACE 1
    L3: SPACE 1
END