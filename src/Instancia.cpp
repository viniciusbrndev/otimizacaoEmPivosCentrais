#include "../include/Instancia.hpp"

Instancia::Instancia(std::string& nomeArq){
    this->carregarDeFicheiro(nomeArq);
}
void Instancia::carregarDeFicheiro(const std::string& caminhoArquivo){
    std::ifstream arquivo(caminhoArquivo);
    if(!arquivo.is_open()){
        std::cout<<"Arquivo não pode ser aberto";
        return;
    }
    arquivo>>nPivos>>limiteAguaHora>>nHorasHorizonte;
    int temp;
    //ignorar custo de água WC
    for(int i = 0; i < nHorasHorizonte; i++){
        arquivo>>temp;
    }
    double tmp = 0.0;
    //custo de enrgia EC
    for(int i = 0; i < nHorasHorizonte; i++){
        arquivo>>tmp;
        custoEnergiaHora.push_back(tmp);
    }
    Pivo pTemp;
    for(int i = 0; i < nPivos; i++){
        arquivo>>pTemp.id>>pTemp.aguaPhora>>pTemp.potenciaKw>>pTemp.horasNecessarias;
        pivos.push_back(pTemp);
    }
    arquivo.close();
}
std::string Instancia::formataNome(char* argv[]){
    std::filesystem::path caminhoUsuario(argv[1]);
    std::filesystem::path caminhoDesejado = "/Documentos/otimizacaoEmPivosCentrais/instancias";

    std::filesystem::path caminhoAbsoluto = caminhoDesejado / caminhoUsuario;
    if(std::filesystem::exists(caminhoAbsoluto)){
        return caminhoAbsoluto.string();
    }
    std::cout<<"O Arquivo"<<caminhoUsuario.string()<<"não existe no diretório: "<<caminhoDesejado.string();
    return "";
}
int Instancia::getNPivos() const{
    return nPivos;
}
double Instancia::getLimiteAguaHora() const{
    return limiteAguaHora;
}
int Instancia::getNHorasHorizonte() const{
    return nHorasHorizonte;
}
const std::vector<double>& Instancia::getCustoEnergiaHora() const{}
const std::vector<Pivo>& Instancia::getPivos() const{}

double Instancia::getCustoEnergiaNaHora(int t) const{

}
double Instancia::getDemandaTotalAgua() const{}