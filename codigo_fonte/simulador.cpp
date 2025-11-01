/**
 * @file simulador.cpp
 * @brief Implementação de um Carregador e Simulador de CPU (com Log).
 *
 * Esta versão salva o *resultado da execução* (OUTPUTs e erros)
 * em um arquivo de log (.log) em vez de no console.
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "utilitarios.h" 

// Tamanho da memória simulada
const int TAMANHO_MEMORIA = 10000; 

// --- Variáveis Globais da CPU ---
std::vector<int> memoria_simulada(TAMANHO_MEMORIA, 0);
int ACC = 0; // Acumulador
int PC = 0;  // Contador de Programa

/**
 * @brief Imprime a mensagem de ajuda para o simulador.
 */
void imprimir_ajuda_simulador() {
    std::cout << "--- Simulador (simulador.exe) ---" << std::endl;
    std::cout << "Carrega e executa um arquivo de codigo objeto (.o2 ou .exe)." << std::endl;
    std::cout << "\nUSO:" << std::endl;
    std::cout << "  ./simulador.exe <arquivo_objeto> [endereco_base]" << std::endl;
    std::cout << "  ./simulador.exe -h ou --help" << std::endl;
    std::cout << "\nARGUMENTOS:" << std::endl;
    std::cout << "  <arquivo_objeto>: (Obrigatorio) O arquivo .o2 ou .exe a ser executado." << std::endl;
    std::cout << "  [endereco_base]:  (Opcional) O endereco de memoria onde o programa" << std::endl;
    std::cout << "                    sera carregado. Padrao: 0" << std::endl;
    std::cout << "\nSAIDA:" << std::endl;
    std::cout << "  O log da execucao (qualquer OUTPUT ou erro) sera salvo em" << std::endl;
    std::cout << "  um arquivo <nome_do_arquivo>.log na pasta raiz." << std::endl;
}

/**
 * @brief Carrega o programa na memória simulada.
 */
int carregar_programa(const std::string& nome_arquivo, int endereco_base) {
    // ... (O restante da função permanece exatamente o mesmo) ...
    std::ifstream arquivo_objeto(nome_arquivo);
    if (!arquivo_objeto.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << nome_arquivo << "'" << std::endl;
        return -1;
    }
    std::cout << "Carregando arquivo '" << nome_arquivo << "' na memoria a partir do endereco " << endereco_base << "..." << std::endl;
    int valor;
    int contador_posicao = 0;
    while (arquivo_objeto >> valor) {
        int endereco_absoluto = contador_posicao + endereco_base;
        if (endereco_absoluto >= TAMANHO_MEMORIA) {
            std::cerr << "Erro: Estouro de memoria!" << std::endl;
            return -1;
        }
        memoria_simulada[endereco_absoluto] = valor;
        contador_posicao++;
    }
    arquivo_objeto.close();
    if (contador_posicao == 0) {
        std::cout << "Aviso: O arquivo '" << nome_arquivo << "' esta vazio." << std::endl;
    }
    std::cout << "Programa carregado com sucesso (" << contador_posicao << " posicoes)." << std::endl;
    return contador_posicao;
}

/**
 * @brief Executa o código carregado na memória.
 */
void executar_simulacao(int endereco_base, std::stringstream& log_ss) {
    // ... (O restante da função permanece exatamente o mesmo) ...
    PC = endereco_base; 
    bool executando = true;
    log_ss << "--- INICIANDO EXECUCAO ---" << std::endl;
    while (executando) {
        if (PC < 0 || PC >= TAMANHO_MEMORIA) {
            log_ss << "Erro de Execucao: Contador de Programa (PC) fora dos limites da memoria (" << PC << ")" << std::endl;
            executando = false;
            continue;
        }
        int opcode = memoria_simulada[PC];
        int operando1, operando2;
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
    // --- INÍCIO DA CORREÇÃO ---
    // 1. Validar Argumentos
    if (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")) {
        imprimir_ajuda_simulador();
        return 0;
    }
    
    // Uso: ./simulador <arquivo_objeto> [endereco_base_opcional]
    if (argc < 2 || argc > 3) {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        imprimir_ajuda_simulador();
        return 1;
    }
    // --- FIM DA CORREÇÃO ---

    std::string nome_arquivo_exec = argv[1];
    int endereco_base = 0; 

    if (argc == 3) {
        try {
            endereco_base = std::stoi(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Erro: Endereco base '" << argv[2] << "' invalido." << std::endl;
            return 1;
        }
    }

    // 2. Carregar o Programa
    int posicoes_carregadas = carregar_programa(nome_arquivo_exec, endereco_base);
    if (posicoes_carregadas <= 0) {
        std::cerr << "Falha no carregamento. Simulacao abortada." << std::endl;
        return 1;
    }

    // 3. Preparar o Log de Execução
    std::stringstream log_execucao_ss;
    
    // 4. Executar a Simulação
    executar_simulacao(endereco_base, log_execucao_ss);
    
    // 5. Salvar o Log em um arquivo
    std::string nome_arquivo_log = obter_nome_base(nome_arquivo_exec) + ".log";
    
    std::ofstream arquivo_log(nome_arquivo_log);
    if (!arquivo_log.is_open()) {
        std::cerr << "Erro: Nao foi possivel criar o arquivo de log '" << nome_arquivo_log << "'" << std::endl;
        std::cout << "\n--- LOG DA EXECUCAO ---\n" << log_execucao_ss.str() << std::endl;
    } else {
        arquivo_log << log_execucao_ss.str();
        arquivo_log.close();
        std::cout << "Execucao concluida. Log salvo em: " << nome_arquivo_log << std::endl;
    }

    return 0;
}