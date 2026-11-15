#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

#include "BuscaLocal.hpp"
#include "Construtor.hpp"
#include "Instancia.hpp"
#include "Solucao.hpp"
#include "Vizinhanca.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <instancia> [alpha]\n";
        return 1;
    }

    const std::string caminho = Instancia::formataNome(argv[1]);
    if (caminho.empty()) {
        std::cout << "Arquivo não encontrado\n";
        return 1;
    }

    try {
        Instancia inst(caminho);
        inst.imprimeInst();

        std::mt19937 rng;

        Construtor construtor(inst, rng);
        Solucao solPossivel = (argc == 3)
            ? construtor.solucaoGulosaParcial(std::atof(argv[2]))
            : construtor.solucaoGulosaMelhorada();

        ShiftAleatorio shift(rng);
        BuscaLocal refinamento(shift, 100);
        solPossivel = refinamento.localSearch(solPossivel);

        solPossivel.print();
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
