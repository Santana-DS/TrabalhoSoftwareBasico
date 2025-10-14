#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <string>

// Função para extrair o nome base de um arquivo de um caminho completo
inline std::string obter_nome_base(const std::string& caminho_completo) {
    size_t pos_barra = caminho_completo.find_last_of("/\\");
    std::string nome_com_extensao = (pos_barra == std::string::npos) ? caminho_completo : caminho_completo.substr(pos_barra + 1);
    
    size_t pos_ponto = nome_com_extensao.find_last_of(".");
    return (pos_ponto == std::string::npos) ? nome_com_extensao : nome_com_extensao.substr(0, pos_ponto);
}

// Função para remover espaços em branco do início e do fim de uma string
inline std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\v\f";
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos) return ""; // String vazia ou só com espaços
    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;
    return str.substr(strBegin, strRange);
}

#endif // UTILITARIOS_H