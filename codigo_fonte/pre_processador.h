#ifndef PRE_PROCESSADOR_H
#define PRE_PROCESSADOR_H

#include <string>
#include "estruturas.h" // Inclui as definições de 'Macro' e 'TabelaMacros'

/**
 * @brief Executa toda a fase de pré-processamento.
 * * Esta função orquestra a leitura de um arquivo .asm, a identificação e armazenamento
 * de todas as definições de macro, e a subsequente expansão de todas as chamadas de macro
 * (incluindo aninhadas e com parâmetros). O resultado final é um arquivo .pre "puro",
 * sem qualquer sintaxe de macro, pronto para ser processado pelo montador.
 * * @param nome_arquivo_entrada O caminho do arquivo .asm original a ser lido.
 * @param nome_arquivo_saida O caminho do arquivo .pre a ser gerado.
 */
void executar_pre_processamento(const std::string& nome_arquivo_entrada, const std::string& nome_arquivo_saida);

#endif // PRE_PROCESSADOR_H