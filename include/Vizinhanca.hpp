#pragma once

#include <random>

#include "Solucao.hpp"

// Um tipo de movimento da busca local.
class Vizinhanca {
  public:
    virtual ~Vizinhanca() = default;

    virtual bool tentar(Solucao& sol, int pivo, int hora) = 0;
};


class ShiftAleatorio : public Vizinhanca {
  public:
    explicit ShiftAleatorio(std::mt19937& rng) : rng_(rng) {}

    bool tentar(Solucao& sol, int pivo, int hora) override;


    int randPos(const Solucao& sol, int pivo, int horaAtual);

  private:
    std::mt19937& rng_;
};
class BestImprovement : public Vizinhanca{
  public:
    explicit BestImprovement(){}
    
    bool tentar(Solucao& sol,  int pivo, int hora) override;
    bool buscaMelhora(Solucao& sol, int pivo);
};