#include "Avaliador.hpp"

#include "Instancia.hpp"
#include "Solucao.hpp"

double Avaliador::custoEnergia(const Solucao& s) const {
    double soma = 0.0;
    for (int p = 0; p < s.nPivos(); p++) {
        for (int t = 0; t < s.nHoras(); t++) {
            if (s.ligado(p, t)) {
                soma += inst_.custoEnergiaPivoHora(p, t);
            }
        }
    }
    return soma;
}

// Cada início de intervalo (hora ligada cuja anterior está desligada, ou
// t == 0) paga potência × tarifa da hora de início.
double Avaliador::custoIntervalos(const Solucao& s) const {
    double soma = 0.0;
    for (int p = 0; p < s.nPivos(); p++) {
        for (int t = 0; t < s.nHoras(); t++) {
            if (s.ligado(p, t) && (t == 0 || !s.ligado(p, t - 1))) {
                soma += inst_.custoEnergiaPivoHora(p, t);
            }
        }
    }
    return soma;
}

double Avaliador::calculaFO(const Solucao& s) const {
    return custoEnergia(s) + custoIntervalos(s);
}

double Avaliador::calculaGastoAguaHora(int t, const Solucao& s) const {
    double soma = 0.0;
    for (int p = 0; p < s.nPivos(); p++) {
        if (s.ligado(p, t)) {
            soma += inst_.getGastoWChoraPivo(p);
        }
    }
    return soma;
}

int Avaliador::demandaRestantePivo(int pivo, const Solucao& s) const {
    int restante = inst_.getDemandaHorasPivo(pivo);
    for (int t = 0; t < s.nHoras(); t++) {
        if (s.ligado(pivo, t)) {
            restante--;
        }
    }
    return restante;
}

bool Avaliador::horasNecessarias(const Solucao& s) const {
    for (int p = 0; p < s.nPivos(); p++) {
        if (demandaRestantePivo(p, s) != 0) {
            return false;
        }
    }
    return true;
}

bool Avaliador::validaSolucao(const Solucao& s) const {
    if (!horasNecessarias(s)) {
        return false;
    }
    for (int t = 0; t < s.nHoras(); t++) {
        if (estourouVazao(calculaGastoAguaHora(t, s), inst_.getLimiteAguaHora())) {
            return false;
        }
    }
    return true;
}

bool Avaliador::estourouVazao(double usoDeAguaHr, double vMax) {
    return usoDeAguaHr - vMax > kTolerancia;
}
