#include "../include/Construtor.hpp"
#include "../include/Instancia.hpp"
#include "../include/Solucao.hpp"
#include <vector>
#include <algorithm>

Construtor::Construtor(Instancia* inst){
    this->inst = inst;
}
//Vertifica se o pivô teve sua demanda de horas ligado atingida
bool Construtor::horasNecessaria(Solucao& sol){
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
double Construtor::calculaGastoWCHora(int t, Solucao& sol){
    double soma = 0;
    for(int i = 0; i < this->inst->getNPivos();i++){
        if(sol.ligacao[i][t])
            soma += this->inst->getGastoWChoraPivo(i);
    }
    return soma;
}
bool Construtor::validaSolucao(Solucao& sol){
    sol.viavel = this->horasNecessaria(sol);
    if(sol.viavel){
        double gastoAgua;
        for(int j = 0; j<this->inst->getNHorasHorizonte();j++){
            gastoAgua = calculaGastoWCHora(j,sol);
            if(gastoAgua > this->inst->getLimiteAguaHora())
                sol.viavel = false;
        }
    } 
    return sol.viavel;
}
//método visa apenas alocar os horários respeitando o limite de água
Solucao Construtor::solucaoGulosa(){
    Solucao solPossivel(*inst);
    int n = this->inst->getNPivos();
    int h = this->inst->getNHorasHorizonte();

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
                if( gastoDeAgua[horas[t].second] + this->inst->getGastoWChoraPivo(j) <= this->inst->getLimiteAguaHora()){
                    solPossivel.ligacao[j][horas[t].second] = 1;//liga o pivo naquela hora
                    demandaRestante[j]--;
                    gastoDeAgua[horas[t].second] += this->inst->getGastoWChoraPivo(j);
                }
            }
        }
    }
    solPossivel.viavel = validaSolucao(solPossivel);
    return solPossivel;
}
// double Construtor::calculaCustoEnergia(const Solucao& sol){
//     for(int i = 0; i < this->inst->getNPivos();i++){
//         for(int j = 0; j < this->inst->getNHorasHorizonte(); j++){

//         }
//     }
// }

