#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string>
#include <vector>
#include <map>

// --- ESTRUTURAS DO PRÉ-PROCESSADOR ---
struct Macro {
    std::string nome;
    std::vector<std::string> parametros;
    std::vector<std::string> corpo;
};
using TabelaMacros = std::map<std::string, Macro>;


// --- ESTRUTURAS DO MONTADOR E LIGADOR ---
struct Simbolo {
    std::string nome;
    int endereco;
    bool definido;
    bool eh_publico;
    bool eh_externo;
};
using TabelaSimbolos = std::map<std::string, Simbolo>;

struct Instrucao {
    std::string nome;
    int opcode;
    int operandos_esperados;
};
using TabelaInstrucoes = std::map<std::string, Instrucao>;

struct Pendencia {
    std::string simbolo;
    int endereco_a_corrigir;
};
using ListaPendencias = std::vector<Pendencia>;

/**
 * @struct EntradaRelocacao
 * @brief (TABELA DE USOS/EXTERNOS) Informa ao LIGADOR quais endereços
 * devem ser corrigidos com o valor de um símbolo EXTERNO.
 */
struct EntradaRelocacao {
    int endereco_no_codigo;
    std::string simbolo; 
};
using TabelaRelocacao = std::vector<EntradaRelocacao>;

/**
 * @typedef TabelaRelocacaoInterna
 * @brief (NOVO) Informa ao LIGADOR quais endereços de operandos
 * são INTERNOS e precisam ter o 'offset' do módulo somado a eles.
 */
using TabelaRelocacaoInterna = std::vector<int>;


#endif // ESTRUTURAS_H