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
    water_H1_positions = new double*[NUM_WATERS];
    water_H2_positions = new double*[NUM_WATERS];
    double HOH_ANGLE_RAD = DEG2RAD(HOH_ANGLE_DEG), tmp_r = OH_LENGTH * sin(HOH_ANGLE_RAD), rand_angle_rad;

    for (int i = 0; i < NUM_WATERS; i++) {
        water_O_positions[i] = new double[3];
        water_H1_positions[i] = new double[3];
        water_H2_positions[i] = new double[3];

        for (int j = 0; j < 3; j++)
            water_O_positions[i][j] = RAN3() * BOX_LENGTH;

        water_H1_positions[i][0] = water_O_positions[i][0];
        water_H1_positions[i][1] = water_O_positions[i][1];
        water_H1_positions[i][2] = water_O_positions[i][2] + OH_LENGTH;

        rand_angle_rad = 2.0 * M_PI * RAN3();
        water_H2_positions[i][0] = water_H2_positions[i][0] + tmp_r * cos(rand_angle_rad);
        water_H2_positions[i][1] = water_H2_positions[i][1] + tmp_r * sin(rand_angle_rad);
        water_H2_positions[i][2] = water_H2_positions[i][2] + OH_LENGTH * cos(HOH_ANGLE_RAD);
    }
    return;
}
