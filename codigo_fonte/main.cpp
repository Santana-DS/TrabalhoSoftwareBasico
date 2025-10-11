#include <iostream> // Equivalente ao <stdio.h> para entrada e saída no console
#include <fstream>  // Para manipulação de arquivos (file stream)
#include <string>   // Para usar o tipo de dado std::string
#include <vector>   // Para usar listas dinâmicas (vetores)

int main(int argc, char *argv[]) {
    // 1. Verifica se o número de argumentos está correto
    if (argc != 2) {
        std::cerr << "Erro: Formato de execucao invalido." << std::endl;
        std::cerr << "Uso: ./compilador <nome_do_arquivo.asm>" << std::endl;
        return 1; // Retorna 1 para indicar erro
    }

    // 2. Tenta abrir o arquivo de entrada
    std::string nome_arquivo_entrada = argv[1];
    std::ifstream arquivo_entrada(nome_arquivo_entrada); // ifstream = input file stream

    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivo_entrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo '" << nome_arquivo_entrada << "'" << std::endl;
        return 1;
    }

    std::cout << "Arquivo '" << nome_arquivo_entrada << "' aberto com sucesso. Conteudo:" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    // 3. Lê o conteúdo do arquivo e armazena em um vetor de strings
    std::string linha;
    std::vector<std::string> linhas_do_arquivo;
    while (std::getline(arquivo_entrada, linha)) {
        linhas_do_arquivo.push_back(linha);
    }

    // 4. Imprime o conteúdo que foi armazenado
    for (int i = 0; i < linhas_do_arquivo.size(); ++i) {
        std::cout << i + 1 << ": " << linhas_do_arquivo[i] << std::endl;
    }

    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Leitura concluida." << std::endl;

    // 5. Fecha o arquivo (em C++, isso acontece automaticamente quando a variável 'arquivo_entrada' sai de escopo, mas é bom saber)
    arquivo_entrada.close();

    return 0; // Retorna 0 para indicar sucesso
}