#include "Vizinhanca.hpp"
#include "Avaliador.hpp"

#include <vector>
#include <algorithm>

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
bool BestImprovement::buscaMelhora(Solucao& sol, int pivo) {
    std::vector<int> candidatos;
    std::vector<int> vazios;

    for (int i = 0; i < sol.nHoras(); i++) {
        if (sol.ligado(pivo, i)) {
            candidatos.push_back(i);
        } else {
            vazios.push_back(i);
        }
    }

    if (candidatos.empty() || vazios.empty())
        return false;

    double melhorCusto = sol.funcObjetivo();
    int melhorDe = -1;
    int melhorPara = -1;

    // Reutiliza uma única cópia temporária 
    Solucao vizinho = sol;

    for (int de : candidatos) {
        for (int para : vazios) {
            vizinho = sol; 
            vizinho.mover(pivo, de, para);

            if (vizinho.viavel() && vizinho.funcObjetivo() < melhorCusto) {
                melhorCusto = vizinho.funcObjetivo();
                melhorDe = de;
                melhorPara = para;
            }
        }
    }

    if (melhorDe == -1)
        return false;

    sol.mover(pivo, melhorDe, melhorPara);
    return true;
}
bool BestImprovement::tentar(Solucao& sol,  int pivo, int hora){
    return buscaMelhora(sol,  pivo);
}