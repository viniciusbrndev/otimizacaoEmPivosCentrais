#include "../include/Instancia.hpp"
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
    
    Instancia inst(caminho);

    inst.imprimeInst();


    return 0;
}