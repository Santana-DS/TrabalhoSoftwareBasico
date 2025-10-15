#include "pre_processador.h"
#include "utilitarios.h" // Adicionado
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

void substituir_ocorrencias(std::string& str, const std::string& de, const std::string& para) {
    if (de.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(de, start_pos)) != std::string::npos) {
        str.replace(start_pos, de.length(), para);
        start_pos += para.length();
    }
}

std::vector<std::string> tokenizar_original_case(const std::string& linha) {
    std::vector<std::string> tokens;
    std::string codigo_sem_comentario = linha.substr(0, linha.find(';'));
    std::istringstream iss(codigo_sem_comentario);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

void executar_pre_processamento(const std::string& nome_arquivo_entrada, const std::string& nome_arquivo_saida_pre) {
    std::ifstream arquivo_entrada(nome_arquivo_entrada);
    if (!arquivo_entrada.is_open()) { return; }

    TabelaMacros tabela_de_macros;
    std::string linha;
    
    bool definindo_macro = false;
    std::string nome_macro_atual;
    while (std::getline(arquivo_entrada, linha)) {
        std::vector<std::string> tokens = tokenizar_original_case(linha);
        if (tokens.empty()) continue;
        
        std::string primeiro_token_upper = tokens[0];
        std::transform(primeiro_token_upper.begin(), primeiro_token_upper.end(), primeiro_token_upper.begin(), ::toupper);
        
        std::string segundo_token_upper = (tokens.size() > 1) ? tokens[1] : "";
        std::transform(segundo_token_upper.begin(), segundo_token_upper.end(), segundo_token_upper.begin(), ::toupper);

        if (segundo_token_upper == "MACRO") {
            definindo_macro = true;
            nome_macro_atual = tokens[0];
            nome_macro_atual.pop_back();
            Macro nova_macro;
            nova_macro.nome = nome_macro_atual;
            for (size_t i = 2; i < tokens.size(); ++i) {
                std::string parametro = tokens[i];
                if (parametro.back() == ',') parametro.pop_back();
                nova_macro.parametros.push_back(parametro);
            }
            tabela_de_macros[nome_macro_atual] = nova_macro;
            continue;
        }
        
        if (primeiro_token_upper == "ENDMACRO") { definindo_macro = false; continue; }
        if (definindo_macro) { tabela_de_macros[nome_macro_atual].corpo.push_back(linha); }
    }

    std::vector<std::string> linhas_a_processar;
    arquivo_entrada.clear();
    arquivo_entrada.seekg(0, std::ios::beg);
    definindo_macro = false;
    while (std::getline(arquivo_entrada, linha)) {
        std::vector<std::string> tokens = tokenizar_original_case(linha);
        if (!tokens.empty()){
            std::string p_token_upper = tokens[0];
            std::string s_token_upper = (tokens.size() > 1) ? tokens[1] : "";
            std::transform(p_token_upper.begin(), p_token_upper.end(), p_token_upper.begin(), ::toupper);
            std::transform(s_token_upper.begin(), s_token_upper.end(), s_token_upper.begin(), ::toupper);
            if (s_token_upper == "MACRO") definindo_macro = true;
            if (p_token_upper == "ENDMACRO") { definindo_macro = false; continue; }
        }
        if (!definindo_macro) linhas_a_processar.push_back(linha);
    }

    bool expansao_ocorreu = true;
    while(expansao_ocorreu) {
        expansao_ocorreu = false;
        std::vector<std::string> proximo_ciclo_linhas;
        for(const auto& linha_ciclo : linhas_a_processar) {
            std::vector<std::string> tokens_originais = tokenizar_original_case(linha_ciclo);
            if (!tokens_originais.empty() && tabela_de_macros.count(tokens_originais[0])) {
                expansao_ocorreu = true;
                const auto& macro_def = tabela_de_macros.at(tokens_originais[0]);
                std::vector<std::string> argumentos;
                for (size_t i = 1; i < tokens_originais.size(); ++i) {
                    std::string arg = tokens_originais[i];
                    if (arg.back() == ',') arg.pop_back();
                    argumentos.push_back(arg);
                }
                for (const auto& linha_corpo : macro_def.corpo) {
                    std::string linha_expandida = linha_corpo;
                    for (size_t i = 0; i < macro_def.parametros.size(); ++i) {
                        if (i < argumentos.size()) {
                            substituir_ocorrencias(linha_expandida, macro_def.parametros[i], argumentos[i]);
                        }
                    }
                    proximo_ciclo_linhas.push_back(linha_expandida);
                }
            } else {
                proximo_ciclo_linhas.push_back(linha_ciclo);
            }
        }
        linhas_a_processar = proximo_ciclo_linhas;
    }

    std::ofstream arquivo_saida(nome_arquivo_saida_pre);
    if (!arquivo_saida.is_open()) { return; }

    for(const auto& linha_final : linhas_a_processar) {
        std::string linha_trim = trim(linha_final);
        if (linha_trim.empty()) {
            arquivo_saida << std::endl;
        } else if (linha_trim[0] != ';') {
            arquivo_saida << linha_final.substr(0, linha_final.find(';')) << std::endl;
        }
    }
    
    std::cout << "Arquivo '" << nome_arquivo_saida_pre << "' gerado com sucesso." << std::endl;
    arquivo_entrada.close();
    arquivo_saida.close();
}