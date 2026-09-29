#ifndef SOLUCAO_H
#define SOLUCAO_H
#include "Instancia.hpp"


struct Solucao{
    // Matriz de decisão: ligacao[p][t] = 1 se o pivo p liga na hora t
    std::vector<std::vector<int>> ligacao; 
    
    double custoTotalEnergia;
    bool viavel;

    // Construtor padrão
    Solucao() : custoTotalEnergia(0.0), viavel(true){}

    // Construtor que inicializa a matriz zerada com base na Instancia
    Solucao(const Instancia& inst) {
        int nPivos = inst.getNPivos();
        int nHoras = inst.getNHorasHorizonte();
        
        ligacao.assign(nPivos, std::vector<int>(nHoras, 0));
        custoTotalEnergia = 0.0;
        viavel = true;
    }

    // Método utilitário 
    void limpar(){
        for (auto& linha : ligacao) {
            std::fill(linha.begin(), linha.end(), 0);
        }
        custoTotalEnergia = 0.0;
        viavel = true;
    }

    void print(const Instancia& inst){
        std::cout << std::endl<<"Eh viavel: "<<viavel<<std::endl<<"Custo Total de Energia: " << custoTotalEnergia << std::endl;
        for(int i = 0; i < inst.getNPivos(); i++){
            for(int j = 0; j < inst.getNHorasHorizonte(); j++){
                std::cout << ligacao[i][j]<<" ";
            }
            std::cout << std::endl;
        }
    }
};

#endif