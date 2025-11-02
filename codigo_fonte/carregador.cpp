/**
 * @file carregador.cpp
 * @brief Implementação de um Carregador (Loader) com offset.
 *
 * Este programa simula um carregador. Ele lê um arquivo de código objeto
 * e o "carrega" em um vetor que simula a memória principal do computador.
 * Aceita um argumento opcional para definir o endereço base do carregamento.
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Define um tamanho máximo para nossa memória simulada
// (Aumentado para permitir offsets grandes)
const int TAMANHO_MEMORIA = 10000; 

/**
 * @brief Imprime o estado atual da memória simulada.
 * Imprime apenas as posições que foram preenchidas.
 * @param memoria O vetor que simula a memória.
 * @param posicoes_carregadas O número de "palavras" carregadas do arquivo.
 * @param endereco_base O endereço inicial onde o carregamento começou.
 */
void imprimir_memoria(const std::vector<int>& memoria, int posicoes_carregadas, int endereco_base) {
    std::cout << "--- ESTADO DA MEMORIA ---" << std::endl;
    std::cout << "(Carregadas " << posicoes_carregadas << " posicoes a partir do endereco " << endereco_base << ")" << std::endl;
    
    for (int i = 0; i < posicoes_carregadas; ++i) {
        int endereco_absoluto = i + endereco_base;
        // Imprime em formato [Endereco]: Valor
        std::cout << "[" << endereco_absoluto << "]: " << memoria[endereco_absoluto] << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Programa carregado com sucesso." << std::endl;
}

/**
 * @brief Função principal do Carregador.
 */
int main(int argc, char *argv[]) {
    // 1. Validar Argumentos
    // Uso: ./carregador <nome_do_arquivo.o2> [endereco_base]
    if (argc < 2 || argc > 3) {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        std::cerr << "Uso: ./carregador <arquivo_objeto> [endereco_base_opcional]" << std::endl;
        return 1;
    }

    std::string nome_arquivo_o2 = argv[1];
    int endereco_base = 0; // Padrão é carregar em 0

    // Verifica se o endereço base opcional foi fornecido
    if (argc == 3) {
        try {
            endereco_base = std::stoi(argv[2]);
        } catch (const std::exception& e) {
            std::cerr << "Erro: Endereco base '" << argv[2] << "' invalido. Deve ser um numero." << std::endl;
            return 1;
        }
        if (endereco_base < 0) {
             std::cerr << "Erro: Endereco base nao pode ser negativo." << std::endl;
            return 1;
        }
    }

    // 2. Simular a Memória
    std::vector<int> memoria_simulada(TAMANHO_MEMORIA, 0);

    // 3. Abrir o arquivo .o2
    std::ifstream arquivo_objeto(nome_arquivo_o2);
    if (!arquivo_objeto.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << nome_arquivo_o2 << "'" << std::endl;
        return 1;
    }

    std::cout << "Carregando arquivo '" << nome_arquivo_o2 << "' na memoria a partir do endereco " << endereco_base << "..." << std::endl;

    // 4. Ler o arquivo e "Carregar" na Memória com offset
    int valor;
    int contador_posicao = 0;
    
    while (arquivo_objeto >> valor) {
        int endereco_absoluto = contador_posicao + endereco_base;

        if (endereco_absoluto >= TAMANHO_MEMORIA) {
            std::cerr << "Erro: Estouro de memoria! O programa (comecando em " << endereco_base 
                      << ") e maior que " << TAMANHO_MEMORIA << " posicoes." << std::endl;
            return 1;
        }
        
        // "Carrega" o valor na memória simulada
        memoria_simulada[endereco_absoluto] = valor;
        contador_posicao++;
    }

    arquivo_objeto.close();

    // 5. Validar e Imprimir
    if (contador_posicao == 0) {
        std::cout << "Aviso: O arquivo '" << nome_arquivo_o2 << "' esta vazio. Nada foi carregado." << std::endl;
    } else {
        imprimir_memoria(memoria_simulada, contador_posicao, endereco_base);
    }

    return 0;
}