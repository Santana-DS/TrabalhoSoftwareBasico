; --- exemplos/modulo_main.asm (CORRIGIDO) ---
; Programa principal que usa uma função externa.

EXTERN SOMA     ; Declara que SOMA é definido em outro módulo
PUBLIC ARG1    ; Torna ARG1 público para que SOMA possa usá-lo
PUBLIC ARG2    ; Torna ARG2 público para que SOMA possa usá-lo
PUBLIC RET      ; Ponto de retorno após a chamada de SOMA

SECTION TEXT
    LOAD  UM      ; Carrega o valor 1
    STORE ARG1    ; Armazena em ARG1
    LOAD  DADO_EXTERNO ; Carrega o valor 5 (definido externamente)
    STORE ARG2    ; Armazena em ARG2
    
    JMP   SOMA    ; Chama a sub-rotina externa
RET:    STORE RESULT  ; Ponto de retorno. Armazena o resultado (que SOMA deixou no ACC)
    
    OUTPUT RESULT ; Mostra o resultado (esperado: 6)
    STOP

SECTION DATA
UM:     CONST 1
DADO_EXTERNO: CONST 5 ; Vamos mover o dado para cá para simplificar
ARG1:   SPACE 1
ARG2:   SPACE 1
RESULT: SPACE 1