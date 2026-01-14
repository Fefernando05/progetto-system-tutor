/*
 * Vehicle.h
 * Classe per rappresentare un veicolo
 * Autore: Fernand Dongmo
 */

#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

class Vehicle
{
private:
    std::string plate;   // Targa del veicolo
    double speed;        // Velocità in m/s
    int lane;            // Corsia dell'autostrada
    double position;     // Posizione lungo la corsia

public:
    // Constructeur corrigé pour accepter la lane
    Vehicle(const std::string& plateNumber, double speedValue, int laneNumber)
        : plate(plateNumber), speed(speedValue), lane(laneNumber), position(0.0) {}

    std::string getPlate() const { return plate; }
    double getSpeed() const { return speed; }
    int getLane() const { return lane; }
    double getPosition() const { return position; }

    void setSpeed(double s) { speed = s; }
    void setLane(int l) { lane = l; }
    void setPosition(double p) { position = p; }
};

#endif

