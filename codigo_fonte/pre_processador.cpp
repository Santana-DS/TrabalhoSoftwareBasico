#include "pre_processador.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Função auxiliar para substituir todas as ocorrências de uma substring
void substituir_ocorrencias(std::string& str, const std::string& de, const std::string& para) {
    if (de.empty()) return;
    size_t start_pos = 0;
    while ((start_pos = str.find(de, start_pos)) != std::string::npos) {
        str.replace(start_pos, de.length(), para);
        start_pos += para.length();
    }
}

// Função que quebra uma linha em tokens (palavras)
std::vector<std::string> tokenizar_pre(const std::string& linha) {
    std::vector<std::string> tokens;
    // Converte para maiúsculas para ser case-insensitive na detecção
    std::string linha_upper = linha;
    for (char &c : linha_upper) c = toupper(c);

    std::string codigo_sem_comentario = linha_upper.substr(0, linha_upper.find(';'));
    std::istringstream iss(codigo_sem_comentario);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// Retorna os tokens da linha original, preservando o case
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
    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo de entrada '" << nome_arquivo_entrada << "'." << std::endl;
        return;
    }

    TabelaMacros tabela_de_macros;
    std::string linha;
    
    // PASSAGEM 1: DETECÇÃO DE MACROS
    bool definindo_macro = false;
    std::string nome_macro_atual;
    while (std::getline(arquivo_entrada, linha)) {
        std::vector<std::string> tokens = tokenizar_pre(linha);
        if (tokens.empty()) continue;

        if (tokens.size() == 1 && tokens[0] == "ENDMACRO") { definindo_macro = false; continue; }
        if (definindo_macro) { tabela_de_macros[nome_macro_atual].corpo.push_back(linha); continue; }
        if (tokens.size() >= 2 && tokens[1] == "MACRO") {
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
        }
    }

    // Carrega o código-fonte (sem definições de macro) para a memória
    std::vector<std::string> linhas_a_processar;
    arquivo_entrada.clear();
    arquivo_entrada.seekg(0, std::ios::beg);
    definindo_macro = false;
    while (std::getline(arquivo_entrada, linha)) {
        std::vector<std::string> tokens = tokenizar_pre(linha);
        if (!tokens.empty() && tokens.size() >= 2 && tokens[1] == "MACRO") definindo_macro = true;
        if (!tokens.empty() && tokens.size() == 1 && tokens[0] == "ENDMACRO") { definindo_macro = false; continue; }
        if (!definindo_macro) linhas_a_processar.push_back(linha);
    }

    // PASSAGEM 2: EXPANSÃO ITERATIVA DE MACROS (LÓGICA CORRIGIDA)
    bool expansao_ocorreu = true;
    while(expansao_ocorreu) {
        expansao_ocorreu = false;
        std::vector<std::string> proximo_ciclo_linhas;
        for(const auto& linha_ciclo : linhas_a_processar) {
            std::vector<std::string> tokens_upper = tokenizar_pre(linha_ciclo);
            
            if (!tokens_upper.empty() && tabela_de_macros.count(tokens_upper[0])) {
                expansao_ocorreu = true;
                const auto& macro_def = tabela_de_macros.at(tokens_upper[0]);
                std::vector<std::string> tokens_originais = tokenizar_original_case(linha_ciclo);
                
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

    // Escreve o resultado final no arquivo .pre
    std::ofstream arquivo_saida(nome_arquivo_saida_pre);
    if (!arquivo_saida.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de saida '" << nome_arquivo_saida_pre << "'." << std::endl;
        return;
    }
    for(const auto& linha_final : linhas_a_processar) {
        arquivo_saida << linha_final << std::endl;
    }
    
    std::cout << "Arquivo '" << nome_arquivo_saida_pre << "' gerado com sucesso." << std::endl;
    arquivo_entrada.close();
    arquivo_saida.close();
}