#ifndef MONTADOR_H
#define MONTADOR_H

#include <string>

/**
 * @brief Executa toda a fase de montagem do código.
 *
 * Esta função implementa o algoritmo de passagem única. Ela lê o arquivo .pre
 * (que já foi processado pelo pré-processador), constrói a Tabela de Símbolos,
 * traduz as instruções para código objeto e utiliza uma Lista de Pendências para
 * lidar com referências a rótulos futuros. Ao final, gera os dois arquivos de
 * saída: .o1 (com o código intermediário e a lista de pendências) e .o2 (com o
 * código objeto final e resolvido).
 *
 * @param nome_arquivo_pre O caminho do arquivo .pre a ser montado.
 */
void executar_montagem(const std::string& nome_arquivo_pre);

#endif // MONTADOR_H