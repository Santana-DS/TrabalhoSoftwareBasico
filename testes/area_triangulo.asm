; --- TESTE DA ÁREA DE UM TRIÂNGULO ---
; Calcula (base * altura) / 2 para base=10 e altura=5.
; O resultado esperado (25) será armazenado em RESULTADO.

; --- Seção de Dados ---
BASE:       CONST 10
ALTURA:     CONST 5
DOIS:       CONST 2
RESULTADO:  SPACE

; --- Seção de Código ---
; Carrega a base no acumulador
LOAD BASE
; Multiplica pela altura (acumulador = 10 * 5 = 50)
MULT ALTURA
; Divide por dois (acumulador = 50 / 2 = 25)
DIV DOIS
; Armazena o resultado final
STORE RESULTADO

; Imprime o resultado no "console" (simulado)
OUTPUT RESULTADO

STOP