#pragma once

#include <string>
#include <vector>

struct Pivo {
    int id = 0;
    double aguaPhora = 0.0;
    double potenciaKw = 0.0;
    int horasNecessarias = 0;

    bool operator>(const Pivo& outro) const {
        return horasNecessarias > outro.horasNecessarias;
    }
};

// Dados do problema. Somente leitura depois de carregada.
class Instancia {
  private:
    int nPivos = 0;
    double limiteAguaHora = 0.0;
    int nHorasHorizonte = 0;

    std::vector<double> custoEnergiaHora;  // tamanho nHorasHorizonte
    std::vector<Pivo> pivos;

    void carregarDeFicheiro(const std::string& caminhoArquivo);

  public:
    // Lança std::runtime_error se o arquivo não abrir ou estiver malformado.
    explicit Instancia(const std::string& nomeArq);

    void imprimeInst() const;
    static std::string formataNome(const char* argv);

    int getNPivos() const;
    double getLimiteAguaHora() const;
    int getNHorasHorizonte() const;

    double getGastoECPivo(int p) const;      // potência (kW). Antes retornava int.
    double getGastoWChoraPivo(int p) const;  // água por hora
    int getDemandaHorasPivo(int p) const;    // -1 se o índice for inválido

    const std::vector<double>& getCustoEnergiaHora() const;
    const std::vector<Pivo>& getPivos() const;

    // Lança std::out_of_range se t for inválido.
    double getCustoEnergiaNaHora(int t) const;

    // Custo de energia de ligar o pivô p na hora t (potência × tarifa).
    double custoEnergiaPivoHora(int p, int t) const;
};
