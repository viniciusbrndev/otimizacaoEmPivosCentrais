#include "../include/Instancia.hpp"
#include "../include/Solucao.hpp"
#include "../include/Construtor.hpp"
#include "iostream"
#include <string>


int main(int argc,char* argv[]){
    if (argc < 2) {
        return 1;
    }
    std::string caminho = Instancia::formataNome(argv[1]);
    if(caminho.empty()){
        std::cout<<"Arquivo não econtrado";
        return 1;
    }
    //Instanciando objetos
    Instancia inst(caminho);
    inst.imprimeInst();
    Construtor HeuristicaCon(&inst);
    //Construindo uma solucao gulosa
    Solucao SolPossivel = HeuristicaCon.solucaoGulosa();
    SolPossivel.print(inst);
    std::cout<<std::endl;

    SolPossivel = HeuristicaCon.metodoDeRefinamento(SolPossivel);
    SolPossivel.print(inst);
    std::cout<<std::endl;

    return 0;
}