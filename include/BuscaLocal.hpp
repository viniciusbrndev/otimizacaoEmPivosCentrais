#pragma once

#include "Solucao.hpp"
#include "Vizinhanca.hpp"

class BuscaLocal {
  public:
    explicit BuscaLocal(Vizinhanca& vizinhanca, int maxIteracoes = 100)
        : vizinhanca_(vizinhanca), maxIteracoes_(maxIteracoes) {}

    // Recebe a solução por valor: a original não é alterada.
    Solucao localSearch(Solucao solucao);
    
  private:
    Vizinhanca& vizinhanca_;
    int maxIteracoes_;
};
