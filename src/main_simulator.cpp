#include "Simulator.h"

int main() {
    Simulator sim("Data/Highway.txt",
                  "Data/Runs.txt",
                  "Data/Passages.txt",
                  10000);

    sim.run();
    return 0;
}
