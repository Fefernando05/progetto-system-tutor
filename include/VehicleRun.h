#ifndef VEHICLERUN_H
#define VEHICLERUN_H

#include <string>
#include <vector>

class VehicleRun {
public:
    struct Segment {
        double speedKmh;
        double durationMin;
    };

    VehicleRun(const std::string& plate, int entry, int exit);

    void addSegment(double speed, double duration);
    void setStartTime(double t);

    const std::string& getPlate() const;
    int getEntry() const;
    int getExit() const;
    double getStartTime() const;
    const std::vector<Segment>& getSegments() const;

private:
    std::string plate;
    int entryIndex;
    int exitIndex;
    double startTime;
    std::vector<Segment> segments;
};

#endif
