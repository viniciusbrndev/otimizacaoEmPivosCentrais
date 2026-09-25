#ifndef CONSTRUTOR_H
#define CONSTRUTOR_H
#include "../include/Construtor.hpp"
#include "../include/Instancia.hpp"
#include "../include/Solucao.hpp"
#include <vector>



class Construtor{
    private:
        Instancia* inst;

    public:
        explicit Construtor(Instancia* inst);
        //auxiliares
        bool validaSolucao(Solucao& sol);
        bool horasNecessaria(Solucao& sol);
        double calculaCustoEnergia(const Solucao& sol);
        double calculaGastoHora(int t, Solucao& sol);
        //Contrutores de Soluções
        Solucao solucaoGulosa();

};












#endif