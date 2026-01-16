#include "VehicleRun.h"

VehicleRun::VehicleRun(const std::string& p, int e, int x)
    : plate(p), entryIndex(e), exitIndex(x), startTime(0.0)
{}

void VehicleRun::addSegment(double speed, double duration) {
    segments.push_back({speed, duration});
}

void VehicleRun::setStartTime(double t) {
    startTime = t;
}

const std::string& VehicleRun::getPlate() const { return plate; }
int VehicleRun::getEntry() const { return entryIndex; }
int VehicleRun::getExit() const { return exitIndex; }
double VehicleRun::getStartTime() const { return startTime; }
const std::vector<VehicleRun::Segment>& VehicleRun::getSegments() const { return segments; }
