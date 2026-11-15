#include "Vizinhanca.hpp"

#include <vector>

#include "Avaliador.hpp"

int ShiftAleatorio::randPos(const Solucao& sol, int pivo, int horaAtual) {
    const int h = sol.nHoras();
    const bool estadoDesejado = !sol.ligado(pivo, horaAtual);  // oposto ao atual

    std::vector<int> candidatos;
    for (int k = 0; k < h; k++) {
        if (k != horaAtual && sol.ligado(pivo, k) == estadoDesejado) {
            candidatos.push_back(k);
        }
    }
    if (candidatos.empty()) {
        return -1;  // não há posição válida para trocar
    }

    std::uniform_int_distribution<int> dist(0, static_cast<int>(candidatos.size()) - 1);
    return candidatos[dist(rng_)];
}

bool ShiftAleatorio::tentar(Solucao& sol, int pivo, int hora) {
    const int k = this->randPos(sol, pivo, hora);
    if (k == -1) {
        return false;
    }

    const double foAntes = sol.funcObjetivo();
    const bool ligadoNaHora = sol.ligado(pivo, hora);
    const int de = ligadoNaHora ? hora : k;    // hora ligada que sai
    const int para = ligadoNaHora ? k : hora;  // hora desligada que entra

    sol.mover(pivo, de, para);
    if (sol.viavel() && sol.funcObjetivo() <= foAntes + Avaliador::kTolerancia) {
        return true;
    }
    sol.mover(pivo, para, de);  // desfaz
    return false;
}
