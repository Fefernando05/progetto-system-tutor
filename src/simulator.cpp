/*
 * Simulator.cpp
 * Simula i veicoli e applica le sanzioni in base alla velocità
 * Autore: Fernand Dongmo
 */

#include "Simulator.h"
#include "TutorSystem.h"
#include <iostream>

using namespace std;

Simulator::Simulator(TutorSystem* tutorSys)
{
    tutor = tutorSys;
}

void Simulator::runSimulation(double deltaSeconds)
{
    Highway* highway = tutor->getHighway();
    std::vector<Vehicle>& vehicles = tutor->getVehicles();

    for(auto& v : vehicles)
    {
        // Avanza il veicolo
        double newPos = v.getPosition() + v.getSpeed() * deltaSeconds;
        v.setPosition(newPos);

        // Controllo velocità
        double limit = highway->getSpeedLimit();  // <<< CORRETTO
        if(v.getSpeed() > limit)
        {
            double fine = (v.getSpeed() - limit) * 5.0; // multa semplice
            tutor->addSanction(v.getPlate(), fine);
        }
    }

    cout << "[Simulator] Simulazione completata per " << deltaSeconds << " secondi\n";
}

void Simulator::reset()
{
    vector<Vehicle>& vehicles = tutor->getVehicles();
    for(Vehicle& v : vehicles)
        v.setPosition(0.0);

    cout << "[Simulator] Tutti i veicoli resettati\n";
}
