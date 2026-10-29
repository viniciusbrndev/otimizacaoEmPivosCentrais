#ifndef SOLUCAO_H
#define SOLUCAO_H
#include "Instancia.hpp"


struct Solucao{
    // Matriz de decisão: ligacao[p][t] = 1 se o pivo p liga na hora t
    std::vector<std::vector<int>> ligacao; 
    std::vector<std::vector<int>> intervalo;

    double custoTotalEnergia;
    double funcObjetivo;
    bool viavel;

    // Construtor padrão
    Solucao() : custoTotalEnergia(0.0), viavel(true), funcObjetivo(0.0){}

    // Construtor que inicializa a matriz zerada com base na Instancia
    Solucao(const Instancia& inst) {
        int nPivos = inst.getNPivos();
        int nHoras = inst.getNHorasHorizonte();
        
        ligacao.assign(nPivos, std::vector<int>(nHoras, 0));
        intervalo.assign(nPivos, std::vector<int>(nHoras, 0));

        custoTotalEnergia = 0.0;
        viavel = true;
        funcObjetivo = 0.0;
    }

    void atualizarIntervalos() {
        int nPivos = ligacao.size();
        if (nPivos == 0) return;
        int nHoras = ligacao[0].size();
        
        intervalo.assign(nPivos, std::vector<int>(nHoras, 0));
        
        for (int i = 0; i < nPivos; i++) {
            for (int t = 0; t < nHoras; t++) {
                if (ligacao[i][t] == 1) {
                    // É o início de um intervalo se for a primeira hora (t == 0) 
                    // ou se na hora anterior o pivô estava desligado (0)
                    if (t == 0 || ligacao[i][t - 1] == 0) {
                        intervalo[i][t] = 1;
                    }
                }
            }
        }
    }

    // Método utilitário 
    void limpar(){
        for (auto& linha : ligacao) {
            std::fill(linha.begin(), linha.end(), 0);
        }
        for (auto& linha : intervalo) {
            std::fill(linha.begin(), linha.end(), 0);
        }
        custoTotalEnergia = 0.0;
        viavel = true;
    }

    void print()const{
        int nPivos = ligacao.size();
        if (nPivos == 0) return;
        int nHoras = ligacao[0].size();

        std::cout << std::endl<<"Eh viavel: "<<viavel<<
            "\nCusto Total de Energia: " << custoTotalEnergia << 
            "Valor FO: " << funcObjetivo << std::endl;
        for(int i = 0; i < nPivos; i++){
            for(int j = 0; j < nHoras; j++){
                std::cout << ligacao[i][j]<<" ";
            }
            std::cout << std::endl;
        }
        std::cout << "\n\nIntervalos de Irrigação\n";
        for(int i = 0; i < nPivos; i++){
            for(int j = 0; j < nHoras; j++){
                std::cout << intervalo[i][j]<<" ";
            }
            std::cout << std::endl;
        }
    }
};

#endif