/**
 * @file montador.cpp
 * @brief Implementação do montador de passagem única (MODIFICADO com Tabela de Usos).
 * Esta versão suporta dois modos:
 * 1. Modo Absoluto (padrão): Gera .o1 e .o2 e rejeita diretivas de ligação.
 * 2. Modo Ligador (-c): Gera .obj (com Tabela de Usos) e aceita diretivas PUBLIC/EXTERN.
 */
#include "montador.h"
#include "estruturas.h"
#include "utilitarios.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <set> 

// --- PROTÓTIPOS DAS FUNÇÕES AUXILIARES ---
std::vector<std::string> tokenizar_montador(const std::string& linha);
void popular_tabela_instrucoes(TabelaInstrucoes& ti);
bool is_number(const std::string& s);
bool is_label_valido(const std::string& s);

// --- FUNÇÃO PRINCIPAL DO MONTADOR ---
void executar_montagem(const std::string& nome_arquivo_pre, const std::string& nome_base_saida, bool modo_ligador) {
    // --- 1. INICIALIZAÇÃO ---
    TabelaInstrucoes tabela_instrucoes;
    popular_tabela_instrucoes(tabela_instrucoes);
    TabelaSimbolos tabela_simbolos;
    ListaPendencias lista_pendencias;
    TabelaRelocacao tabela_relocacao; 
    std::set<std::string> tabela_de_usos; 
    std::vector<int> codigo_objeto;
    int contador_posicao = 0;
    int contador_linha = 0;
    bool houve_erro = false;

    std::ifstream arquivo_pre(nome_arquivo_pre);
    if (!arquivo_pre.is_open()) { 
        std::cerr << "Erro: Nao foi possivel abrir o arquivo .pre '" << nome_arquivo_pre << "'" << std::endl;
        return; 
    }

    // --- 2. PASSAGEM ÚNICA ---
    std::string linha;
    while (std::getline(arquivo_pre, linha)) {
        contador_linha++;
        
        std::string linha_sem_comentario = linha.substr(0, linha.find(';'));
        std::string parte_instrucao;
        
        size_t pos_colon = linha_sem_comentario.find(':');
        
        if (pos_colon != std::string::npos) {
            std::string nome_rotulo = trim(linha_sem_comentario.substr(0, pos_colon));
            std::string nome_rotulo_upper = nome_rotulo;
            std::transform(nome_rotulo_upper.begin(), nome_rotulo_upper.end(), nome_rotulo_upper.begin(), ::toupper);

            if (!is_label_valido(nome_rotulo)) { houve_erro = true; }
            if (tabela_simbolos.count(nome_rotulo_upper) && tabela_simbolos.at(nome_rotulo_upper).definido) { houve_erro = true; } 
            else { 
                if (!tabela_simbolos.count(nome_rotulo_upper)) {
                    tabela_simbolos[nome_rotulo_upper] = {nome_rotulo_upper, contador_posicao, true, false, false};
                } else {
                    if (modo_ligador && tabela_simbolos.at(nome_rotulo_upper).eh_externo) {
                        std::cerr << "Erro Semantico (linha " << contador_linha << "): Simbolo '" << nome_rotulo << "' definido mas tambem declarado como EXTERN." << std::endl;
                        houve_erro = true;
                    }
                    tabela_simbolos[nome_rotulo_upper].endereco = contador_posicao;
                    tabela_simbolos[nome_rotulo_upper].definido = true;
                }
            }
            parte_instrucao = linha_sem_comentario.substr(pos_colon + 1);
        } else {
            parte_instrucao = linha_sem_comentario;
        }

        std::vector<std::string> tokens = tokenizar_montador(parte_instrucao);
        if (tokens.empty()) continue;
        
        std::string op_upper = tokens[0];
        std::transform(op_upper.begin(), op_upper.end(), op_upper.begin(), ::toupper);
        
        if (tabela_instrucoes.count(op_upper)) {
            const auto& instrucao = tabela_instrucoes.at(op_upper);
            if ((tokens.size() - 1) != instrucao.operandos_esperados) { houve_erro = true; } 
            else {
                codigo_objeto.push_back(instrucao.opcode);
                
                for (int i = 0; i < instrucao.operandos_esperados; ++i) {
                    std::string operando = tokens[i + 1];
                    if (operando.back() == ',') operando.pop_back();

                    if (is_number(operando)) {
                        codigo_objeto.push_back(std::stoi(operando));
                    } else {
                        std::string simbolo_base = operando;
                        int offset = 0;
                        size_t pos_soma = operando.find('+');
                        
                        if (pos_soma != std::string::npos) {
                            simbolo_base = operando.substr(0, pos_soma);
                            offset = std::stoi(operando.substr(pos_soma + 1));
                        }
                        std::transform(simbolo_base.begin(), simbolo_base.end(), simbolo_base.begin(), ::toupper);

                        if (!tabela_simbolos.count(simbolo_base)) {
                            tabela_simbolos[simbolo_base] = {simbolo_base, 0, false, false, false};
                        }

                        const auto& simbolo_info = tabela_simbolos.at(simbolo_base);
                        int endereco_a_corrigir = codigo_objeto.size();
                        codigo_objeto.push_back(offset); 

                        if (simbolo_info.eh_externo) {
                            if (!modo_ligador) {
                                std::cerr << "Erro Semantico (linha " << contador_linha << "): Simbolo externo '" << simbolo_base << "' nao permitido em modo absoluto." << std::endl;
                                houve_erro = true;
                            }
                            tabela_relocacao.push_back({endereco_a_corrigir, simbolo_base});
                            tabela_de_usos.insert(simbolo_base); 
                        } else {
                            lista_pendencias.push_back({simbolo_base, endereco_a_corrigir});
                        }
                    }
                }
            }
            contador_posicao += 1 + instrucao.operandos_esperados;
        
        // --- INÍCIO DA CORREÇÃO ---
        } else if (op_upper == "PUBLIC") {
            if (!modo_ligador) {
                std::cerr << "Erro Sintatico (linha " << contador_linha << "): Diretiva PUBLIC so e permitida em modo modulo (-c)." << std::endl;
                houve_erro = true;
                continue;
            }
            if (tokens.size() < 2) { 
                std::cerr << "Erro Sintatico (linha " << contador_linha << "): Diretiva PUBLIC espera pelo menos um simbolo." << std::endl;
                houve_erro = true; 
            } else {
                // Itera por todos os tokens (símbolos) na linha
                for (size_t i = 1; i < tokens.size(); ++i) {
                    std::string nome_simbolo = tokens[i];
                    if (nome_simbolo.back() == ',') nome_simbolo.pop_back(); // Remove vírgula
                    if (nome_simbolo.empty()) continue;
                    
                    std::transform(nome_simbolo.begin(), nome_simbolo.end(), nome_simbolo.begin(), ::toupper);
                    
                    if (!tabela_simbolos.count(nome_simbolo)) {
                        tabela_simbolos[nome_simbolo] = {nome_simbolo, 0, false, true, false};
                    } else {
                        tabela_simbolos[nome_simbolo].eh_publico = true;
                    }
                }
            }
            continue; 

        } else if (op_upper == "EXTERN") {
            if (!modo_ligador) {
                std::cerr << "Erro Sintatico (linha " << contador_linha << "): Diretiva EXTERN so e permitida em modo modulo (-c)." << std::endl;
                houve_erro = true;
                continue;
            }
            if (tokens.size() < 2) { 
                std::cerr << "Erro Sintatico (linha " << contador_linha << "): Diretiva EXTERN espera pelo menos um simbolo." << std::endl;
                houve_erro = true; 
            } else {
                // Itera por todos os tokens (símbolos) na linha
                for (size_t i = 1; i < tokens.size(); ++i) {
                    std::string nome_simbolo = tokens[i];
                    if (nome_simbolo.back() == ',') nome_simbolo.pop_back(); // Remove vírgula
                    if (nome_simbolo.empty()) continue;

                    std::transform(nome_simbolo.begin(), nome_simbolo.end(), nome_simbolo.begin(), ::toupper);
                    
                    if (tabela_simbolos.count(nome_simbolo) && tabela_simbolos.at(nome_simbolo).definido) {
                        std::cerr << "Erro Semantico (linha " << contador_linha << "): Simbolo '" << nome_simbolo << "' declarado como EXTERN mas ja definido neste modulo." << std::endl;
                        houve_erro = true;
                    } else {
                        tabela_simbolos[nome_simbolo] = {nome_simbolo, 0, false, false, true};
                    }
                }
            }
            continue;
        // --- FIM DA CORREÇÃO ---
            
        } else if (op_upper == "SECTION") {
            continue;
            
        } else if (op_upper == "BEGIN" || op_upper == "END") {
            // Ignora diretivas de início/fim do módulo
            continue;

        } else if (op_upper == "SPACE") {
            int espacos = (tokens.size() > 1 && is_number(tokens[1])) ? std::stoi(tokens[1]) : 1;
            for(int i = 0; i < espacos; ++i) codigo_objeto.push_back(0);
            contador_posicao += espacos;
        } else if (op_upper == "CONST") {
            codigo_objeto.push_back(std::stoi(tokens[1]));
            contador_posicao += 1;
        } else {
            std::cerr << "Erro Sintatico (linha " << contador_linha << "): Instrucao ou diretiva '" << tokens[0] << "' inexistente." << std::endl;
            houve_erro = true;
        }
    }
    
    // --- VERIFICAÇÃO DE ERROS ---
    if (houve_erro) { 
        std::cout << "\nErros encontrados durante a leitura. A montagem foi interrompida." << std::endl; 
        return; 
    }

    // --- RESOLUÇÃO DE PENDÊNCIAS INTERNAS ---
    for (const auto& p : lista_pendencias) { 
        if(tabela_simbolos.count(p.simbolo) && tabela_simbolos.at(p.simbolo).definido) {
            codigo_objeto[p.endereco_a_corrigir] += tabela_simbolos.at(p.simbolo).endereco; 
        } 
    }
    
    // --- VERIFICAÇÃO PÓS-RESOLUÇÃO ---
    for (const auto& p : lista_pendencias) {
        if (!tabela_simbolos.count(p.simbolo) || !tabela_simbolos.at(p.simbolo).definido) {
            if (!tabela_simbolos.count(p.simbolo) || !tabela_simbolos.at(p.simbolo).eh_externo) {
                std::cerr << "Erro Semantico: O rotulo interno '" << p.simbolo << "' foi usado mas nunca declarado." << std::endl;
                houve_erro = true;
            }
        }
    }
    if (houve_erro) { 
        std::cout << "\nErros encontrados (pendencias nao resolvidas). A montagem foi interrompida." << std::endl; 
        return; 
    }

    // --- 3. GERAÇÃO DOS ARQUIVOS DE SAÍDA (Dependente do Modo) ---
    
    if (!modo_ligador) {
        // --- MODO ABSOLUTO (Requisito do Professor) ---
        std::string nome_arquivo_o1 = nome_base_saida + ".o1";
        std::string nome_arquivo_o2 = nome_base_saida + ".o2";

        std::ofstream arquivo_o1(nome_arquivo_o1);
        arquivo_o1 << "CODIGO OBJETO INTERMEDIARIO (ja resolvido):" << std::endl;
        for(const auto& val : codigo_objeto) { arquivo_o1 << val << " "; }
        arquivo_o1 << std::endl; 
        arquivo_o1.close();
        
        std::ofstream arquivo_o2(nome_arquivo_o2);
        for (size_t i = 0; i < codigo_objeto.size(); ++i) { 
            arquivo_o2 << codigo_objeto[i] << (i == codigo_objeto.size() - 1 ? "" : " "); 
        }
        arquivo_o2.close();
        
        std::cout << "Arquivos '" << nome_arquivo_o1 << "' e '" << nome_arquivo_o2 << "' gerados com sucesso." << std::endl;

    } else {
        // --- MODO LIGADOR (Relocável) ---
        std::string nome_arquivo_obj = nome_base_saida + ".obj";
        
        std::ofstream arquivo_obj(nome_arquivo_obj);
        if (!arquivo_obj.is_open()) {
            std::cerr << "Erro: Nao foi possivel criar o arquivo .obj '" << nome_arquivo_obj << "'" << std::endl;
            return;
        }

        arquivo_obj << "--- ARQUIVO OBJETO ---" << std::endl;
        
        arquivo_obj << "\nTABELA DE USOS:" << std::endl;
        for(const auto& simbolo : tabela_de_usos) {
            arquivo_obj << simbolo << std::endl;
        }
        
        arquivo_obj << "\nTABELA DE DEFINICOES (PUBLICOS):" << std::endl;
        for(const auto& par : tabela_simbolos) {
            if (par.second.eh_publico) {
                arquivo_obj << par.second.nome << " " << par.second.endereco << std::endl;
            }
        }

        arquivo_obj << "\nTABELA DE RELOCACAO:" << std::endl;
        for(const auto& r : tabela_relocacao) {
            arquivo_obj << r.simbolo << " " << r.endereco_no_codigo << std::endl;
        }

        arquivo_obj << "\nCODIGO:" << std::endl;
        for(const auto& val : codigo_objeto) { arquivo_obj << val << " "; }
        arquivo_obj << std::endl;

        arquivo_obj.close();
        std::cout << "Arquivo '" << nome_arquivo_obj << "' gerado com sucesso (Modo Ligador)." << std::endl;
    }
}

// --- DEFINIÇÕES DAS FUNÇÕES AUXILIARES ---
// (Estas funções estão completas e não precisam de mudança)

std::vector<std::string> tokenizar_montador(const std::string& linha) { 
    std::vector<std::string> tokens; 
    std::istringstream iss(trim(linha)); 
    std::string token; 
    while (iss >> token) tokens.push_back(token); 
    return tokens; 
}

void popular_tabela_instrucoes(TabelaInstrucoes& ti) { 
    ti["ADD"] = {"ADD", 1, 1}; ti["SUB"] = {"SUB", 2, 1}; ti["MULT"] = {"MULT", 3, 1}; 
    ti["DIV"] = {"DIV", 4, 1}; ti["JMP"] = {"JMP", 5, 1}; ti["JMPN"] = {"JMPN", 6, 1}; 
    ti["JMPP"] = {"JMPP", 7, 1}; ti["JMPZ"] = {"JMPZ", 8, 1}; ti["COPY"] = {"COPY", 9, 2}; 
    ti["LOAD"] = {"LOAD", 10, 1}; ti["STORE"] = {"STORE", 11, 1}; ti["INPUT"] = {"INPUT", 12, 1}; 
    ti["OUTPUT"] = {"OUTPUT", 13, 1}; ti["STOP"] = {"STOP", 14, 0}; 
}

bool is_number(const std::string& s) { 
    if (s.empty()) return false; 
    for (char const &c : s) if (std::isdigit(c) == 0) return false; 
    return true; 
}

bool is_label_valido(const std::string& s) { 
    if (s.empty() || !(std::isalpha(s[0]) || s[0] == '_')) return false; 
    for (char const &c : s) if (!(std::isalnum(c) || c == '_')) return false; 
    return true; 
}