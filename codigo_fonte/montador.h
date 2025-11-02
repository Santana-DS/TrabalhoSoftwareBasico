#ifndef MONTADOR_H
#define MONTADOR_H

#include <string>

/**
 * @brief Executa toda a fase de montagem do código.
 *
 * Esta função implementa o algoritmo de passagem única. Ela lê o arquivo .pre
 * (que já foi processado pelo pré-processador), constrói a Tabela de Símbolos,
 * e traduz as instruções.
 *
 * @param nome_arquivo_pre O caminho do arquivo .pre a ser montado.
 * @param nome_base_saida O caminho e nome base para os arquivos de saída (ex: "executavel/fibonacci").
 * @param modo_ligador Se 'true', gera um arquivo .obj relocável. Se 'false', gera os arquivos .o1 e .o2 absolutos.
 */
void executar_montagem(const std::string& nome_arquivo_pre, const std::string& nome_base_saida, bool modo_ligador);

#endif // MONTADOR_H