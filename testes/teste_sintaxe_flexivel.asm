; --- TESTE DE SINTAXE FLEXÍVEL E MACROS ANINHADAS ---

; Macro A: Chama a Macro B
OPERACAO_ANINHADA: MACRO &ARG1
    LOAD &ARG1
    IMPRIME_E_PARA   ; Chamada para outra macro
ENDMACRO

; Macro B: Finaliza a execução
IMPRIME_E_PARA: MACRO
    OUTPUT VALOR_FINAL ; Usa um rótulo definido bem no final
    STOP
ENDMACRO

ROTULO_UM:  ; Rótulo sozinho com comentário

    LoAd    DEZ    ; Carga com tabs, espaços e mixed case

STORE   VALOR_FINAL  ; Armazena com espaços extras

CHAMADA_MACRO:
    operacao_aninhada    VALOR_FINAL ; chamada de macro em minúsculas

; Definições de variáveis misturadas e no final do arquivo
DEZ: CONST    10
VALOR_FINAL:
    SPACE