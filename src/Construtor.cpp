#include "Construtor.hpp"
#include "Instancia.hpp"
#include "Solucao.hpp"
#include <vector>
#include <algorithm>

#define MAX_ITERACOES 100
#define PENALIDADE_AGUA 200
#define PENALIDADE_DEMANDA 100

Construtor::Construtor(Instancia* inst){
    this->inst = inst;
}
//Vertifica se o pivô teve sua demanda de horas ligado atingida
bool Construtor::horasNecessaria(Solucao& sol) const{
    int atendeuDemanda;
    for(int i = 0; i < this->inst->getNPivos(); i++){
        atendeuDemanda = this->inst->getDemandaHorasPivo(i);
        for(int j = 0; j < this->inst->getNHorasHorizonte(); j++){
            if(sol.ligacao[i][j])
                atendeuDemanda--;
        }
        if(atendeuDemanda != 0)
            return false;
    }
    return true;
}
//Calcula a água gasta na hora (coluna da matriz)
int Construtor::demandaRestantePivo(int pivo,const Solucao& so) const{
    int atendeuDemanda = this->inst->getDemandaHorasPivo(pivo);
    for(int j = 0; j < this->inst->getNHorasHorizonte(); j++){
        if(so.ligacao[pivo][j])
            atendeuDemanda--;
    }

    return atendeuDemanda;
}
double Construtor::calculaGastoAguaHora(int t, const std::vector<std::vector<int>>& mat)const{
    double soma = 0;
    for(int i = 0; i < this->inst->getNPivos();i++){
        if(mat[i][t])
            soma += this->inst->getGastoWChoraPivo(i);
    }
    return soma;
}
bool Construtor::validaSolucao(Solucao& sol){
    sol.viavel = this->horasNecessaria(sol);
    if(sol.viavel){
        double gastoAgua;
        for(int j = 0; j<this->inst->getNHorasHorizonte();j++){
            gastoAgua = calculaGastoAguaHora(j,sol.ligacao);
            if(gastoAgua > this->inst->getLimiteAguaHora())
                sol.viavel = false;
        }
    } 
    return sol.viavel;
}
bool Construtor::estourouVazao(double usoDeAguaHr, double vMax){
    if(usoDeAguaHr > vMax)
        return true;
    return false;
}
double Construtor::calculaCustoEnergia(const std::vector<std::vector<int>>& mat)const{
    double soma = 0.0;
    int nHoras = inst->getNHorasHorizonte();
    int nPivos = inst->getNPivos();

    for (int i = 0; i < nPivos; i++) {
        for (int t = 0; t < nHoras; t++) {
            soma += inst->getGastoECPivo(i) * inst->getCustoEnergiaNaHora(t) * mat[i][t];
        }
    }
    return soma;
}
int Construtor::randPos(int pivo, int horaAtual, const Solucao& sol){
    int h = inst->getNHorasHorizonte();
    int estadoAtual = sol.ligacao[pivo][horaAtual]; // 1 se ligado, 0 se desligado
    int estadoDesejado = (estadoAtual == 1) ? 0 : 1; // Queremos o estado oposto para k

    std::vector<int> candidatos;
    for (int k = 0; k < h; k++) {
        if (k != horaAtual && sol.ligacao[pivo][k] == estadoDesejado) {
            candidatos.push_back(k);
        }
    }

    if (candidatos.empty()) {
        return -1; // Não há posições válidas para trocar
    }

    int idxSorteado = rand() % candidatos.size();
    return candidatos[idxSorteado];
}
bool Construtor::randonSwap(int pivo, int t, Solucao& s){
    int k = randPos(pivo, t, s);
    if (k == -1) {
        return false; // Não foi possível realizar o swap
    }

    if(s.ligacao[pivo][t] == 1){
        s.ligacao[pivo][k] = 1;
        s.ligacao[pivo][t] = 0;
    } else {
        s.ligacao[pivo][k] = 0;
        s.ligacao[pivo][t] = 1;
    }
    return true;
}
//método visa apenas alocar os horários respeitando o limite de água
Solucao& Construtor::solucaoGulosa(){
    static Solucao solPossivel(*inst);
    int n = this->inst->getNPivos();
    int h = this->inst->getNHorasHorizonte();

    solPossivel = Solucao(*inst);
    std::vector<double> gastoDeAgua(h, 0.0);
    std::vector<int> demandaRestante(n);
    std::vector<std::pair<double, int>> horas(h);
    
    for(int i = 0; i< n; i++){
        demandaRestante[i] = this->inst->getDemandaHorasPivo(i);
    }
    for(int t = 0; t < h; t++){
        horas[t].first = this->inst->getCustoEnergiaNaHora(t);
        horas[t].second = t;
    }
    std::sort(horas.begin(),horas.end());

    //ESCOLHA DA ALOCACAO
    for(int t = 0; t < h; t++){
        for(int j = 0; j < n; j++){
            if(demandaRestante[j] > 0){

                double gastoNovo = gastoDeAgua[horas[t].second] + this->inst->getGastoWChoraPivo(j);
                if(!estourouVazao(gastoNovo, inst->getLimiteAguaHora())){
                    solPossivel.ligacao[j][horas[t].second] = 1;//liga o pivo naquela hora
                    demandaRestante[j]--;
                    gastoDeAgua[horas[t].second] = gastoNovo;
                }
            }
        }
    }
    solPossivel.custoTotalEnergia = calculaCustoEnergia(solPossivel.ligacao);
    solPossivel.viavel = validaSolucao(solPossivel);
    return solPossivel;

}
double Construtor::calculaFO(const Solucao& s) const{
 
    double custoEnergiaTotal = s.custoTotalEnergia;
    double custoLigacoes = calculaCustoEnergia(s.intervalo);

    return custoEnergiaTotal + custoLigacoes ;
} 
Solucao Construtor::solucaoGulosaMelhorada(){
    Solucao solPossivel(*inst);
    
    int n = this->inst->getNPivos();
    int h = this->inst->getNHorasHorizonte();
    std::vector<double> gastoHora(h, 0.0);
    
    std::vector<std::pair<int, int>> pivos(n);
    for(int i = 0 ; i < n; i++){
        pivos[i].first = i;
        pivos[i].second = inst->getDemandaHorasPivo(i);
    }
    std::sort(pivos.begin(), pivos.end(), [](const auto& a, const auto& b){
        return a.second > b.second;
    });
    for (int i = 0; i < n; i++) {
        int pivoId = pivos[i].first;
        int demandaRestante = pivos[i].second;
        double gastoPivo = inst->getGastoWChoraPivo(pivoId);

        for (int j = 0; j < h && demandaRestante > 0; j++) {
            // Verifica se cabe o pivô na hora j sem estourar o limite de água
            if (gastoHora[j] + gastoPivo <= inst->getLimiteAguaHora()) {
                solPossivel.ligacao[pivoId][j] = 1;
                gastoHora[j] += gastoPivo;
                // Decrementa a demanda
                demandaRestante--; 
            }
        }
    }
    solPossivel.custoTotalEnergia = calculaCustoEnergia(solPossivel.ligacao);
    solPossivel.atualizarIntervalos();
    solPossivel.funcObjetivo = calculaFO(solPossivel);
    solPossivel.viavel = validaSolucao(solPossivel);
    return solPossivel;
}

Solucao Construtor::metodoDeRefinamento(Solucao& solucao){
    Solucao solucaoAtual = solucao;
    Solucao solucaoAntiga = solucaoAtual;
    int horas = inst->getNHorasHorizonte();
    int nPivos = inst->getNPivos();

    for (int i = 0; i < horas; i++) {
        solucaoAntiga = solucaoAtual;
        double custoAntigo = calculaCustoEnergia(solucaoAntiga.ligacao);

        bool melhorou = true;
        int iteracoes = 0;
        // Critério de parada da busca local

        while (melhorou && iteracoes < MAX_ITERACOES) {
            melhorou = false;
            iteracoes++;

            for (int j = 0; j < nPivos; j++) {
                Solucao solucaoNova = solucaoAntiga;
                //inverte os pivos
                if (!randonSwap(j, i, solucaoNova)) {
                    continue;
                }

                double custoNovo = calculaCustoEnergia(solucaoNova.ligacao);
                double gastoNovo = calculaGastoAguaHora(i, solucaoNova.ligacao);

                if (custoNovo <= custoAntigo && !estourouVazao(gastoNovo, inst->getLimiteAguaHora()) && validaSolucao(solucaoNova)) {
                    solucaoAntiga = solucaoNova;
                    custoAntigo = custoNovo;
                    melhorou = true;
                }
            }
        }

        solucaoAtual = solucaoAntiga;
        solucaoAtual.custoTotalEnergia = custoAntigo;
    }

    solucaoAtual.viavel = validaSolucao(solucaoAtual);
    return solucaoAtual;
}