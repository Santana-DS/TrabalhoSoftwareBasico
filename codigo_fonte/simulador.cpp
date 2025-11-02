/**
 * @file simulador.cpp
 * @brief Implementação de um Carregador e Simulador de CPU (com Memória Esparsa).
 *
 * Esta versão final usa std::map para a memória, permitindo o carregamento
 * em qualquer endereço (ex: 211028097) sem estourar a memória.
 * Log é salvo na mesma pasta do executável.
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map> 
#include <set> 
#include "utilitarios.h" 

// --- Variáveis Globais da CPU ---
std::map<int, int> memoria_simulada; 
int ACC = 0; // Acumulador
int PC = 0;  // Contador de Programa

// --- Mapa Global de Opcodes ---
std::map<int, std::pair<std::string, int>> opcode_map;

/**
 * @brief Preenche o mapa de opcodes com seus nomes e tamanhos
 */
void popular_mapa_opcodes() {
    opcode_map[1] = {"ADD", 2};
    opcode_map[2] = {"SUB", 2};
    opcode_map[3] = {"MULT", 2};
    opcode_map[4] = {"DIV", 2};
    opcode_map[5] = {"JMP", 2};
    opcode_map[6] = {"JMPN", 2};
    opcode_map[7] = {"JMPP", 2};
    opcode_map[8] = {"JMPZ", 2};
    opcode_map[9] = {"COPY", 3};
    opcode_map[10] = {"LOAD", 2};
    opcode_map[11] = {"STORE", 2};
    opcode_map[12] = {"INPUT", 2};
    opcode_map[13] = {"OUTPUT", 2};
    opcode_map[14] = {"STOP", 1};
}

/**
 * @brief Imprime a mensagem de ajuda para o simulador.
 */
void imprimir_ajuda_simulador() {
    // (Esta função permanece a mesma)
    std::cout << "--- Simulador (simulador.exe) ---" << std::endl;
    std::cout << "Carrega, executa ou desmonta um arquivo de codigo objeto (.o2 ou .exe)." << std::endl;
    std::cout << "\nUSO:" << std::endl;
    std::cout << "  ./simulador.exe [-d | -dump] <arquivo_objeto> [endereco_base]" << std::endl;
    std::cout << "  ./simulador.exe -h ou --help" << std::endl;
    std::cout << "\nMODOS DE OPERACAO:" << std::endl;
    std::cout << "  (nenhum flag):  Executa o programa. O log contera a memoria e a execucao." << std::endl;
    std::cout << "  -d:             Modo 'Debug Trace'. O log contera a memoria e a execucao passo a passo." << std::endl;
    std::cout << "  -dump:          Modo 'Disassembly'. O log contera APENAS a memoria." << std::endl;
    std::cout << "\nARGUMENTOS:" << std::endl;
    std::cout << "  <arquivo_objeto>: (Obrigatorio) O arquivo .o2 ou .exe." << std::endl;
    std::cout << "  [endereco_base]:  (Opcional) Endereco de carregamento. Padrao: 0" << std::endl;
}

/**
 * @brief Carrega o programa na memória simulada (std::map)
 */
int carregar_programa(const std::string& nome_arquivo, int endereco_base) {
    // (Esta função permanece a mesma)
    std::ifstream arquivo_objeto(nome_arquivo);
    if (!arquivo_objeto.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << nome_arquivo << "'" << std::endl;
        return -1;
    }
    std::cout << "Carregando arquivo '" << nome_arquivo << "' na memoria a partir do endereco " << endereco_base << "..." << std::endl;
    
    std::set<int> tabela_relocacao_global;
    std::string linha;
    std::string secao_atual = "NENHUMA";
    int contador_posicao = 0;
    std::vector<int> codigo_buffer;

    while (std::getline(arquivo_objeto, linha)) {
        if (linha.find("RELOCACAO_GLOBAL:") != std::string::npos) { secao_atual = "RELOCACAO"; continue; }
        else if (linha.find("CODIGO:") != std::string::npos) { secao_atual = "CODIGO"; continue; }
        else if (linha.find("---") != std::string::npos || linha.empty()) { continue; }

        std::istringstream iss(linha);
        if (secao_atual == "RELOCACAO") {
            int addr;
            while (iss >> addr) {
                tabela_relocacao_global.insert(addr);
            }
        } else if (secao_atual == "CODIGO") {
            int valor;
            while (iss >> valor) {
                codigo_buffer.push_back(valor);
            }
        }
    }
    arquivo_objeto.close();

    for (size_t i = 0; i < codigo_buffer.size(); ++i) {
        int valor = codigo_buffer[i];
        int endereco_absoluto = i + endereco_base;
        
        if (tabela_relocacao_global.count(i)) {
            memoria_simulada[endereco_absoluto] = valor + endereco_base;
        } else {
            memoria_simulada[endereco_absoluto] = valor;
        }
    }
    
    contador_posicao = codigo_buffer.size();
    if (contador_posicao == 0) {
        std::cout << "Aviso: O arquivo '" << nome_arquivo << "' esta vazio ou em formato invalido." << std::endl;
    }
    
    std::cout << "Programa carregado e relocado com sucesso (" << contador_posicao << " posicoes)." << std::endl;
    return contador_posicao;
}

/**
 * @brief (MODO -dump) Desmonta o código carregado na memória.
 */
void desmontar_memoria(int endereco_base, int posicoes_carregadas, std::stringstream& log_ss) {
    // (Esta função permanece a mesma)
    log_ss << "--- DUMP BRUTO DA MEMORIA (LIGADO E CARREGADO) ---" << std::endl;
    for (int i = 0; i < posicoes_carregadas; ++i) {
        log_ss << memoria_simulada[endereco_base + i] << " ";
    }
    log_ss << "\n--- FIM DO DUMP BRUTO ---" << std::endl;
    log_ss << "\n\n--- DESMONTAGEM DA MEMORIA ---" << std::endl;
    log_ss << "Endereco carregado: " << endereco_base << ", Tamanho: " << posicoes_carregadas << " palavras." << std::endl;
    log_ss << "---------------------------------------------------------\n";
    log_ss << "[Endereço]\t[Valor]\t[Instrução Desmontada]\n";
    int i = endereco_base;
    int fim_programa = endereco_base + posicoes_carregadas;
    while (i < fim_programa) {
        int valor = memoria_simulada[i];
        log_ss << "[" << i << "]\t\t" << valor << "\t";
        if (opcode_map.count(valor)) {
            auto& info = opcode_map[valor];
            std::string nome_instr = info.first;
            int tamanho_instr = info.second;
            log_ss << nome_instr;
            if (i + tamanho_instr - 1 < fim_programa) {
                if (tamanho_instr == 2) {
                    log_ss << " " << memoria_simulada[i + 1];
                } else if (tamanho_instr == 3) {
                    log_ss << " " << memoria_simulada[i + 1] << ", " << memoria_simulada[i + 2];
                }
            } else {
                log_ss << " (Operandos fora dos limites do programa)";
            }
            log_ss << std::endl;
            i += tamanho_instr; 
        } else {
            log_ss << "(Dado)" << std::endl;
            i += 1; 
        }
    }
    log_ss << "--- FIM DA DESMONTAGEM ---" << std::endl;
}

/**
 * @brief (MODO -d) Executa o código carregado na memória.
 */
void executar_simulacao(int endereco_base, std::stringstream& log_ss, bool modo_debug) {
    // (Esta função permanece a mesma)
    PC = endereco_base; 
    bool executando = true;
    log_ss << "--- INICIANDO EXECUCAO ---" << std::endl;
    if (modo_debug) {
        log_ss << "(Modo Debug Ativado)\n";
        log_ss << "-------------------------------------------\n";
        log_ss << "[PC]\t[ACC]\t[OPCODE]\t[INSTRUCAO]\n";
    }
    while (executando) {
        if (PC < 0) {
            log_ss << "Erro de Execucao: Contador de Programa (PC) negativo (" << PC << ")" << std::endl;
            executando = false;
            continue;
        }
        int opcode = memoria_simulada[PC];
        int operando1, operando2; 
        if (modo_debug) {
            std::string instr_nome = (opcode_map.count(opcode)) ? opcode_map[opcode].first : "???";
            log_ss << PC << "\t" << ACC << "\t" << opcode << "\t\t" << instr_nome;
            if (opcode == 9) { 
                log_ss << " " << memoria_simulada[PC+1] << ", " << memoria_simulada[PC+2] << std::endl;
            } else if (opcode >= 1 && opcode <= 13) { 
                log_ss << " " << memoria_simulada[PC+1] << std::endl;
            } else { 
                log_ss << std::endl;
            }
        }
        switch (opcode) {
            case 1: // ADD
                operando1 = memoria_simulada[PC + 1]; 
                ACC += memoria_simulada[operando1]; 
                PC += 2;
                break;
            case 2: // SUB
                operando1 = memoria_simulada[PC + 1];
                ACC -= memoria_simulada[operando1];
                PC += 2;
                break;
            case 3: // MULT
                operando1 = memoria_simulada[PC + 1];
                ACC *= memoria_simulada[operando1];
                PC += 2;
                break;
            case 4: // DIV
                operando1 = memoria_simulada[PC + 1];
                if (memoria_simulada[operando1] == 0) {
                    log_ss << "Erro de Execucao: Divisao por zero no endereco " << PC << std::endl;
                    executando = false;
                } else {
                    ACC /= memoria_simulada[operando1];
                    PC += 2;
                }
                break;
            case 5: // JMP
                operando1 = memoria_simulada[PC + 1];
                PC = operando1; 
                break;
            case 6: // JMPN
                operando1 = memoria_simulada[PC + 1];
                PC = (ACC < 0) ? operando1 : (PC + 2);
                break;
            case 7: // JMPP
                operando1 = memoria_simulada[PC + 1];
                PC = (ACC > 0) ? operando1 : (PC + 2);
                break;
            case 8: // JMPZ
                operando1 = memoria_simulada[PC + 1];
                PC = (ACC == 0) ? operando1 : (PC + 2);
                break;
            case 9: // COPY
                operando1 = memoria_simulada[PC + 1]; 
                operando2 = memoria_simulada[PC + 2]; 
                memoria_simulada[operando2] = memoria_simulada[operando1];
                PC += 3;
                break;
            case 10: // LOAD
                operando1 = memoria_simulada[PC + 1];
                ACC = memoria_simulada[operando1];
                PC += 2;
                break;
            case 11: // STORE
                operando1 = memoria_simulada[PC + 1];
                memoria_simulada[operando1] = ACC;
                PC += 2;
                break;
            case 12: // INPUT
                operando1 = memoria_simulada[PC + 1];
                std::cout << "INPUT (endereco " << operando1 << "): ";
                std::cin >> memoria_simulada[operando1]; 
                PC += 2;
                break;
            case 13: // OUTPUT
                operando1 = memoria_simulada[PC + 1];
                log_ss << "OUTPUT (do endereco " << operando1 << "): " << memoria_simulada[operando1] << std::endl;
                PC += 2;
                break;
            case 14: // STOP
                executando = false;
                break; 
            default:
                log_ss << "Erro de Execucao: Opcode desconhecido (" << opcode << ") no endereco " << PC << std::endl;
                executando = false;
                break;
        }
    }
    log_ss << "--- EXECUCAO CONCLUIDA ---" << std::endl;
    log_ss << "Valor final do Acumulador (ACC): " << ACC << std::endl;
}

/**
 * @brief Função principal do Simulador.
 */
int main(int argc, char *argv[]) {
    popular_mapa_opcodes(); 
    
    // 1. Validar Argumentos
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        imprimir_ajuda_simulador();
        return 0;
    }

    bool modo_debug = false;
    bool modo_dump = false; 
    std::string nome_arquivo_exec;
    int endereco_base = 0;
    int arg_idx_arquivo = 1; 

    std::string flag = "";
    if (argc >= 3) {
        flag = std::string(argv[1]);
        if (flag == "-d") {
            modo_debug = true;
            arg_idx_arquivo = 2;
        } else if (flag == "-dump") {
            modo_dump = true;
            arg_idx_arquivo = 2;
        } else if (flag[0] == '-') {
             std::cerr << "Erro: Flag '" << flag << "' desconhecida." << std::endl;
             imprimir_ajuda_simulador();
             return 1;
        }
    }
    
    if (argc <= arg_idx_arquivo) {
        std::cerr << "Erro: Formato de execucao invalido. Faltando nome do arquivo." << std::endl;
        imprimir_ajuda_simulador();
        return 1;
    }

    nome_arquivo_exec = argv[arg_idx_arquivo];

    if (argc == (arg_idx_arquivo + 2)) {
        try {
            endereco_base = std::stoi(argv[arg_idx_arquivo + 1]);
        } catch (const std::exception& e) {
            std::cerr << "Erro: Endereco base '" << argv[arg_idx_arquivo + 1] << "' invalido." << std::endl;
            return 1;
        }
    } else if (argc > (arg_idx_arquivo + 2)) {
        std::cerr << "Erro: Argumentos em excesso." << std::endl;
        imprimir_ajuda_simulador();
        return 1;
    }

    // 2. Carregar o Programa
    int posicoes_carregadas = carregar_programa(nome_arquivo_exec, endereco_base);
    if (posicoes_carregadas <= 0) {
        std::cerr << "Falha no carregamento. Simulacao abortada." << std::endl;
        return 1;
    }

    // 3. Preparar o Log de Execução
    std::stringstream log_execucao_ss;
    
    // --- INÍCIO DA CORREÇÃO ---
    // Em vez de obter_nome_base, substituímos a extensão do caminho original.
    std::string nome_arquivo_log;
    size_t pos_ponto = nome_arquivo_exec.find_last_of(".");
    if (pos_ponto != std::string::npos) {
        nome_arquivo_log = nome_arquivo_exec.substr(0, pos_ponto) + ".log";
    } else {
        nome_arquivo_log = nome_arquivo_exec + ".log"; // Se não tiver extensão
    }
    // --- FIM DA CORREÇÃO ---
    
    // 4. Executar o Modo selecionado
    if (modo_dump) {
        std::cout << "Modo -dump: Desmontando memoria para " << nome_arquivo_log << "..." << std::endl;
        desmontar_memoria(endereco_base, posicoes_carregadas, log_execucao_ss);
        std::cout << "Desmontagem concluida." << std::endl;
    } else {
        desmontar_memoria(endereco_base, posicoes_carregadas, log_execucao_ss);
        log_execucao_ss << "\n\n" << std::string(50, '=') << "\n\n";
        executar_simulacao(endereco_base, log_execucao_ss, modo_debug);
    }
    
    // 5. Salvar o Log em um arquivo
    std::ofstream arquivo_log(nome_arquivo_log);
    if (!arquivo_log.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de log '" << nome_arquivo_log << "'" << std::endl;
        std::cout << "\n--- LOG DA EXECUCAO ---\n" << log_execucao_ss.str() << std::endl;
    } else {
        arquivo_log << log_execucao_ss.str();
        arquivo_log.close();
        if (!modo_dump) {
            std::cout << "Execucao concluida. Log salvo em: " << nome_arquivo_log << std::endl;
        }
    }

    return 0;
}