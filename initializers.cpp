#include "common.h"

void initialize() {
    if (using_input_config_file)
        load_configuration_file();
    else {
        initialize_constants();
        initialize_waters();
    }
    calculate_energy();

    initialize_radial_dist_sampler();
    
    print_system_vars();
    
    if (!using_input_config_file)
        mc_equilibrate();
    
    return;
}

void initialize_constants() {
    BETA = 1.0 / (BOLTZMANN_K * TEMPERATURE);
    BOX_LENGTH = pow(NUM_WATERS / WATER_DENSITY, 1.0 / 3.0);
    LJEnergy = 0.0;
    return;
}

void initialize_waters() {
    water_O_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++) {
        water_O_positions[i] = new double[3];
        for (int j = 0; j < 3; j++)
            water_O_positions[i][j] = RAN3() * BOX_LENGTH;
    }
    return;
}
