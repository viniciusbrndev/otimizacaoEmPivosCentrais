#include "../include/Construtor.hpp"
#include "../include/Instancia.hpp"
#include "../include/Solucao.hpp"
#include <vector>

Construtor::Construtor(Instancia* inst){
    this->inst = inst;
}
bool Construtor::horasNecessaria(Solucao& sol){
    int atendeuDemanda;
    for(int i = 0; i < this->inst->getNPivos(); i++){
        atendeuDemanda = 0;
        for(int j = 0; j < this->inst->getNHorasHorizonte(); j++){
            if(sol.ligacao[i][j])
                atendeuDemanda++;
        }
        if(atendeuDemanda != this->inst->getDemandaDoPivo(i))
            return false;
    }
    return true;
}
double Construtor::calculaGastoHora(int t, Solucao& sol){
    double soma = 0;
    for(int i = 0; i < this->inst->getNPivos();i++){
        if(sol.ligacao[i][t])
            soma += this->inst->getDemandaDoPivo(i);
    }
}
bool Construtor::validaSolucao(Solucao& sol){
    sol.viavel = this->horasNecessaria(sol);
    if(sol.viavel){
        
    }
    double gastoAgua;
    for(int j = 0; j<this->inst->getNHorasHorizonte();j++){
        gastoAgua = calculaGastoHora(j,sol);
        if(gastoAgua > this->inst->getLimiteAguaHora())
            sol.viavel = false;
    }
    return sol.viavel;
}
Solucao Construtor::solucaoGulosa(){
    
}
double Construtor::calculaCustoEnergia(const Solucao& sol){}

