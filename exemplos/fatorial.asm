; --- TESTE DE FATORIAL ---
; Calcula o fatorial de N (N=5). O resultado esperado em RESULTADO é 120.

; --- Seção de Dados ---
N:          CONST 5     ; O número para o qual queremos calcular o fatorial
RESULTADO:  SPACE       ; Onde o resultado acumulado será guardado
UM:         CONST 1     ; Constante para decrementar e para comparar

; --- Seção de Código ---
INICIO:
    ; Inicializa o resultado com 1, pois qualquer fatorial começa com 1.
    LOAD UM
    STORE RESULTADO

LOOP:
    ; Condição de parada: se N se tornar 1, o loop termina.
    ; (N - 1) == 0 ? Se sim, pula para o fim.
    LOAD N
    SUB UM
    JMPZ FIM

    ; Lógica principal: RESULTADO = RESULTADO * N
    LOAD RESULTADO
    MULT N
    STORE RESULTADO
    
    ; Decrementa N para a próxima iteração
    LOAD N
    SUB UM
    STORE N

    ; Volta para o início do loop incondicionalmente
    JMP LOOP

FIM:
    STOP