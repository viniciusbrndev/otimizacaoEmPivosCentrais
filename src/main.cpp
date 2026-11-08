#include "../include/Instancia.hpp"
#include "../include/Solucao.hpp"
#include "../include/Construtor.hpp"
#include "iostream"
#include <string>
#include <cstdlib>


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
    Solucao SolPossivel;
    if(argc == 3){
        SolPossivel = HeuristicaCon.solucaoGulosaParcial(std::atof(argv[2]));
    }
    else{
        SolPossivel = HeuristicaCon.solucaoGulosaMelhorada();
    }
    
    // std::cout<<std::endl;
    // SolPossivel = HeuristicaCon.metodoDeRefinamento(SolPossivel);
    SolPossivel.print();
    // std::cout<<std::endl;

    return 0;
}