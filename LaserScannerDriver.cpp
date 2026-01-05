#include "LaserScannerDriver.h"
#include <algorithm>

// Costruttore di default 
LaserScannerDriver::LaserScannerDriver()
    : RIS_ANGL{ 1.0 }, inizio{ 0 }, fine{ 0 }
{
    buffer = new double* [BUFFER_DIM + 1];
}

// Costruttore con parametro
LaserScannerDriver::LaserScannerDriver(double rs)
    : RIS_ANGL{ rs }
{
    if (RIS_ANGL < 0.1 || RIS_ANGL > 1)
        throw InvalidResolutionException();

    buffer = new double* [BUFFER_DIM + 1];
    inizio = 0;
    fine = 0;
}

// Costruttore di copia
LaserScannerDriver::LaserScannerDriver(const LaserScannerDriver& ls)
    : RIS_ANGL{ ls.RIS_ANGL }
{
    buffer = new double* [BUFFER_DIM + 1];
    inizio = 0;
    fine = 0; // Importante inizializzare prima di copiare

    int i = ls.inizio;
    while (i != ls.fine)
    {
        double* from = ls.buffer[i];              // lettura da cui copiare
        double* to = new double[ls.getNum()];     // lettura su cui copiare
        std::copy(from, from + getNum(), to);     // copy
        queue(to);                                // inserisco la lettura copiata nel buffer

        i = (i + 1) % (BUFFER_DIM + 1);           // incremento controllando di non essere arrivato alla fine 
    }
}

// Distruttore
LaserScannerDriver::~LaserScannerDriver()
{
    clear_buffer();
    delete[] buffer;
}

// Costruttore di move
LaserScannerDriver::LaserScannerDriver(LaserScannerDriver&& ls) noexcept
    : buffer{ ls.buffer }, RIS_ANGL{ ls.RIS_ANGL }, inizio{ ls.inizio }, fine{ ls.fine }
{
    ls.buffer = nullptr;      // invalido la sorgente 
    ls.inizio = 0;
    ls.fine = 0;
}

// Funzione queue 
void LaserScannerDriver::queue(double* lettura)
{
    if (increment(fine) == inizio)   // buffer e' pieno
    {
        delete[] buffer[inizio];      // libero memoria in posizione inizio
        buffer[inizio] = nullptr;
        inizio = increment(inizio);   // avanzo inizio
        
        buffer[fine] = lettura;       // inserisco in fine
        fine = increment(fine);
    }
    else
    {
        buffer[fine] = lettura;      // se non e' vuoto inserisco in fine e incremento tale indice
        fine = increment(fine);
    }
}

// Funzione dequeue 
double* LaserScannerDriver::dequeue()
{
    if (isEmpty())
        throw EmptyBufferException();

    double* tmp = buffer[inizio];    // puntatore alla lettura da eliminare
    buffer[inizio] = nullptr;
    inizio = increment(inizio);      // inizio punta alla nuova lettura piu' vecchia
    return tmp;                      // la memoria verra liberata dal chiamante
}

// Funzione getLast
double* LaserScannerDriver::getLast() const
{
    if (isEmpty())
        throw EmptyBufferException();

    // Gestione dell'indice circolare all'indietro
    int lastIndex = (fine - 1);
    if (lastIndex < 0)
        lastIndex = BUFFER_DIM;

    return buffer[lastIndex];
}

// Funzione get_scan
std::vector<double> LaserScannerDriver::get_scan()
{
    double* tmp = dequeue();
    std::vector<double> v;
    for (int i = 0; i < getNum(); i++)
        v.push_back(tmp[i]);
    delete[] tmp;
    return v;
}

// Funzione new_scan
void LaserScannerDriver::new_scan(std::vector<double>& v)
{
    double* lettura = new double[getNum()];
    for (int i = 0; i < getNum(); i++)
    {
        if (i < (int)v.size())
            lettura[i] = v.at(i);
        else
            lettura[i] = 0.0;
    }
    queue(lettura);
}

// Funzione clear_buffer 
void LaserScannerDriver::clear_buffer()
{
    if (!buffer) return; // Safety check

    while (!isEmpty())
    {
        delete[] buffer[inizio];   // libero memoria
        buffer[inizio] = nullptr;
        inizio = increment(inizio);
    }
}

// Operatore <<
std::ostream& operator<<(std::ostream& os, LaserScannerDriver& ls)
{
    if (ls.isEmpty())
    {
        return os << "IL BUFFER E' VUOTO";
    }
    double* last = ls.getLast();
    double angolo = 0; // Meglio double per coerenza
    for (int i = 0; i < ls.getNum(); i++)
    {
        os << "(" << last[i] << "," << angolo << ")  ";
        angolo += ls.getRis();
    }
    return os;
}

// Funzione get_distance
double LaserScannerDriver::get_distance(double a) const
{
    if (isEmpty())
    {
        std::cout << "IL BUFFER E' VUOTO " << "\n";
        return -1;
    }
    double* p = getLast();

    if (a < 0)
        return p[0];
    if (a >= (getNum() * getRis() - getRis()))
        return p[getNum() - 1];

    double angleCounter = 0;
    int i = 0;
    
    // Trova l'indice i tale che angleCounter >= a
    // Nota: (i * getRis()) calcola l'angolo all'indice i
    while ((i * getRis()) < a) 
    {
        i++;
    }
    angleCounter = i * getRis();

    if (angleCounter == a)
        return p[i];

    // Confronto distanze angolari
    if ((angleCounter - a) < (a - (angleCounter - getRis())))
        return p[i];
    else
        return p[i - 1];
}

// Assegnamento di move 
LaserScannerDriver& LaserScannerDriver::operator=(LaserScannerDriver&& ls)
{
    if (RIS_ANGL != ls.RIS_ANGL)
        throw InvalidResolutionException();
    
    if (this == &ls) return *this; // Protezione self-assignment

    clear_buffer(); // libera la memoria puntata PRIMA di cancellare l'array di puntatori
    delete[] buffer;

    buffer = ls.buffer;
    inizio = ls.inizio;
    fine = ls.fine;

    ls.buffer = nullptr;
    ls.inizio = 0;
    ls.fine = 0;

    return *this;
}

// Assegnamento di copia
LaserScannerDriver& LaserScannerDriver::operator=(const LaserScannerDriver& ls)
{
    if (RIS_ANGL != ls.RIS_ANGL)
        throw InvalidResolutionException();

    if (this == &ls)
    {
        std::cout << "\n" << "SELF ASSIGNMENT !";
        return *this;
    }

    clear_buffer();

    int i = ls.inizio;
    while (i != ls.fine)
    {
        double* from = ls.buffer[i];
        double* to = new double[ls.getNum()];
        std::copy(from, from + getNum(), to);
        queue(to);

        i = (i + 1) % (BUFFER_DIM + 1);
    }
    return *this;
}
