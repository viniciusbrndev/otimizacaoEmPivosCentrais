#include "Construtor.hpp"

#include <algorithm>
#include <utility>
#include <vector>

#include "Avaliador.hpp"

Construtor::Construtor(const Instancia& inst, std::mt19937& rng)
    : inst_(inst), rng_(rng) {}

Solucao Construtor::solucaoGulosa() const {
    Solucao sol(inst_);
    const int n = inst_.getNPivos();
    const int h = inst_.getNHorasHorizonte();
    const double limiteAgua = inst_.getLimiteAguaHora();

    // (tarifa, hora) ordenado do mais barato para o mais caro
    std::vector<std::pair<double, int>> horas(h);
    for (int t = 0; t < h; t++) {
        horas[t] = {inst_.getCustoEnergiaNaHora(t), t};
    }
    std::sort(horas.begin(), horas.end());

    for (int i = 0; i < h; i++) {
        const int t = horas[i].second;
        for (int p = 0; p < n; p++) {
            if (sol.demandaRestante(p) > 0 &&
                !Avaliador::estourouVazao(
                    sol.gastoAgua(t) + inst_.getGastoWChoraPivo(p), limiteAgua)) {
                sol.ligar(p, t);
            }
        }
    }
    return sol;
}

Solucao Construtor::solucaoGulosaMelhorada() {
    return this->solucaoGulosaParcial(0.0f);
}

Solucao Construtor::solucaoGulosaParcial(float alpha) {
    alpha = std::clamp(alpha, 0.0f, 1.0f);

    Solucao sol(inst_);
    const int n = inst_.getNPivos();
    const int h = inst_.getNHorasHorizonte();
    const double limiteAgua = inst_.getLimiteAguaHora();

    for (int t = 0; t < h; t++) {
        // Candidatos da hora: só pivôs com demanda restante > 0
        std::vector<int> candidatos;
        for (int i = 0; i < n; i++) {
            if (sol.demandaRestante(i) > 0) candidatos.push_back(i);
        }
        // Maior demanda restante primeiro; desempate por índice (determinístico)
        std::sort(candidatos.begin(), candidatos.end(), [&](int a, int b) {
            const int da = sol.demandaRestante(a);
            const int db = sol.demandaRestante(b);
            if (da != db) return da > db;
            return a < b;
        });

        while (!candidatos.empty()) {
            const int maxD = sol.demandaRestante(candidatos.front());
            const int minD = sol.demandaRestante(candidatos.back());
            const double corte = maxD - alpha * (maxD - minD);

            // RCL = prefixo dos candidatos que passa no corte
            int tam = 0;
            while (tam < static_cast<int>(candidatos.size()) &&
                   sol.demandaRestante(candidatos[tam]) >= corte) {
                tam++;
            }
            // alpha = 0: sem sorteio, idêntico ao guloso (mesmo com empates)
            if (alpha <= 0.0f) tam = 1;

            int pos = 0;
            if (tam > 1) {
                std::uniform_int_distribution<int> dist(0, tam - 1);
                pos = dist(rng_);
            }
            const int p = candidatos[pos];

            if (!Avaliador::estourouVazao(
                    sol.gastoAgua(t) + inst_.getGastoWChoraPivo(p), limiteAgua)) {
                sol.ligar(p, t);
            }
            // Remove o sorteado (coube ou não), preservando a ordem
            candidatos.erase(candidatos.begin() + pos);
        }
    }
    return sol;
}
