#include "Solucao.hpp"

#include <algorithm>
#include <iostream>

#include "Avaliador.hpp"
#include "Instancia.hpp"

Solucao::Solucao(const Instancia& inst)
    : inst_(&inst),
    ligacao_(inst.getNPivos(), std::vector<int>(inst.   getNHorasHorizonte(), 0)),
    horasLigadas_(inst.getNPivos(), 0),
      gastoAgua_(inst.getNHorasHorizonte(), 0.0) {
    this->recontarContadores();
}

int Solucao::nHoras() const { return inst_->getNHorasHorizonte(); }

bool Solucao::inicioIntervalo(int p, int t) const {
    return ligacao_[p][t] == 1 && (t == 0 || ligacao_[p][t - 1] == 0);
}

int Solucao::demandaRestante(int p) const {
    return inst_->getDemandaHorasPivo(p) - horasLigadas_[p];
}


double Solucao::custoInicioJanela(int p, int t) const {
    double c = 0.0;
    if (inicioIntervalo(p, t)) {
        c += inst_->custoEnergiaPivoHora(p, t);
    }
    if (t + 1 < nHoras() && inicioIntervalo(p, t + 1)) {
        c += inst_->custoEnergiaPivoHora(p, t + 1);
    }
    return c;
}

void Solucao::definir(int p, int t, bool valor) {
    if ((ligacao_[p][t] == 1) == valor) {
        return;
    }

    const double limite = inst_->getLimiteAguaHora();
    const int demanda = inst_->getDemandaHorasPivo(p);
    const double sinal = valor ? 1.0 : -1.0;

    const bool demandaOkAntes = (horasLigadas_[p] == demanda);
    const bool estouroAntes = Avaliador::estourouVazao(gastoAgua_[t], limite);
    const double inicioAntes = custoInicioJanela(p, t);

    ligacao_[p][t] = valor ? 1 : 0;
    horasLigadas_[p] += valor ? 1 : -1;
    gastoAgua_[t] += sinal * inst_->getGastoWChoraPivo(p);
    custoEnergia_ += sinal * inst_->custoEnergiaPivoHora(p, t);
    custoIntervalos_ += custoInicioJanela(p, t) - inicioAntes;

    const bool demandaOkDepois = (horasLigadas_[p] == demanda);
    const bool estouroDepois = Avaliador::estourouVazao(gastoAgua_[t], limite);

    if (demandaOkAntes && !demandaOkDepois) {
        pivosDemandaErrada_++;
    } else if (!demandaOkAntes && demandaOkDepois) {
        pivosDemandaErrada_--;
    }
    if (!estouroAntes && estouroDepois) {
        horasComEstouro_++;
    } else if (estouroAntes && !estouroDepois) {
        horasComEstouro_--;
    }
}

void Solucao::ligar(int p, int t) { this->definir(p, t, true); }
void Solucao::desligar(int p, int t) { this->definir(p, t, false); }

void Solucao::mover(int p, int de, int para) {
    this->desligar(p, de);
    this->ligar(p, para);
}

void Solucao::recontarContadores() {
    pivosDemandaErrada_ = 0;
    for (int p = 0; p < nPivos(); p++) {
        if (horasLigadas_[p] != inst_->getDemandaHorasPivo(p)) {
            pivosDemandaErrada_++;
        }
    }
    horasComEstouro_ = 0;
    for (int t = 0; t < nHoras(); t++) {
        if (Avaliador::estourouVazao(gastoAgua_[t], inst_->getLimiteAguaHora())) {
            horasComEstouro_++;
        }
    }
}

void Solucao::limpar() {
    for (auto& linha : ligacao_) {
        std::fill(linha.begin(), linha.end(), 0);
    }
    std::fill(horasLigadas_.begin(), horasLigadas_.end(), 0);
    std::fill(gastoAgua_.begin(), gastoAgua_.end(), 0.0);
    custoEnergia_ = 0.0;
    custoIntervalos_ = 0.0;
    this->recontarContadores();
}

void Solucao::recalcular() {
    const Avaliador av(*inst_);
    for (int p = 0; p < nPivos(); p++) {
        horasLigadas_[p] = static_cast<int>(
            std::count(ligacao_[p].begin(), ligacao_[p].end(), 1));
    }
    for (int t = 0; t < nHoras(); t++) {
        gastoAgua_[t] = av.calculaGastoAguaHora(t, *this);
    }
    custoEnergia_ = av.custoEnergia(*this);
    custoIntervalos_ = av.custoIntervalos(*this);
    this->recontarContadores();
}

void Solucao::print() const {
    const int n = nPivos();
    if (n == 0) return;
    const int h = nHoras();

    std::cout << std::endl << "Eh viavel: " << viavel()
              << "\nCusto Total de Energia: " << custoEnergia()
              << "\nValor FO: " << funcObjetivo() << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < h; j++) {
            std::cout << ligacao_[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n\nIntervalos de Irrigação\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < h; j++) {
            std::cout << (inicioIntervalo(i, j) ? 1 : 0) << " ";
        }
        std::cout << std::endl;
    }
}
