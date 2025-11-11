#include "gtest/gtest.h"
#include "../Gestore.h"
#include "../Transazione.h"
#include <fstream>
#include <sstream>
#include <cstdio> // Per std::remove

// Nome del file di test per Gestore
const std::string TEST_FILENAME = "test_contocorrente.csv";

// =================================================================
// Setup e Utility per i Test
// =================================================================

// Classe GestoreMock per isolare il nome del file e permettere il cleanup
class GestoreMock : public Gestore {
public:
    GestoreMock() : Gestore() {
        // Forza il nome del file di test per Gestore
        // Nota: Nel tuo Gestore.h/cpp, 'nomeFile' è privato, quindi questo richiede
        // un piccolo adattamento, ma per semplicità useremo 'contocorrente.csv'
        // e faremo pulizia dopo. ASSUMIAMO che il file sia 'contocorrente.csv'.
        // Se si volesse cambiarlo, si dovrebbe modificare la classe Gestore.
    }
};

// Funzione di utility per pulire i file di test
void cleanupFile(const std::string& filename) {
    std::remove(filename.c_str());
}

// =================================================================
// Test per la classe Transazione
// =================================================================

TEST(TransazioneTest, CostruttoreEntrata) {
    Transazione t(100.50, "Stipendio", true);
    ASSERT_NEAR(100.50, t.getImporto(), 0.001);
    ASSERT_EQ("Stipendio", t.getDescription());
    ASSERT_TRUE(t.isEntrata());
    ASSERT_NE(0, t.getData()); // Verifica che la data sia stata impostata (non è 0)
}

TEST(TransazioneTest, CostruttoreUscita) {
    Transazione t(45.99, "Spesa", false);
    ASSERT_NEAR(45.99, t.getImporto(), 0.001);
    ASSERT_EQ("Spesa", t.getDescription());
    ASSERT_FALSE(t.isEntrata());
}

TEST(TransazioneTest, TocsvFormatoEntrata) {
    Transazione t(25.00, "Regalo", true);
    std::string csv = t.tocsv();
    // La data (timestamp) sarà dinamica, quindi verifichiamo le parti statiche
    // Formato atteso: Tipo,Importo,Descrizione,Data
    // Esempio: ENTRATA,25.00,"Regalo",1731248400
    ASSERT_TRUE(csv.rfind("ENTRATA,25.00,\"Regalo\",") == 0);
}

TEST(TransazioneTest, TocsvFormatoUscitaConVirgole) {
    Transazione t(5.50, "Caffè, Panino", false);
    std::string csv = t.tocsv();
    // La descrizione DEVE essere tra virgolette per gestire la virgola
    ASSERT_TRUE(csv.rfind("USCITA,5.50,\"Caffè, Panino\",") == 0);
}

TEST(TransazioneTest, GetDataFormattata) {
    Transazione t(1.0, "Test", true);
    // Non possiamo testare la data esatta, ma possiamo verificare il formato: DD/MM/YYYY HH:MM
    std::string dataFormattata = t.getDataFormattata();
    // Verifica lunghezza (es. 10/11/2025 12:30 ha 16 caratteri)
    ASSERT_EQ(16, dataFormattata.length());
    // Verifica presenza di '/' e ':'
    ASSERT_NE(std::string::npos, dataFormattata.find('/'));
    ASSERT_NE(std::string::npos, dataFormattata.find(':'));
}


// =================================================================
// Test per la classe Gestore
// =================================================================

// Imposta un ambiente di test pulito
struct GestoreTest : public ::testing::Test {
    Gestore gestore;
    const std::string filename = "contocorrente.csv"; // Il nome del file usato in Gestore.cpp

    // Viene eseguito prima di ogni test
    void SetUp() override {
        cleanupFile(filename); // Pulisce il file prima di ogni test
    }

    // Viene eseguito dopo ogni test
    void TearDown() override {
        cleanupFile(filename); // Pulisce il file dopo ogni test
    }
};

TEST_F(GestoreTest, SaldoInizialeZero) {
    ASSERT_NEAR(0.0, gestore.calcola_saldo(), 0.001);
    ASSERT_EQ(0, gestore.getNumeroTransazioni());
}

TEST_F(GestoreTest, AggiungiEntrataCalcolaSaldo) {
    gestore.add_transiction_entrata(50.00, "Regalo");
    gestore.add_transiction_entrata(10.50, "Ritorno");
    ASSERT_NEAR(60.50, gestore.calcola_saldo(), 0.001);
    ASSERT_EQ(2, gestore.getNumeroTransazioni());
}

TEST_F(GestoreTest, AggiungiUscitaCalcolaSaldo) {
    gestore.add_transiction_uscita(20.00, "Cena");
    gestore.add_transiction_uscita(5.00, "Caffè");
    ASSERT_NEAR(-25.00, gestore.calcola_saldo(), 0.001);
    ASSERT_EQ(2, gestore.getNumeroTransazioni());
}

TEST_F(GestoreTest, SaldoMisto) {
    gestore.add_transiction_entrata(100.00, "Stipendio");
    gestore.add_transiction_uscita(20.00, "Benzina");
    gestore.add_transiction_entrata(5.00, "Rimborso");
    gestore.add_transiction_uscita(15.50, "Acquisto");

    // Saldo atteso: 100.00 - 20.00 + 5.00 - 15.50 = 69.50
    ASSERT_NEAR(69.50, gestore.calcola_saldo(), 0.001);
    ASSERT_EQ(4, gestore.getNumeroTransazioni());
}

TEST_F(GestoreTest, SalvataggioECaricamento) {
    // 1. Aggiungi transazioni
    gestore.add_transiction_entrata(1000.00, "Stipendio del mese");
    gestore.add_transiction_uscita(250.00, "Affitto");
    gestore.add_transiction_uscita(15.99, "Libro, computer"); // Con virgola

    double saldoAtteso = 1000.00 - 250.00 - 15.99; // 734.01
    ASSERT_NEAR(saldoAtteso, gestore.calcola_saldo(), 0.001);
    ASSERT_EQ(3, gestore.getNumeroTransazioni());

    // 2. Salva
    gestore.savefile();

    // 3. Carica in un nuovo gestore (simula il riavvio)
    Gestore nuovoGestore;

    // 4. Verifica
    ASSERT_EQ(3, nuovoGestore.getNumeroTransazioni());
    // Il saldo è calcolato dalle transazioni caricate.
    // NOTA: Poiché il costruttore Transazione ignora il timestamp salvato e usa la data corrente,
    // se ci fossero logiche dipendenti dalla data non potremmo testarle, ma il saldo sì.
    ASSERT_NEAR(saldoAtteso, nuovoGestore.calcola_saldo(), 0.001);
}

TEST_F(GestoreTest, CaricamentoFileVuoto) {
    // Crea un file vuoto (solo intestazione)
    std::ofstream file(filename);
    file << "Tipo,Importo,Description,Data\n";
    file.close();

    Gestore gestoreVuoto; // Carica il file
    ASSERT_EQ(0, gestoreVuoto.getNumeroTransazioni());
    ASSERT_NEAR(0.0, gestoreVuoto.calcola_saldo(), 0.001);
}

TEST_F(GestoreTest, CaricamentoFileInesistente) {
    // La funzione SetUp rimuove il file, quindi all'inizio non esiste.
    // L'esecuzione del costruttore deve gestire l'assenza.
    Gestore gestoreNuovo;
    ASSERT_EQ(0, gestoreNuovo.getNumeroTransazioni());
    ASSERT_NEAR(0.0, gestoreNuovo.calcola_saldo(), 0.001);
}