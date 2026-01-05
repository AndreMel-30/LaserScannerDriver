
# C++ Laser Scanner Driver

> **Nota:** Questo progetto è stato sviluppato come elaborato accademico universitario (anno 2020-2021). L'obiettivo principale è l'implementazione di strutture dati a basso livello e la gestione manuale della memoria in C++.

## Descrizione
Questo progetto implementa un driver software simulato per un sensore Laser Scanner. Il driver gestisce un flusso di dati in entrata (scansioni laser) utilizzando un **buffer circolare** a dimensione fissa.

Il core del progetto è la gestione efficiente delle risorse tramite puntatori raw e l'implementazione completa della **Rule of 5** del C++ (Costruttori, Distruttori, Copy/Move Semantics), garantendo la corretta allocazione e deallocazione della memoria heap senza l'ausilio di smart pointers o container STL per il buffer interno.

##  Caratteristiche Tecniche

* **Gestione della Memoria Manuale**: Utilizzo di `double**` (array di puntatori) per gestire le scansioni dinamiche.
* **Buffer Circolare (Ring Buffer)**: Implementazione logica FIFO per memorizzare le ultime `N` scansioni senza riallocare l'intero buffer.
* **Move Semantics (C++11)**: Implementazione di Move Constructor e Move Assignment Operator per il trasferimento efficiente delle risorse.
* **Deep Copy**: Implementazione del Copy Constructor per la copia profonda delle strutture dinamiche.
* **Gestione Eccezioni**: Definizioni di eccezioni custom per buffer vuoto o risoluzioni non valide.

##  Struttura del Progetto

* `LaserScannerDriver.h`: Definizione della classe e dell'interfaccia.
* `LaserScannerDriver.cpp`: Implementazione della logica del driver.
* `main.cpp`: Programma demo che mostra l'utilizzo della classe.

## 💻Compilazione ed Esecuzione

Il progetto non richiede librerie esterne oltre alla standard library. È necessario un compilatore che supporti almeno **C++11**.

### Compilazione (g++)
```bash
g++ -std=c++11 main.cpp LaserScannerDriver.cpp -o laser_driver

```

### Esecuzione

```bash
./laser_driver

```

## Esempio di Utilizzo

```cpp
#include "LaserScannerDriver.h"

int main() {
    // Inizializza il driver con risoluzione angolare di 1.0 gradi
    LaserScannerDriver driver(1.0);

    // Crea dati simulati
    std::vector<double> scan_data(180, 10.5); 

    // Inserisce una nuova scansione nel buffer
    driver.new_scan(scan_data);

    // Recupera la distanza ad un angolo specifico
    double distance = driver.get_distance(45.0);
    
    std::cout << "Distanza a 45 gradi: " << distance << std::endl;
    
    return 0;
}

```

##  Competenze Acquisite

Attraverso questo progetto sono stati approfonditi i seguenti concetti:

* Allocazione dinamica (`new`/`delete`) e prevenzione dei Memory Leaks.
* Puntatori e aritmetica dei puntatori.
* Override degli operatori (es. `operator<<`, `operator=`).
* Algoritmi su buffer circolari.

---

**Autore:** Andrea Melissari
**Corso:** Laboratorio di Programmazione/Università di Padova/A.A. 2020-2021

```

