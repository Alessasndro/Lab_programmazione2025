//
// Created by valen on 10/11/2025.
//

#include "Gestore.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // NECESSARIO per formattare il saldo

//costruttore
Gestore::Gestore() {
    loadfile(); // Carica le transazioni dal file al momento della creazione del gestore
}

void Gestore::add_transiction_entrata(double importo, const std::string& description) {
    transazioni.push_back(std::make_unique<Transazione>(importo, description, true));
    std::cout << "entrata registrata con successo" << std::endl;
}

void Gestore::add_transiction_uscita(double importo, const std::string& description) {
    transazioni.push_back(std::make_unique<Transazione>(importo, description, false));
    std::cout << "uscita registrata con successo" << std::endl;
}
//show transaction
void Gestore::show_all_transiction() const {
    if (transazioni.empty()) {
        std::cout << "Nessuna transazione registrata." << std::endl;
        return;
    }
    // Corretto il refuso /n in \n e aggiunto il manipolatore
    std::cout << "\n ----ELENCO TRANSIZIONI----" << std::endl;

    for (size_t i = 0; i < transazioni.size(); ++i) {
        std::cout << i+1 << ".";
        transazioni[i]->Showditails(); // Usa il metodo corretto
    }

    std::cout << "...................."<< std::endl;
    // Formattazione a 2 decimali per correggere la visualizzazione corrotta
    std::cout << " SALDO CORRENTE: " << std::fixed << std::setprecision(2) << calcola_saldo() << "euro" << std::endl;
}

//salva transizione su file
void Gestore::savefile() const {
    std::ofstream file(nomeFile);
    if (!file.is_open()) {
        std::cerr << " Errore: impossibile aprire il file per il salvataggio." << std::endl;
        return;
    }
    file<< "Tipo,Importo,Description,Data\n"; // Intestazione CSV (rimosso std::endl in eccesso)
    for (const auto& transazione : transazioni) {
        file << transazione->tocsv() << "\n"; // Usa il metodo corretto
    }
    file.close();
    std::cout << " Salvate " << transazioni.size() << " transazioni" << std::endl;
}

//load transazioni da file
void Gestore::loadfile() {
    std::ifstream file(nomeFile);
    if (!file.is_open()) {
        return;
    }
    transazioni.clear();
    std::string line;
    std::getline(file, line); // Salta intestazione
    int count = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string tipo, importoStr, descrizioneCompleta, timestampStr;

        std::getline(ss, tipo, ',');
        std::getline(ss, importoStr, ',');

        // Legge la descrizione che è racchiusa tra virgolette, leggendo fino alla virgola finale
        std::getline(ss, descrizioneCompleta, ',');

        std::getline(ss, timestampStr); // Legge il resto (Data)

        // Rimuovi virgolette dalla descrizione: ora cerca le virgolette all'esterno
        if (descrizioneCompleta.size() >= 2 && descrizioneCompleta.front() == '"' && descrizioneCompleta.back() == '"') {
            descrizioneCompleta = descrizioneCompleta.substr(1, descrizioneCompleta.length() - 2);
        }

        try {
            double importo = std::stod(importoStr);
            bool isEntrata = (tipo == "ENTRATA"); // Usa "ENTRATA" come nel tuo tocsv()

            // NOTA: Per il momento, il costruttore Transazione ignora il timestamp e usa la data corrente.
            transazioni.push_back(std::make_unique<Transazione>(importo, descrizioneCompleta, isEntrata));
            count++;
        } catch (const std::exception& e) {
            std::cerr << " Errore nel caricamento riga: " << line << ". Ignorata." << std::endl;
        }
    }

    file.close();
    if (count > 0) {
        std::cout << " Caricate " << count << " transazioni" << std::endl;
    }
}

double Gestore::calcola_saldo() const {
    double saldo = 0.0;
    for (const auto& transazione : transazioni) {
        if (transazione->isEntrata()) {
            saldo += transazione->getImporto();
        } else {
            saldo -= transazione->getImporto();
        }
    }
    return saldo;
}

size_t Gestore::getNumeroTransazioni() const {
    return transazioni.size();
}