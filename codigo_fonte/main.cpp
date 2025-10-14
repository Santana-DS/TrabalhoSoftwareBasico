#include <iostream>
#include <string>
#include "pre_processador.h"
#include "montador.h"
#include "utilitarios.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        std::cerr << "Uso: ./compilador <nome_do_arquivo.asm>" << std::endl;
        return 1;
    }

    std::string nome_arquivo_asm = argv[1];
    std::string nome_base = obter_nome_base(nome_arquivo_asm);

    // --- FASE 1: PRÉ-PROCESSAMENTO ---
    // MUDANÇA: Adiciona o caminho "executavel/" ao nome do arquivo .pre
    std::string nome_arquivo_pre = "executavel/" + nome_base + ".pre";
    
    executar_pre_processamento(nome_arquivo_asm, nome_arquivo_pre);

    // --- FASE 2: MONTAGEM ---
    // O montador agora receberá o caminho completo para o arquivo .pre
    executar_montagem(nome_arquivo_pre);

    return 0;
}