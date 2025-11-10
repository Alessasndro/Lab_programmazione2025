//
// Created by valen on 10/11/2025.
//

#include "Transazione.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>

Transazione::Transazione(double importo, std::string  description, bool entrata_uscita)
    : importo(importo), description(std::move(description)), entrata_uscita(entrata_uscita) {
    data = std::time(nullptr); // Imposta la data corrente
}

//dettaglio transizione :
void Transazione::Showditails() const {
    if (entrata_uscita) {
        std::cout<<"[ENTRATA]-";
    } else {
        std::cout<<"[USCITA]-";
    }
    std::cout <<"data:"<<getDataFormattata()<<" _ " <<description << " _ " << importo << " $ "<< std::endl;
}

std::string Transazione::tocsv() const {
    std::ostringstream ss;
    std::string tipo= entrata_uscita ? "ENTRATA" : "USCITA";
    ss << tipo << "," << importo << "," <<"|"<< description << "," << data;
    return ss.str();
}

//getter

double Transazione::getImporto() const {
    return importo;
}
std::string Transazione::getDescription() const {
    return description;
}
std::time_t Transazione::getData() const {
    return data;
}
bool Transazione::isEntrata() const {
    return entrata_uscita;
}
//formatta data
std::string Transazione::getDataFormattata() const {
    std::tm* timeinfo = std::localtime(&data);
    std::stringstream ss;
    ss << std::put_time(timeinfo, "%d/%m/%Y %H:%M");
    return ss.str();
}

