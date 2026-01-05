#include <iostream>
#include <vector>
#include "LaserScannerDriver.h"

int main() {
    try {
        std::cout << "--- Laser Scanner Driver Demo ---\n";

        // 1. Creazione Driver con risoluzione 0.5 gradi
        LaserScannerDriver driver(1.0);
        std::cout << "Driver creato. Risoluzione: " << driver.getRis() << "\n";

        // 2. Creazione dati simulati
        std::vector<double> scan1;
        for(int i=0; i < driver.getNum(); i++) scan1.push_back(10.0 + i * 0.1); // Distanza crescente

        std::vector<double> scan2;
        for(int i=0; i < driver.getNum(); i++) scan2.push_back(50.0); // Distanza fissa

        // 3. Inserimento scansioni
        std::cout << "Inserimento scansione 1...\n";
        driver.new_scan(scan1);
        
        std::cout << "Inserimento scansione 2...\n";
        driver.new_scan(scan2);

        // 4. Stampa ultima scansione
        std::cout << "Ultima scansione nel buffer:\n" << driver << "\n\n";

        // 5. Test get_distance
        double angle = 5.0;
        std::cout << "Distanza all'angolo " << angle << ": " << driver.get_distance(angle) << "\n";

        // 6. Test Copy Constructor
        std::cout << "Test copia costruttore...\n";
        LaserScannerDriver driverCopy = driver;
        std::cout << "Copia effettuata. Ultima scansione copia: " << driverCopy.getLast()[0] << "\n";

    } catch (LaserScannerDriver::InvalidResolutionException&) {
        std::cerr << "Errore: Risoluzione non valida!\n";
    } catch (LaserScannerDriver::EmptyBufferException&) {
        std::cerr << "Errore: Buffer vuoto!\n";
    } catch (...) {
        std::cerr << "Errore generico.\n";
    }

    return 0;
}
