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
void Instancia::imprimeInst(){
    std::cout<<"Num pivos: "<<nPivos<<"\nLimite de água: "<<limiteAguaHora<<"Num horas: "<<nHorasHorizonte;
    for(int i = 0; i < nPivos;i++){
        std::cout<<"\nAgua/h: "<<pivos[i].aguaPhora<<"\nPotencia: "<<pivos[i].potenciaKw<<"\nHoras Nesc: "<<pivos[i].horasNecessarias;
    }
    std::cout<<"\nCutoEC/h: ";
    for(int i = 0; i < nHorasHorizonte;i++)
        std::cout<<custoEnergiaHora[i];
}
std::string Instancia::formataNome(const char* argv){
    std::filesystem::path caminhoUsuario(argv);
    std::filesystem::path caminhoDesejado = "instancias";

    std::filesystem::path caminhoAbsoluto = caminhoDesejado/caminhoUsuario;
    if(std::filesystem::exists(caminhoAbsoluto)){
        return caminhoAbsoluto.string();
    }
    std::cout<<"O Arquivo  "<<caminhoUsuario.string()<<" não existe no diretório: "<<caminhoDesejado.string();
    return "";
}

//GETERS
int Instancia::getNPivos() const{
    return nPivos;
}
double Instancia::getLimiteAguaHora() const{
    return limiteAguaHora;
}
int Instancia::getNHorasHorizonte() const{
    return nHorasHorizonte;
}
const std::vector<double>& Instancia::getCustoEnergiaHora() const{
    return this->custoEnergiaHora;
}
const std::vector<Pivo>& Instancia::getPivos() const{
    return this->pivos;
}
int Instancia::getDemandaDoPivo(int t){
    return this->pivos[t].horasNecessarias;
}
double Instancia::getGastoPivo(int i){
    return this->pivos[i].aguaPhora;
}

double Instancia::getCustoEnergiaNaHora(int t) const{
    if(t>nHorasHorizonte)
        return 0.0;
    return this->custoEnergiaHora[t];
}
double Instancia::getDemandaTotalAgua() const{
    return 0.0;
}