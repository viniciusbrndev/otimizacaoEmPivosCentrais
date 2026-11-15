#include "BuscaLocal.hpp"

Solucao BuscaLocal::localSearch(Solucao solucao) {
    const int horas = solucao.nHoras();
    const int nPivos = solucao.nPivos();

    for (int t = 0; t < horas; t++) {
        bool melhorou = true;
        int it = 0;

        // Critério de parada: nenhum pivô melhora ou limite de iterações
        while (melhorou && it < maxIteracoes_) {
            melhorou = false;
            it++;

            for (int p = 0; p < nPivos; p++) {
                if (vizinhanca_.tentar(solucao, p, t)) {
                    melhorou = true;
                }
            }
        }
    }

    // Remove o erro de arredondamento acumulado pelas atualizações incrementais
    solucao.recalcular();
    return solucao;
}
