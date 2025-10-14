; --- MULTIPLICA UM VETOR POR UMA CONSTANTE ---
; Resultado esperado: VETOR = {10, 20, 30}

VETOR: SPACE 3  ; Vetor de 3 posições, inicializado com 0, 0, 0
K:     CONST 5
DOIS:  CONST 2

; Inicializa o vetor com valores para o teste
LOAD K
STORE VETOR+0   ; VETOR[0] = 5
LOAD K
ADD K
STORE VETOR+1   ; VETOR[1] = 10
LOAD VETOR+1
ADD K
STORE VETOR+2   ; VETOR[2] = 15

; Multiplica cada elemento por DOIS
LOAD VETOR+0
MULT DOIS
STORE VETOR+0

LOAD VETOR+1
MULT DOIS
STORE VETOR+1

LOAD VETOR+2
MULT DOIS
STORE VETOR+2

STOP