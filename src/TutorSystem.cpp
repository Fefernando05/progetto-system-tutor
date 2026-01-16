#include "TutorSystem.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>

TutorSystem::TutorSystem(const std::string& highwayFile,
                         const std::string& passagesFile)
{
    loadHighway(highwayFile);
    loadPassages(passagesFile);

    currentTime = 0.0;
    nextPassageIndex = 0;

    totalSanctioned = 0;
    countPerGate.assign(gates.size(), 0);
    totalSpeedSum = 0.0;
    totalSpeedCount = 0;
}

void TutorSystem::loadHighway(const std::string& filename)
{
    std::ifstream in(filename);
    if(!in)
        throw std::runtime_error("Errore apertura " + filename);

    double km;
    char type;

    while(in >> km >> type) {
        if(type == 'V')
            gates.push_back(km);
    }

    std::sort(gates.begin(), gates.end());
}

void TutorSystem::loadPassages(const std::string& filename)
{
    std::ifstream in(filename);
    if(!in)
        throw std::runtime_error("Errore apertura " + filename);

    PassageRecord p;
    while(in >> p.plate >> p.gateID >> p.time)
        passages.push_back(p);

    std::sort(passages.begin(), passages.end(),
              [](const PassageRecord& a, const PassageRecord& b){
                  return a.time < b.time;
              });
}

void TutorSystem::reset()
{
    currentTime = 0.0;
    nextPassageIndex = 0;
    lastSanctions.clear();

    totalSanctioned = 0;
    std::fill(countPerGate.begin(), countPerGate.end(), 0);
    totalSpeedSum = 0.0;
    totalSpeedCount = 0;
}

void TutorSystem::set_time(double deltaSeconds)
{
    lastSanctions.clear();
    double newTime = currentTime + deltaSeconds;

    // On traite tous les passages dans l'intervalle (currentTime, newTime]
    while(nextPassageIndex < passages.size() &&
          passages[nextPassageIndex].time <= newTime)
    {
        const PassageRecord& p = passages[nextPassageIndex];

        countPerGate[p.gateID]++;

        // Chercher un passage précédent du même véhicule
        for(size_t j = nextPassageIndex; j > 0; j--) {
            const PassageRecord& prev = passages[j-1];
            if(prev.plate == p.plate && prev.gateID < p.gateID) {

                double dt = p.time - prev.time;
                double dx = gates[p.gateID] - gates[prev.gateID];
                double speed = dx / (dt/3600.0);

                totalSpeedSum += speed;
                totalSpeedCount++;

                if(speed > 130.0) {
                    lastSanctions.push_back({
                        p.plate,
                        prev.gateID,
                        p.gateID,
                        speed,
                        prev.time,
                        p.time
                    });
                    totalSanctioned++;
                }
                break;
            }
        }

        nextPassageIndex++;
    }

    currentTime = newTime;
}

void TutorSystem::print_last_sanctions(std::ostream& out) const
{
    if(lastSanctions.empty()) {
        out << "Nessuna sanzione.\n";
        return;
    }

    for(const auto& s : lastSanctions) {
        out << "Targa: " << s.plate
            << " | Tratta: V" << s.gateA << "-V" << s.gateB
            << " | Velocita media: " << std::fixed << std::setprecision(2)
            << s.speed << " km/h"
            << " | Passaggi: " << s.tA << "s - " << s.tB << "s\n";
    }
}

void TutorSystem::print_stats(std::ostream& out) const
{
    out << "--- STATISTICHE ---\n";

    out << "Veicoli per varco:\n";
    for(size_t i=0; i<countPerGate.size(); i++) {
        out << "  V" << i << ": " << countPerGate[i] << "\n";
    }

    if(totalSpeedCount > 0) {
        out << "Velocita media globale: "
            << (totalSpeedSum / totalSpeedCount) << " km/h\n";
    } else {
        out << "Velocita media globale: n/d\n";
    }

    out << "Veicoli sanzionati: " << totalSanctioned << "\n";
}
