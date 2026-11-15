#pragma once

#include <vector>

class Instancia;

class Solucao {
  public:
    explicit Solucao(const Instancia& inst);

    // ---- Consultas ----
    int nPivos() const { return static_cast<int>(ligacao_.size()); }
    int nHoras() const;
    bool ligado(int p, int t) const { return ligacao_[p][t] == 1; }
    const std::vector<std::vector<int>>& ligacao() const { return ligacao_; }

    // Início de intervalo: ligado em t e desligado em t-1 (ou t == 0).
    bool inicioIntervalo(int p, int t) const;

    int horasLigadas(int p) const { return horasLigadas_[p]; }
    int demandaRestante(int p) const;
    double gastoAgua(int t) const { return gastoAgua_[t]; }

    double custoEnergia() const { return custoEnergia_; }
    double custoIntervalos() const { return custoIntervalos_; }
    double funcObjetivo() const { return custoEnergia_ + custoIntervalos_; }
    bool viavel() const { return pivosDemandaErrada_ == 0 && horasComEstouro_ == 0; }

    // ---- Alterações ----
    void ligar(int p, int t);      // idempotente
    void desligar(int p, int t);   // idempotente
    // Move uma hora ligada: desliga `de` e liga `para`. A demanda não muda.
    void mover(int p, int de, int para);

    void limpar();
    // Recalcula tudo do zero (via Avaliador). Elimina erro de arredondamento
    // acumulado pelas atualizações incrementais.
    void recalcular();

    void print() const;

  private:
    const Instancia* inst_;
    std::vector<std::vector<int>> ligacao_;
    std::vector<int> horasLigadas_;
    std::vector<double> gastoAgua_;
    double custoEnergia_ = 0.0;
    double custoIntervalos_ = 0.0;
    int pivosDemandaErrada_ = 0;  // pivôs com horasLigadas != demanda
    int horasComEstouro_ = 0;     // horas acima do limite de água

    void definir(int p, int t, bool valor);
    double custoInicioJanela(int p, int t) const;
    void recontarContadores();
};
