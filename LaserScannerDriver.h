#ifndef LASERSCANNERDRIVER_H
#define LASERSCANNERDRIVER_H

#include <iostream>
#include <vector>

class LaserScannerDriver
{
public:
    // Costruttori
    LaserScannerDriver();                                  // default
    LaserScannerDriver(double);                            // riceve la risoluzione angolare
    LaserScannerDriver(LaserScannerDriver&&) noexcept;     // costruttore di move 
    LaserScannerDriver(const LaserScannerDriver&);         // costruttore di copia

    // Distruttore
    ~LaserScannerDriver();

    // Eccezioni
    class InvalidResolutionException {};
    class EmptyBufferException {};

    // Funzioni core
    void new_scan(std::vector<double>&);
    std::vector<double> get_scan();
    void clear_buffer();
    double get_distance(double a) const;

    // Funzioni utili public
    int getNum() const { return (int)(180 / RIS_ANGL); }      // numero di dati contenuti in una lettura 
    double* getLast() const;                                  // ritorna l'ultima lettura inserita
    bool isEmpty() const { return inizio == fine; }           // ritorna true se il buffer e' vuoto
    double getRis() const { return RIS_ANGL; }                // ritorna la risoluzione angolare

    // Operatori
    LaserScannerDriver& operator=(LaserScannerDriver&&);      // assegnamento di move
    LaserScannerDriver& operator=(const LaserScannerDriver&); // assegnamento di copia

private:
    // Variabili membro 
    double** buffer;              // buffer di puntatori
    int const BUFFER_DIM = 10;    // dimensione buffer
    double const RIS_ANGL;        // risoluzione angolare
    int inizio;                   // indice di inizio buffer
    int fine;                     // indice di fine buffer

    // Funzioni utili private 
    void queue(double*);                           // gestisce inserimenti nel buffer
    double* dequeue();
    int increment(int index) { return (index + 1) % (BUFFER_DIM + 1); }  // gestione indici buffer
};

// Dichiarazione operatore globale
std::ostream& operator<<(std::ostream& os, LaserScannerDriver& ls);

#endif
