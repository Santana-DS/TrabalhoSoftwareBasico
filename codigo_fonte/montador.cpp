#include "montador.h"
#include "estruturas.h"
#include "utilitarios.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

std::vector<std::string> tokenizar_montador(const std::string& linha);
void popular_tabela_instrucoes(TabelaInstrucoes& ti);
bool is_number(const std::string& s);
bool is_label_valido(const std::string& s);

void executar_montagem(const std::string& nome_arquivo_pre) {
    TabelaInstrucoes tabela_instrucoes;
    popular_tabela_instrucoes(tabela_instrucoes);

    TabelaSimbolos tabela_simbolos;
    ListaPendencias lista_pendencias;
    std::vector<int> codigo_objeto;
    int contador_posicao = 0;
    int contador_linha = 0;
    bool houve_erro = false;

    std::ifstream arquivo_pre(nome_arquivo_pre);
    if (!arquivo_pre.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo .pre '" << nome_arquivo_pre << "'." << std::endl;
        return;
    }

    std::string linha;
    while (std::getline(arquivo_pre, linha)) {
        contador_linha++;
        std::vector<std::string> tokens = tokenizar_montador(linha);
        if (tokens.empty()) continue;

        if (tokens[0].back() == ':') {
            std::string nome_rotulo = tokens[0].substr(0, tokens[0].length() - 1);
            std::string nome_rotulo_upper = nome_rotulo;
            std::transform(nome_rotulo_upper.begin(), nome_rotulo_upper.end(), nome_rotulo_upper.begin(), ::toupper);

            if (!is_label_valido(nome_rotulo)) {
                std::cerr << "Erro Lexico (linha " << contador_linha << "): Rotulo '" << nome_rotulo << "' invalido." << std::endl;
                houve_erro = true;
            }
            if (tabela_simbolos.count(nome_rotulo_upper) && tabela_simbolos.at(nome_rotulo_upper).definido) {
                std::cerr << "Erro Semantico (linha " << contador_linha << "): Rotulo '" << nome_rotulo << "' declarado duas vezes." << std::endl;
                houve_erro = true;
            } else {
                tabela_simbolos[nome_rotulo_upper] = {nome_rotulo_upper, contador_posicao, true};
            }

            tokens.erase(tokens.begin());
            if (tokens.empty()) continue;

            if (tokens[0].back() == ':') {
                std::cerr << "Erro Sintatico (linha " << contador_linha << "): Dois rotulos na mesma linha." << std::endl;
                houve_erro = true;
                continue;
            }
        }

        std::string op_upper = tokens[0];
        std::transform(op_upper.begin(), op_upper.end(), op_upper.begin(), ::toupper);

        if (tabela_instrucoes.count(op_upper)) {
            const auto& instrucao = tabela_instrucoes.at(op_upper);
            if ((tokens.size() - 1) != instrucao.operandos_esperados) {
                 std::cerr << "Erro Sintatico (linha " << contador_linha << "): Instrucao '" << op_upper << "' espera " << instrucao.operandos_esperados << " operando(s), mas recebeu " << tokens.size() - 1 << "." << std::endl;
                 houve_erro = true;
            } else {
                codigo_objeto.push_back(instrucao.opcode);
                for (int i = 0; i < instrucao.operandos_esperados; ++i) {
                    std::string operando = tokens[i + 1];
                    if (operando.back() == ',') {
                        operando.pop_back();
                    }
                    if (is_number(operando)) {
                        codigo_objeto.push_back(std::stoi(operando));
                    } else {
                        std::transform(operando.begin(), operando.end(), operando.begin(), ::toupper);
                        int endereco_pendencia = codigo_objeto.size();
                        codigo_objeto.push_back(0);
                        lista_pendencias.push_back({operando, endereco_pendencia});
                    }
                }
            }
            contador_posicao += 1 + instrucao.operandos_esperados;
        } else if (op_upper == "SPACE") {
            codigo_objeto.push_back(0);
            contador_posicao += 1;
        } else if (op_upper == "CONST") {
            codigo_objeto.push_back(std::stoi(tokens[1]));
            contador_posicao += 1;
        } else {
            std::cerr << "Erro Sintatico (linha " << contador_linha << "): Instrucao ou diretiva '" << tokens[0] << "' inexistente." << std::endl;
            houve_erro = true;
        }
    }

    for (const auto& pendencia : lista_pendencias) {
        if (!tabela_simbolos.count(pendencia.simbolo)) {
            std::cerr << "Erro Semantico: O rotulo '" << pendencia.simbolo << "' foi usado mas nunca declarado." << std::endl;
            houve_erro = true;
        }
    }

    if (houve_erro) {
        std::cout << "\nErros encontrados. A montagem foi interrompida." << std::endl;
        return;
    }

    std::string nome_base = obter_nome_base(nome_arquivo_pre);
    std::string nome_arquivo_o1 = nome_base + ".o1";
    std::ofstream arquivo_o1(nome_arquivo_o1);
    for(const auto& val : codigo_objeto) arquivo_o1 << val << " ";
    arquivo_o1.close();

    for (const auto& pendencia : lista_pendencias) {
        codigo_objeto[pendencia.endereco_a_corrigir] = tabela_simbolos.at(pendencia.simbolo).endereco;
    }

    std::string nome_arquivo_o2 = nome_base + ".o2";
    std::ofstream arquivo_o2(nome_arquivo_o2);
    for (size_t i = 0; i < codigo_objeto.size(); ++i) {
        arquivo_o2 << codigo_objeto[i] << (i == codigo_objeto.size() - 1 ? "" : " ");
    }
    arquivo_o2.close();

    std::cout << "Arquivos '" << nome_arquivo_o1 << "' e '" << nome_arquivo_o2 << "' gerados com sucesso." << std::endl;
}

std::vector<std::string> tokenizar_montador(const std::string& linha) {
    std::vector<std::string> tokens;
    std::string codigo_sem_comentario = linha.substr(0, linha.find(';'));
    std::istringstream iss(codigo_sem_comentario);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

void popular_tabela_instrucoes(TabelaInstrucoes& ti) {
    ti["ADD"] = {"ADD", 1, 1}; ti["SUB"] = {"SUB", 2, 1}; ti["MULT"] = {"MULT", 3, 1};
    ti["DIV"] = {"DIV", 4, 1}; ti["JMP"] = {"JMP", 5, 1}; ti["JMPN"] = {"JMPN", 6, 1};
    ti["JMPP"] = {"JMPP", 7, 1}; ti["JMPZ"] = {"JMPZ", 8, 1};
    ti["COPY"] = {"COPY", 9, 2}; ti["LOAD"] = {"LOAD", 10, 1}; ti["STORE"] = {"STORE", 11, 1};
    ti["INPUT"] = {"INPUT", 12, 1}; ti["OUTPUT"] = {"OUTPUT", 13, 1}; ti["STOP"] = {"STOP", 14, 0};
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