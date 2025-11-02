/**
 * @file ligador.cpp
 * @brief Implementação de um ligador de dois passos (lê .obj otimizado).
 *
 * Esta versão foi atualizada para processar a Tabela de Relocação Externa
 * no formato agrupado (ex: SIMBOLO: 10, 15, 20).
 */
#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <vector>
#include <map>
#include <set> 
#include "estruturas.h"
#include "utilitarios.h" // <-- Adicionado para usar trim()

using TabelaGlobalSimbolos = std::map<std::string, int>;

/**
 * @brief Imprime a mensagem de ajuda para o ligador.
 */
void imprimir_ajuda_ligador() {
    std::cout << "--- Ligador (ligador.exe) ---" << std::endl;
    std::cout << "Combina multiplos arquivos objeto (.obj) em um unico executavel (.exe)." << std::endl;
    std::cout << "\nUSO:" << std::endl;
    std::cout << "  ./ligador.exe -o <saida.exe> -m <saida.map> <entrada1.obj> [entrada2.obj ...]" << std::endl;
    std::cout << "  ./ligador.exe -h ou --help" << std::endl;
    std::cout << "\nARGUMENTOS:" << std::endl;
    std::cout << "  -o <saida.exe>: (Obrigatorio) Especifica o nome do arquivo executavel final." << std::endl;
    std::cout << "  -m <saida.map>: (Obrigatorio) Especifica o nome do arquivo de mapa (log de ligacao)." << std::endl;
    std::cout << "  <entrada.obj>:  (Obrigatorio) Um ou mais arquivos objeto a serem ligados." << std::endl;
}

/**
 * @brief Executa as duas passagens do ligador.
 */
void executar_ligacao(const std::string& nome_arquivo_saida, const std::string& nome_arquivo_mapa, const std::vector<std::string>& arquivos_entrada) {
    
    std::stringstream log_ss;
    log_ss << "--- INICIANDO LIGACAO ---" << std::endl;
    log_ss << "Arquivo de Saida: " << nome_arquivo_saida << std::endl;
    log_ss << "Arquivo de Mapa: " << nome_arquivo_mapa << std::endl;
    log_ss << "Arquivos de Entrada:" << std::endl;
    for (const auto& arq : arquivos_entrada) {
        log_ss << "  - " << arq << std::endl;
    }

    TabelaGlobalSimbolos tabela_global_simbolos;
    std::set<std::string> global_tabela_de_usos; 
    int offset_relocacao_atual = 0; 
    bool houve_erro = false;
    std::map<std::string, int> tamanho_modulos;
    std::map<std::string, int> offset_modulos;

    // --- PASSO 1: CONSTRUIR TABELA GLOBAL DE SIMBOLOS (Passagem 1) ---
    log_ss << "\nIniciando Passagem 1 (Construcao da Tabela Global de Simbolos)..." << std::endl;

    for (const auto& nome_arquivo : arquivos_entrada) {
        std::ifstream arquivo_obj(nome_arquivo);
        if (!arquivo_obj.is_open()) {
            std::cerr << "Erro Ligador: Nao foi possivel abrir o arquivo '" << nome_arquivo << "'" << std::endl;
            houve_erro = true;
            continue;
        }

        offset_modulos[nome_arquivo] = offset_relocacao_atual; 
        int tamanho_codigo_modulo = 0;
        std::string linha;
        std::string secao_atual = "NENHUMA";

        while (std::getline(arquivo_obj, linha)) {
            if (linha.find("TABELA DE USOS:") != std::string::npos) { secao_atual = "USOS"; continue; }
            else if (linha.find("TABELA DE DEFINICOES (PUBLICOS):") != std::string::npos) { secao_atual = "DEFINICOES"; continue; }
            else if (linha.find("TABELA DE RELOCACAO (EXTERNOS):") != std::string::npos) { secao_atual = "RELOCACAO_EXT"; continue; }
            else if (linha.find("TABELA DE RELOCACAO (INTERNA):") != std::string::npos) { secao_atual = "RELOCACAO_INT"; continue; }
            else if (linha.find("CODIGO:") != std::string::npos) { secao_atual = "CODIGO"; continue; }
            else if (linha.find("---") != std::string::npos || linha.empty()) { continue; }

            std::istringstream iss(linha);
            if (secao_atual == "CODIGO") {
                int opcode;
                while (iss >> opcode) { tamanho_codigo_modulo++; }
            } else if (secao_atual == "DEFINICOES") {
                std::string simbolo;
                int endereco_relativo;
                iss >> simbolo >> endereco_relativo;
                if (tabela_global_simbolos.count(simbolo)) {
                    log_ss << "Erro Ligador: Simbolo publico '" << simbolo << "' definido multiplas vezes." << std::endl;
                    houve_erro = true;
                } else {
                    int endereco_absoluto = endereco_relativo + offset_relocacao_atual;
                    tabela_global_simbolos[simbolo] = endereco_absoluto;
                }
            } else if (secao_atual == "USOS") { 
                std::string simbolo;
                iss >> simbolo;
                if (!simbolo.empty()) {
                    global_tabela_de_usos.insert(simbolo);
                }
            }
        }
        tamanho_modulos[nome_arquivo] = tamanho_codigo_modulo; 
        offset_relocacao_atual += tamanho_codigo_modulo; 
        arquivo_obj.close();
    }

    if (houve_erro) {
        std::cerr << "\nErros encontrados na Passagem 1. Ligacao interrompida." << std::endl;
        std::ofstream arquivo_mapa(nome_arquivo_mapa);
        if (arquivo_mapa.is_open()) {
            arquivo_mapa << log_ss.str(); 
            arquivo_mapa.close();
            std::cerr << "Log de erro salvo em: " << nome_arquivo_mapa << std::endl;
        }
        return;
    }
    
    log_ss << "\nVerificando simbolos externos (Tabela de Usos)..." << std::endl;
    for (const auto& simbolo_usado : global_tabela_de_usos) {
        if (!tabela_global_simbolos.count(simbolo_usado)) {
            log_ss << "Erro Ligador: Simbolo externo '" << simbolo_usado << "' e usado, mas nunca foi definido publicamente em nenhum modulo." << std::endl;
            houve_erro = true;
        }
    }
    
    if (houve_erro) {
        std::cerr << "\nErros de definicao encontrados. Ligacao interrompida." << std::endl;
        std::ofstream arquivo_mapa(nome_arquivo_mapa);
        if (arquivo_mapa.is_open()) {
            arquivo_mapa << log_ss.str(); 
            arquivo_mapa.close();
            std::cerr << "Log de erro salvo em: " << nome_arquivo_mapa << std::endl;
        }
        return;
    }

    log_ss << "\n--- Tabela Global de Simbolos (FINAL, Relativa a 0) ---" << std::endl;
    for(const auto& par : tabela_global_simbolos) {
        log_ss << "Simbolo: " << par.first << ", Endereco: " << par.second << std::endl;
    }
    log_ss << "-------------------------------------------\n";

    // --- PASSO 2: RELOCAR E UNIR CODIGO (Passagem 2) ---
    log_ss << "\nIniciando Passagem 2 (Relocacao e Geracao de Codigo)..." << std::endl;
    
    std::vector<int> codigo_final;
    std::set<int> tabela_relocacao_global; 

    // Passo 2a: Unir todo o código
    log_ss << "  > Etapa 2a: Unindo codigo..." << std::endl;
    for (const auto& nome_arquivo : arquivos_entrada) {
        std::ifstream arquivo_obj(nome_arquivo);
        std::string linha;
        std::string secao_atual = "NENHUMA";
        while (std::getline(arquivo_obj, linha)) {
            if (linha.find("CODIGO:") != std::string::npos) { secao_atual = "CODIGO"; continue; }
            if (secao_atual == "NENHUMA" || secao_atual == "CODIGO") {
                if (linha.find("TABELA") != std::string::npos || linha.find("---") != std::string::npos) {
                    secao_atual = "NENHUMA"; 
                    continue;
                }
                if (secao_atual == "CODIGO") {
                    std::istringstream iss(linha);
                    int opcode;
                    while (iss >> opcode) { codigo_final.push_back(opcode); }
                }
            }
        }
        arquivo_obj.close();
    }

    // Passo 2b: Aplicar Relocação
    log_ss << "  > Etapa 2b: Aplicando relocacao (relativa a 0)..." << std::endl;
    for (const auto& nome_arquivo : arquivos_entrada) {
        std::ifstream arquivo_obj(nome_arquivo);
        std::string linha;
        std::string secao_atual = "NENHUMA";
        int offset_deste_modulo = offset_modulos.at(nome_arquivo);

        while (std::getline(arquivo_obj, linha)) {
            if (linha.find("TABELA DE RELOCACAO (EXTERNOS):") != std::string::npos) { secao_atual = "RELOCACAO_EXT"; continue; }
            else if (linha.find("TABELA DE RELOCACAO (INTERNA):") != std::string::npos) { secao_atual = "RELOCACAO_INT"; continue; }
            
            if (secao_atual != "NENHUMA" && (linha.find("TABELA") != std::string::npos || linha.find("---") != std::string::npos || linha.find("CODIGO") != std::string::npos)) {
                 if(linha.find("TABELA DE RELOCACAO (EXTERNOS):") == std::string::npos && linha.find("TABELA DE RELOCACAO (INTERNA):") == std::string::npos) {
                    secao_atual = "NENHUMA"; 
                    continue;
                 }
            }
            
            // --- INÍCIO DA CORREÇÃO ---
            if (secao_atual == "RELOCACAO_EXT") {
                // Novo formato: SIMBOLO: 10, 15, 20
                
                size_t pos_colon = linha.find(':');
                if (pos_colon == std::string::npos) continue; // Linha mal formatada

                std::string simbolo = trim(linha.substr(0, pos_colon));
                std::string enderecos_str = trim(linha.substr(pos_colon + 1));
                
                if(simbolo.empty()) continue; 

                int endereco_absoluto_do_simbolo = tabela_global_simbolos.at(simbolo);
                
                std::istringstream iss_enderecos(enderecos_str);
                std::string endereco_individual_str;
                
                // Itera por cada endereço separado por vírgula
                while (std::getline(iss_enderecos, endereco_individual_str, ',')) {
                    int endereco_relativo_a_corrigir = std::stoi(trim(endereco_individual_str));
                    int indice_no_codigo_final = endereco_relativo_a_corrigir + offset_deste_modulo;
                    
                    codigo_final[indice_no_codigo_final] += endereco_absoluto_do_simbolo;
                    tabela_relocacao_global.insert(indice_no_codigo_final);
                    
                    log_ss << "    > [EXT] Resolvido simbolo '" << simbolo << "' no endereco " << indice_no_codigo_final 
                            << ". (Valor adicionado: " << endereco_absoluto_do_simbolo << ")" << std::endl;
                }
            
            } else if (secao_atual == "RELOCACAO_INT") {
                // Formato: 10 (um endereço por linha)
                std::istringstream iss(linha);
                int endereco_relativo_a_corrigir;
                if (!(iss >> endereco_relativo_a_corrigir)) continue; 
                
                int indice_no_codigo_final = endereco_relativo_a_corrigir + offset_deste_modulo;

                codigo_final[indice_no_codigo_final] += offset_deste_modulo;
                tabela_relocacao_global.insert(indice_no_codigo_final);

                log_ss << "    > [INT] Marcado endereco relativo " << endereco_relativo_a_corrigir 
                        << " (Absoluto: " << indice_no_codigo_final << ") para relocacao." << std::endl;
            }
            // --- FIM DA CORREÇÃO ---
        }
        arquivo_obj.close();
    }

    
    // --- PASSO 3: GERAR ARQUIVO DE SAIDA (EXECUTÁVEL ESTRUTURADO) ---
    std::ofstream arquivo_saida(nome_arquivo_saida);
    if (!arquivo_saida.is_open()) {
        std::cerr << "Erro Ligador: Nao foi possivel criar o arquivo de saida '" << nome_arquivo_saida << "'" << std::endl;
        return;
    }

    arquivo_saida << "RELOCACAO_GLOBAL:" << std::endl;
    for(int addr : tabela_relocacao_global) {
        arquivo_saida << addr << " ";
    }
    arquivo_saida << std::endl;

    arquivo_saida << "CODIGO:" << std::endl;
    for (size_t i = 0; i < codigo_final.size(); ++i) {
        arquivo_saida << codigo_final[i] << (i == codigo_final.size() - 1 ? "" : " ");
    }
    arquivo_saida.close();
    log_ss << "\nArquivo executavel relocavel gerado: " << nome_arquivo_saida << std::endl;

    // --- PASSO 4: GERAR ARQUIVO DE MAPA ---
    log_ss << "\nLigacao concluida com sucesso!" << std::endl;

    std::ofstream arquivo_mapa(nome_arquivo_mapa);
    if (!arquivo_mapa.is_open()) {
        std::cerr << "Erro Ligador: Nao foi possivel criar o arquivo de mapa '" << nome_arquivo_mapa << "'" << std::endl;
    } else {
        arquivo_mapa << log_ss.str(); 
        arquivo_mapa.close();
    }

    std::cout << "Ligacao concluida com sucesso!" << std::endl;
    std::cout << "  > Executavel gerado: " << nome_arquivo_saida << std::endl;
    std::cout << "  > Mapa gerado:       " << nome_arquivo_mapa << std::endl;
}


/**
 * @brief Função principal do Ligador.
 */
int main(int argc, char *argv[]) {
    // 1. Validar Argumentos
    if (argc >= 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        imprimir_ajuda_ligador();
        return 0;
    }
    if (argc < 6) {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        imprimir_ajuda_ligador();
        return 1;
    }
    std::string nome_arquivo_saida;
    std::string nome_arquivo_mapa;
    std::vector<std::string> arquivos_entrada;
    if (std::string(argv[1]) != "-o") {
        std::cerr << "Erro: Flag '-o' para arquivo de saida nao encontrada na posicao correta." << std::endl;
        return 1;
    }
    nome_arquivo_saida = argv[2];
    if (std::string(argv[3]) != "-m") {
        std::cerr << "Erro: Flag '-m' para arquivo de mapa nao encontrada na posicao correta." << std::endl;
        return 1;
    }
    nome_arquivo_mapa = argv[4];
    for (int i = 5; i < argc; ++i) {
        arquivos_entrada.push_back(argv[i]);
    }

    // 2. Chama a função principal de ligação
    executar_ligacao(nome_arquivo_saida, nome_arquivo_mapa, arquivos_entrada);

    return 0;
}