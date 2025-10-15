; --- TROCA O CONTEÚDO DE DUAS VARIÁVEIS (SWAP) ---
; Valor inicial: A=10, B=20
; Valor final esperado: A=20, B=10

; --- Dados ---
A:      CONST 10
B:      CONST 20
TEMP:   SPACE   ; Variável auxiliar para a troca

; --- Código ---
; Guarda o valor de A na variável temporária
LOAD A
STORE TEMP

; Copia o valor de B para A
LOAD B
STORE A

; Recupera o valor original de A (que está em TEMP) e guarda em B
LOAD TEMP
STORE B

STOP