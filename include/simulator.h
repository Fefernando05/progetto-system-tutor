/*
 * Simulator.h
 * Simula il passaggio dei veicoli sull'autostrada e applica sanzioni
 * Autore: Fernand Dongmo
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

// Forward declaration per evitare dipendenza circolare
class TutorSystem;

#include "Vehicle.h"
#include "Highway.h"
#include <vector>

class Simulator
{
private:
    TutorSystem* tutor; // Puntatore al sistema Tutor

public:
    // Costruttore con puntatore a TutorSystem
    Simulator(TutorSystem* tutorSys);

    // Avanza la simulazione
    void runSimulation(double deltaSeconds);

    // Resetta i veicoli
    void reset();
};

#endif

