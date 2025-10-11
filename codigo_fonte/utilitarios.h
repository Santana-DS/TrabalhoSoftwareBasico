#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <string>

inline std::string obter_nome_base(const std::string& caminho_completo) {
    size_t pos_barra = caminho_completo.find_last_of("/\\");
    std::string nome_com_extensao = (pos_barra == std::string::npos) ? caminho_completo : caminho_completo.substr(pos_barra + 1);
    size_t pos_ponto = nome_com_extensao.find_last_of(".");
    return (pos_ponto == std::string::npos) ? nome_com_extensao : nome_com_extensao.substr(0, pos_ponto);
}

#endif // UTILITARIOS_H