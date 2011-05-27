#include "common.h"

void initialize() {
    LJEnergy = 0.0;
    initialize_water_positions_and_energies();
    initialize_radial_dist_sampler();
    return;
}

void initialize_water_positions_and_energies() {
    for (int i = 0; i < NUM_WATERS; i++) {
        for (int j = 0; j < 3; j++) {
            water_positions[i][j] = RAN3() * BOX_LENGTH;
        }
    }
    return;
}
