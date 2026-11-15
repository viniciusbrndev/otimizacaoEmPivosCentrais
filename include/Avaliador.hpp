#pragma once

class Instancia;
class Solucao;

// Avaliação "do zero" de uma solução: percorre a matriz de ligação e NÃO
// altera nada. É a fonte de verdade para conferir os valores que a Solucao
// mantém incrementalmente.
class Avaliador {
  public:
    // Tolerância para comparar números reais (água acumulada, custos).
    static constexpr double kTolerancia = 1e-9;

    explicit Avaliador(const Instancia& inst) : inst_(inst) {}

    double custoEnergia(const Solucao& s) const;     // Σ potência·tarifa·ligado
    double custoIntervalos(const Solucao& s) const;  // Σ potência·tarifa·início
    double calculaFO(const Solucao& s) const;        // energia + intervalos

    double calculaGastoAguaHora(int t, const Solucao& s) const;
    int demandaRestantePivo(int pivo, const Solucao& s) const;

    bool horasNecessarias(const Solucao& s) const;   // todas as demandas atendidas
    bool validaSolucao(const Solucao& s) const;      // demanda + água (não altera s)

    static bool estourouVazao(double usoDeAguaHr, double vMax);

  private:
    const Instancia& inst_;
};
