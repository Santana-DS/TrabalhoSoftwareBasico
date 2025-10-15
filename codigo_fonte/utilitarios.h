#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <string>

/**
 * @brief Extrai o nome base de um arquivo a partir de um caminho completo ou relativo.
 * Remove tanto o caminho do diretório quanto a extensão do arquivo.
 * Exemplos:
 * - "exemplos/teste.asm" -> "teste"
 * - "C:\\pasta\\teste.asm" -> "teste"
 * - "teste.asm" -> "teste"
 * @param caminho_completo O caminho do arquivo a ser processado.
 * @return Uma string contendo apenas o nome base do arquivo.
 */
inline std::string obter_nome_base(const std::string& caminho_completo) {
    // Encontra a última barra (seja / para Linux/Mac ou \\ para Windows) para isolar o nome do arquivo.
    size_t pos_barra = caminho_completo.find_last_of("/\\");
    std::string nome_com_extensao = (pos_barra == std::string::npos) ? caminho_completo : caminho_completo.substr(pos_barra + 1);
    
    // Encontra o último ponto (.) para remover a extensão do arquivo.
    size_t pos_ponto = nome_com_extensao.find_last_of(".");
    return (pos_ponto == std::string::npos) ? nome_com_extensao : nome_com_extensao.substr(0, pos_ponto);
}

/**
 * @brief Remove espaços em branco, tabulações e quebras de linha do início e do fim de uma string.
 * @param str A string a ser "limpa".
 * @return A string sem espaços em branco nas extremidades.
 */
inline std::string trim(const std::string& str) {
    // Define todos os caracteres considerados como espaço em branco.
    const std::string whitespace = " \t\n\r\v\f";
    
    // Encontra a posição do primeiro caractere que NÃO é um espaço em branco.
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) return ""; // Se não encontrar, a string é vazia ou só tem espaços.

    // Encontra a posição do último caractere que NÃO é um espaço em branco.
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    // Retorna a substring entre o primeiro e o último caractere de conteúdo.
    return str.substr(strBegin, strRange);
}

#endif // UTILITARIOS_H