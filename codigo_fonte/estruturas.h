#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <string>
#include <vector>
#include <map>

struct Macro {
    std::string nome;
    std::vector<std::string> parametros;
    std::vector<std::string> corpo;
};
using TabelaMacros = std::map<std::string, Macro>;

struct Simbolo {
    std::string nome;
    int endereco;
    bool definido;
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

#endif // ESTRUTURAS_H