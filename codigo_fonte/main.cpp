/**
 * @file main.cpp
 * @brief Ponto de entrada do programa e orquestrador das fases de compilação.
 *
 * Este arquivo contém a função `main`, que é responsável por:
 * 1. Validar os argumentos da linha de comando.
 * 2. Extrair os nomes de arquivo necessários.
 * 3. Chamar a fase de pré-processamento.
 * 4. Chamar a fase de montagem em sequência.
 */
#include <iostream>
#include <string>
#include "pre_processador.h"
#include "montador.h"
#include "utilitarios.h"

/**
 * @brief Função principal do programa.
 * @param argc O número de argumentos fornecidos na linha de comando.
 * @param argv O vetor de strings contendo os argumentos.
 * @return 0 em caso de sucesso, 1 em caso de erro de uso.
 */
int main(int argc, char *argv[]) {
    // Valida se o usuário forneceu o número correto de argumentos.
    // O esperado é 2: o nome do executável (argv[0]) e o nome do arquivo .asm (argv[1]).
    if (argc != 2) {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        std::cerr << "Uso: ./compilador <nome_do_arquivo.asm>" << std::endl;
        return 1;
    }

    // Pega o nome do arquivo .asm a partir dos argumentos da linha de comando.
    std::string nome_arquivo_asm = argv[1];
    // Usa a função auxiliar para extrair o nome base (sem caminho e sem extensão).
    std::string nome_base = obter_nome_base(nome_arquivo_asm);

    // --- FASE 1: PRÉ-PROCESSAMENTO ---
    // Constrói o nome completo do arquivo de saída .pre, especificando que ele
    // deve ser criado dentro da pasta "executavel/".
    std::string nome_arquivo_pre = "executavel/" + nome_base + ".pre";
    
    // Chama a função que executa toda a lógica do pré-processador.
    executar_pre_processamento(nome_arquivo_asm, nome_arquivo_pre);

    // --- FASE 2: MONTAGEM ---
    // Chama a função que executa a montagem, passando como entrada o arquivo .pre
    // que foi gerado pela fase anterior.
    executar_montagem(nome_arquivo_pre);

    // Retorna 0 para indicar que o programa terminou com sucesso.
    return 0;
}