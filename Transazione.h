//
// Created by valen on 10/11/2025.
//

#ifndef LABB_TRANSAZIONE_H
#define LABB_TRANSAZIONE_H
#include <string>
#include <ctime>

class Transazione {
protected:
    double importo;
    std::string description ;
    std::time_t data;
    bool entrata_uscita; // true per entrata, false per uscita
public:
    Transazione(double importo, std::string  description, bool entrata_uscita);
    void Showditails() const;
    std::string tocsv() const;

    //getter
    double getImporto() const;
    std::string getDescription() const;
    std::time_t getData() const;
    bool isEntrata() const;

    std::string getDataFormattata() const; //formatta data
};


#endif //LABB_TRANSAZIONE_H