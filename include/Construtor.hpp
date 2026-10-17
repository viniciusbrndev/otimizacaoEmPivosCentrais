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
        bool horasNecessaria(Solucao& sol)const;
        int demandaRestantePivo(int pivo, const Solucao& so)const;
        double calculaCustoEnergia(const Solucao& sol);
        double calculaGastoAguaHora(int t, Solucao& sol);
        static bool estourouVazao(double usoDeAguaHr, double vMax);
        int randPos(int pivo, int horaAtual, const Solucao& sol);
        bool randonSwap(int pivo, int t, Solucao& s);
        double calculaFO(const Solucao& s) const;
        //Construtores de Soluções
        Solucao& solucaoGulosa();
        Solucao solucaoGulosaMelhorada();
        Solucao metodoDeRefinamento(Solucao& solucao);
};












#endif