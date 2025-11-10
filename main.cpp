#include "Gestore.h"
#include <iostream>
#include <limits>
#include <iomanip> // NECESSARIO per formattare il saldo

// Funzione per mostrare il menu
void mostraMenu() {
    std::cout << "\n=== GESTIONE CONTO CORRENTE ===" << std::endl;
    std::cout << "1. Aggiungi entrata" << std::endl;
    std::cout << "2. Aggiungi uscita" << std::endl;
    std::cout << "3. Mostra transazioni" << std::endl;
    std::cout << "4. Calcola saldo" << std::endl;
    std::cout << "5. Salva su file" << std::endl;
    std::cout << "6. Esci" << std::endl;
    std::cout << "Scelta: ";
}

// Funzione principale
int main() {
    Gestore gestore;
    int scelta;

    std::cout << "Benvenuto nel Gestore Conto Corrente!" << std::endl;

    while (true) {
        mostraMenu();
        // Aggiunto controllo robusto per l'input della scelta
        if (!(std::cin >> scelta)) {
            std::cout << " Input non valido! Inserisci un numero." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (scelta) {
            case 1: {
                double importo;
                std::string descrizione;

                std::cout << "Importo: ";
                if (!(std::cin >> importo)) {
                    std::cout << " Importo non valido!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                std::cin.ignore();

                if (importo <= 0) {
                    std::cout << " Importo deve essere positivo!" << std::endl;
                    break;
                }

                std::cout << "Descrizione: ";
                std::getline(std::cin, descrizione);

                gestore.add_transiction_entrata(importo, descrizione);
                break;
            }

            case 2: {
                double importo;
                std::string descrizione;

                std::cout << "Importo: ";
                 if (!(std::cin >> importo)) {
                    std::cout << " Importo non valido!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                std::cin.ignore();

                if (importo <= 0) {
                    std::cout << " Importo deve essere positivo!" << std::endl;
                    break;
                }

                std::cout << "Descrizione: ";
                std::getline(std::cin, descrizione);

                gestore.add_transiction_uscita(importo, descrizione);
                break;
            }

            case 3:
                gestore.show_all_transiction();
                break;

            case 4:
                // AGGIUNTA la formattazione a 2 decimali per correggere l'output
                std::cout << " Saldo: " << std::fixed << std::setprecision(2) << gestore.calcola_saldo() << "euro" << std::endl;
                break;

            case 5:
                gestore.savefile();
                break;

            case 6:
                gestore.savefile();
                std::cout << " Arrivederci!" << std::endl;
                return 0;

            default:
                std::cout << " Scelta non valida!" << std::endl;
        }

        std::cout << "\nPremi INVIO per continuare...";
        std::cin.get();
        std::cout << std::endl;
    }

    return 0;
}