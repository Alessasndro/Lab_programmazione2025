//
// Created by valen on 10/11/2025.
//

#ifndef LABB_GESTORE_H
#define LABB_GESTORE_H
#include <string>
#include  <vector>
#include "Transazione.h"
#include <memory>


class Gestore {
private:
    std::vector<std::unique_ptr<Transazione>> transazioni;
    std::string nomeFile="contocorrente.csv";

public:
    Gestore();
    double calcola_saldo() const;
    void add_transiction_entrata(double importo, const std::string& description);
    void add_transiction_uscita(double importo, const std::string& description);
    void show_all_transiction() const;
    void savefile() const;    // Salva tutte le transazioni su file CSV
    void loadfile();//carica transizioni da file csv
    size_t getNumeroTransazioni() const;//uso size t per contare numero transizioni
};


#endif //LABB_GESTORE_H