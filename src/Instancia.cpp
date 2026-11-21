#include "Instancia.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

Instancia::Instancia(const std::string& nomeArq) {
    this->carregarDeFicheiro(nomeArq);
}

void Instancia::carregarDeFicheiro(const std::string& caminhoArquivo) {
    std::ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Arquivo nao pode ser aberto: " + caminhoArquivo);
    }

    if (!(arquivo >> nPivos >> limiteAguaHora >> nHorasHorizonte) ||
        nPivos <= 0 || nHorasHorizonte <= 0) {
        throw std::runtime_error(
            "Cabecalho invalido (esperado: nPivos limiteAgua nHoras)");
    }

    double tmp = 0.0;
    for (int i = 0; i < nHorasHorizonte; i++) {
        if (!(arquivo >> tmp)) {
            throw std::runtime_error("Custos de agua incompletos");
        }
    }

    // Custo de energia (EC) por hora
    custoEnergiaHora.reserve(nHorasHorizonte);
    for (int i = 0; i < nHorasHorizonte; i++) {
        if (!(arquivo >> tmp)) {
            throw std::runtime_error("Custos de energia incompletos");
        }
        custoEnergiaHora.push_back(tmp);
    }

    pivos.reserve(nPivos);
    for (int i = 0; i < nPivos; i++) {
        Pivo p;
        if (!(arquivo >> p.id >> p.aguaPhora >> p.potenciaKw >>
              p.horasNecessarias)) {
            throw std::runtime_error("Dados de pivos incompletos");
        }
        pivos.push_back(p);
    }
}

void Instancia::imprimeInst() const {
    std::cout << "Num pivos: " << nPivos << "\nLimite de água: " << limiteAguaHora
              << "\nNum horas: " << nHorasHorizonte;
    for (int i = 0; i < nPivos; i++) {
        std::cout << "\nAgua/h: " << pivos[i].aguaPhora
                  << "\nPotencia: " << pivos[i].potenciaKw
                  << "\nHoras Nesc: " << pivos[i].horasNecessarias;
    }
    std::cout << "\nCustoEC/h: ";
    for (int i = 0; i < nHorasHorizonte; i++) {
        std::cout << custoEnergiaHora[i] << ' ';
    }
    std::cout << '\n';
}

std::string Instancia::formataNome(const char* argv) {
    std::filesystem::path caminhoUsuario(argv);
    std::filesystem::path caminhoDesejado = "instancias";

    std::filesystem::path caminhoAbsoluto = caminhoDesejado / caminhoUsuario;
    if (std::filesystem::exists(caminhoAbsoluto)) {
        return caminhoAbsoluto.string();
    }
    std::cout << "O Arquivo  " << caminhoUsuario.string()
              << " não existe no diretório: " << caminhoDesejado.string() << '\n';
    return "";
}

// GETTERS
int Instancia::getNPivos() const { return nPivos; }
double Instancia::getLimiteAguaHora() const { return limiteAguaHora; }
int Instancia::getNHorasHorizonte() const { return nHorasHorizonte; }

const std::vector<double>& Instancia::getCustoEnergiaHora() const {
    return this->custoEnergiaHora;
}
const std::vector<Pivo>& Instancia::getPivos() const { return this->pivos; }

int Instancia::getDemandaHorasPivo(int p) const {
    if (p >= 0 && p < this->nPivos) {
        return this->pivos[p].horasNecessarias;
    }
    return -1;
}
double Instancia::getGastoECPivo(int p) const {
    return this->pivos.at(p).potenciaKw;
}
double Instancia::getGastoWChoraPivo(int p) const {
    return this->pivos.at(p).aguaPhora;
}

// Antes os dois ramos do if eram idênticos e nada validava o índice.
double Instancia::getCustoEnergiaNaHora(int t) const {
    return this->custoEnergiaHora.at(t);
}

double Instancia::custoEnergiaPivoHora(int p, int t) const {
    return this->getGastoECPivo(p) * this->getCustoEnergiaNaHora(t);
}
