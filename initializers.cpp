#include "common.h"

void initialize() {
    initialize_constants();
    initialize_water_positions_and_energies();
    initialize_radial_dist_sampler();
    return;
}

void initialize_constants() {
    water_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++)
        water_positions[i] = new double[3];

    BOX_LENGTH = pow(NUM_WATERS / WATER_DENSITY, 1.0 / 3.0);
    LJEnergy = 0.0;
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
