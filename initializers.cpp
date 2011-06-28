#include "common.h"

void initialize() {
    if (using_input_config_file)
        load_configuration_file();
    else {
        initialize_constants();
        initialize_waters();
    }
    initialize_rho_k_values_table();
    calculate_and_init_energy();

    initialize_radial_dist_sampler();

    print_all();

    if (!using_input_config_file)
        mc_equilibrate();

    return;
}

void initialize_constants() {
    BETA = 1.0 / (BOLTZMANN_K * TEMPERATURE);
    BOX_LENGTH = pow(NUM_WATERS / WATER_DENSITY, 1.0 / 3.0);
    initialize_erfc_table();
    initialize_k_values_table();
    return;
}

void initialize_waters() {
    water_positions = new double * [NUM_WATERS];
    double HOH_ANGLE_RAD = DEG2RAD(HOH_ANGLE_DEG);
    double tmp_r = OH_LENGTH * sin(HOH_ANGLE_RAD), rand_angle_rad;

    for (int i = 0; i < NUM_WATERS; i++) {
        water_positions[i] = new double[9];

        // Oxygen
        for (int j = 0; j < 3; j++)
            water_positions[i][j] = RAN3() * BOX_LENGTH;

        // First Hydrogen
        water_positions[i][3] = water_positions[i][0];
        water_positions[i][4] = water_positions[i][1];
        water_positions[i][5] = water_positions[i][2] + OH_LENGTH;

        // Second Hydrogen
        rand_angle_rad = 2.0 * M_PI * RAN3();
        water_positions[i][6] = water_positions[i][0] + tmp_r * cos(rand_angle_rad);
        water_positions[i][7] = water_positions[i][1] + tmp_r * sin(rand_angle_rad);
        water_positions[i][8] = water_positions[i][2] + OH_LENGTH * cos(HOH_ANGLE_RAD);
    }
    return;
}
