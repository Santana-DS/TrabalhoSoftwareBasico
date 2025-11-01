; --- exemplos/modulo_func.asm (CORRIGIDO) ---
; Módulo que define e exporta uma função.

PUBLIC SOMA       ; Torna SOMA visível
EXTERN ARG1      ; Declara que ARG1 é definido no módulo principal
EXTERN ARG2      ; Declara que ARG2 é definido no módulo principal
EXTERN RET       ; Declara que RET (retorno) é definido no módulo principal

SECTION TEXT
SOMA:   LOAD  ARG1  ; Carrega o primeiro argumento (definido externamente)
        ADD   ARG2  ; Soma o segundo (definido externamente)
        JMP   RET   ; Pula de volta para o rótulo RET (definido externamente)

; Este módulo não precisa de dados, pois usa os do módulo principal.
SECTION DATA