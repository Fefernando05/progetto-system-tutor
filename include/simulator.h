#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Highway.h"
#include "VehicleRun.h"
#include <string>

class Simulator {
public:
    Simulator(const std::string& highwayFile,
              const std::string& runsFile,
              const std::string& passagesFile,
              int numVehicles);

    void run();

private:
    Highway highway;
    std::string runsFilename;
    std::string passagesFilename;
    int numVehicles;

    double randDouble(double a, double b);
    int randInt(int a, int b);
    std::string randomPlate();

    VehicleRun generateRun();
    void writeRun(std::ofstream& out, const VehicleRun& v);
    void computePassages(const VehicleRun& v, std::ofstream& out);
};

#endif
