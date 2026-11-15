#pragma once

#include <random>

#include "Instancia.hpp"
#include "Solucao.hpp"

// Heurísticas construtivas. Só constrói: avaliação ficou no Avaliador e a
// busca local em BuscaLocal/Vizinhanca.
class Construtor {
  public:
    // `rng` é compartilhado por referência (semente padrão mantida em main).
    Construtor(const Instancia& inst, std::mt19937& rng);

    // Horas da mais barata para a mais cara; pivôs em ordem de índice.
    Solucao solucaoGulosa() const;

    // Guloso por maior demanda restante (adaptativo). Equivale a
    // solucaoGulosaParcial(0).
    Solucao solucaoGulosaMelhorada();

    // GRASP: RCL por limiar sobre a demanda restante, recalculada a cada
    // sorteio. alpha = 0 -> guloso puro; alpha = 1 -> aleatório.
    Solucao solucaoGulosaParcial(float alpha);

  private:
    const Instancia& inst_;
    std::mt19937& rng_;
};
