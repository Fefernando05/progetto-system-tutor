#ifndef TUTORSYSTEM_H
#define TUTORSYSTEM_H

#include <string>
#include <vector>
#include <iostream>

struct PassageRecord {
    std::string plate;
    int gateID;
    double time;
};

class TutorSystem {
public:
    TutorSystem(const std::string& highwayFile,
                const std::string& passagesFile);

    void set_time(double deltaSeconds);
    void reset();
    void print_last_sanctions(std::ostream& out) const;
    void print_stats(std::ostream& out) const;

private:
    std::vector<double> gates;          // km des varchi
    std::vector<PassageRecord> passages;

    double currentTime;
    size_t nextPassageIndex;

    struct Sanction {
        std::string plate;
        int gateA, gateB;
        double speed;
        double tA, tB;
    };

    std::vector<Sanction> lastSanctions;

    int totalSanctioned;
    std::vector<int> countPerGate;
    double totalSpeedSum;
    int totalSpeedCount;

    void loadHighway(const std::string& filename);
    void loadPassages(const std::string& filename);
};

#endif
