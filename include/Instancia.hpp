#ifndef INSTANCIA_H
#define INSTANCIA_H
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>



typedef struct pivo
{
    int id;
    double aguaPhora;
    double potenciaKw;

    int horasNecessarias;
}Pivo;


class Instancia{
    private:    
        int nPivos;
        double limiteAguaHora;
        int nHorasHorizonte;
        
        std::vector<double> custoEnergiaHora; // Vetor de tamanho n_horas_horizonte
        
        std::vector<Pivo> pivos;

        void carregarDeFicheiro(const std::string& caminhoArquivo);
    public:
        explicit Instancia(std::string& nomeArq);
        void printPivo();
        void imprimeInst();
        static std::string formataNome(const char* argv);
        int getNPivos() const;
        double getLimiteAguaHora() const;
        int getNHorasHorizonte() const;
        int getDemandaDoPivo(int t);
        double getGastoPivo(int i);


        const std::vector<double>& getCustoEnergiaHora() const;
        const std::vector<Pivo>& getPivos() const;

        // Métodos utilitários
        double getCustoEnergiaNaHora(int t) const;
        double getDemandaTotalAgua() const;

};

#endif