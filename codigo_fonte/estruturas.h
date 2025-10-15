#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string>
#include <vector>
#include <map>

// --- ESTRUTURAS DO PRÉ-PROCESSADOR ---

/**
 * @struct Macro
 * @brief Armazena a definição completa de uma macro.
 * Contém o nome da macro, a lista de seus parâmetros formais (ex: &ARG1) e
 * as linhas de código que compõem seu corpo.
 */
struct Macro {
    std::string nome;
    std::vector<std::string> parametros;
    std::vector<std::string> corpo;
};
// 'using' cria um apelido para um tipo de dado. Aqui, TabelaMacros é um nome mais
// legível para um mapa que associa o nome de uma macro (string) à sua definição (Macro).
// Funciona como uma combinação das tabelas MNT e MDT.
using TabelaMacros = std::map<std::string, Macro>;


// --- ESTRUTURAS DO MONTADOR ---

/**
 * @struct Simbolo
 * @brief Representa uma entrada na Tabela de Símbolos (TS).
 * Armazena o nome de um rótulo, o endereço de memória onde ele foi definido
 * e um booleano para controle de definição.
 */
struct Simbolo {
    std::string nome;
    int endereco;
    bool definido;
};
// Apelido para a Tabela de Símbolos, implementada como um mapa para busca rápida.
using TabelaSimbolos = std::map<std::string, Simbolo>;

/**
 * @struct Instrucao
 * @brief Representa uma entrada na Tabela de Instruções (TI).
 * Contém o opcode numérico da instrução e quantos operandos ela espera.
 */
struct Instrucao {
    std::string nome;
    int opcode;
    int operandos_esperados;
};
// Apelido para a Tabela de Instruções, implementada como um mapa.
using TabelaInstrucoes = std::map<std::string, Instrucao>;

/**
 * @struct Pendencia
 * @brief Representa uma entrada na Lista de Pendências (LP).
 * Usada para resolver referências a rótulos definidos posteriormente no código.
 * @param simbolo O nome do rótulo que precisa ter seu endereço descoberto.
 * @param endereco_a_corrigir O índice no vetor de código objeto que deve ser preenchido.
 */
struct Pendencia {
    std::string simbolo;
    int endereco_a_corrigir;
};
// Apelido para a Lista de Pendências, implementada como um vetor.
using ListaPendencias = std::vector<Pendencia>;

#endif // ESTRUTURAS_H