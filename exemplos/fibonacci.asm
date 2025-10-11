; --- TESTE DE FIBONACCI ---
; Calcula o N-ésimo número da sequência de Fibonacci (onde N=8).
; O resultado esperado em RESULTADO deve ser 21.

; --- Seção de Dados ---
N:          CONST 8     ; Queremos o 8º número da sequência
FIB_N_1:    CONST 1     ; F(n-1), começa com o valor de F(1)
FIB_N_2:    CONST 0     ; F(n-2), começa com o valor de F(0)
RESULTADO:  SPACE       ; Onde o resultado de cada soma é guardado
UM:         CONST 1     ; Constante para decrementar o contador

; --- Seção de Código ---
LOOP:
    ; Se o contador N chegar a zero, o cálculo terminou.
    LOAD N
    JMPZ FIM

    ; Lógica principal: F(n) = F(n-1) + F(n-2)
    LOAD FIB_N_1
    ADD FIB_N_2
    STORE RESULTADO

    ; Atualiza as variáveis para a próxima iteração
    ; O antigo F(n-1) se torna o novo F(n-2)
    COPY FIB_N_1, FIB_N_2
    ; O resultado da soma se torna o novo F(n-1)
    COPY RESULTADO, FIB_N_1
    
    ; Decrementa o contador N
    LOAD N
    SUB UM
    STORE N

    ; Volta para o início do loop
    JMP LOOP

FIM:
    STOP