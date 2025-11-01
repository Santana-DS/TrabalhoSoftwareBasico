/**
 * @file main.cpp
 * @brief Ponto de entrada do programa e orquestrador das fases de compilação.
 *
 * Responsável por:
 * 1. Validar argumentos (incluindo -h/--help).
 * 2. Chamar pré-processamento e montagem com o modo correto.
 */
#include <iostream>
#include <string>
#include "pre_processador.h"
#include "montador.h"
#include "utilitarios.h"

/**
 * @brief Imprime a mensagem de ajuda para o compilador.
 */
void imprimir_ajuda_compilador() {
    std::cout << "--- Montador (compilador.exe) ---" << std::endl;
    std::cout << "Orquestra o pré-processamento e a montagem." << std::endl;
    std::cout << "\nUSO:" << std::endl;
    std::cout << "  ./compilador.exe <arquivo.asm>         (Modo Absoluto)" << std::endl;
    std::cout << "  ./compilador.exe -c <arquivo.asm>      (Modo Modulo/Relocavel)" << std::endl;
    std::cout << "  ./compilador.exe -h ou --help          (Mostra esta ajuda)" << std::endl;
    std::cout << "\nMODOS DE OPERACAO:" << std::endl;
    std::cout << "  Modo Absoluto (padrao):" << std::endl;
    std::cout << "    Gera arquivos .pre, .o1, e .o2 na pasta 'executavel/'." << std::endl;
    std::cout << "    Rejeita diretivas PUBLIC ou EXTERN." << std::endl;
    std::cout << "\n  Modo Modulo (-c):" << std::endl;
    std::cout << "    Gera arquivos .pre e .obj na pasta 'executavel/'." << std::endl;
    std::cout << "    Processa diretivas PUBLIC e EXTERN para o ligador." << std::endl;
}

/**
 * @brief Função principal do programa.
 */
int main(int argc, char *argv[]) {
    
    std::string nome_arquivo_asm;
    bool modo_ligador = false; 

    // --- 1. VALIDAÇÃO DOS ARGUMENTOS ---
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        imprimir_ajuda_compilador();
        return 0;
    }

    if (argc == 2) {
        // Modo Padrão (Absoluto): ./compilador <arquivo.asm>
        nome_arquivo_asm = argv[1];
        modo_ligador = false;
    } else if (argc == 3 && std::string(argv[1]) == "-c") {
        // Modo Módulo (Relocável): ./compilador -c <arquivo.asm>
        nome_arquivo_asm = argv[2];
        modo_ligador = true;
    } else {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        imprimir_ajuda_compilador(); // Mostra a ajuda em caso de erro
        return 1;
    }

    // --- 2. PREPARAÇÃO DOS NOMES DE ARQUIVO ---
    std::string nome_base = obter_nome_base(nome_arquivo_asm);
    std::string nome_base_saida = "executavel/" + nome_base;
    std::string nome_arquivo_pre = nome_base_saida + ".pre";

    // --- 3. FASE 1: PRÉ-PROCESSAMENTO ---
    executar_pre_processamento(nome_arquivo_asm, nome_arquivo_pre);

    // --- 4. FASE 2: MONTAGEM ---
    executar_montagem(nome_arquivo_pre, nome_base_saida, modo_ligador);

    return 0;
}