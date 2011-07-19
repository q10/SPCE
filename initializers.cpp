#include "common.h"

void initialize() {
    if (using_input_config_file)
        load_configuration_file();
    else {
        initialize_prog_flag_affected_constants();
        initialize_waters();
        initialize_ions();
    }
    initialize_other_constants();
    initialize_other_variables();

    initialize_radial_dist_sampler();

    print_all();

    if (!using_input_config_file)
        mc_equilibrate();

    return;
}

void initialize_prog_flag_affected_constants() {
    BETA = 1.0 / (BOLTZMANN_K * TEMPERATURE);
    NUM_TOTAL_PARTICLES = NUM_WATERS + NUM_IONS;
    BOX_LENGTH = pow(NUM_TOTAL_PARTICLES / WATER_DENSITY, 1.0 / 3.0);
    BOX_VOLUME = BOX_LENGTH * BOX_LENGTH * BOX_LENGTH;
    return;
}

void initialize_other_constants() {
    initialize_erfc_table();
    initialize_k_vectors_table();
    return;
}

void initialize_other_variables() {
    initialize_rotation_matrix();
    initialize_rho_k_values_table();
    calculate_and_init_energy();
    return;
}

void initialize_waters() {
    WATER_POSITIONS = new double * [NUM_WATERS];
    double HOH_ANGLE_RAD = DEG2RAD(HOH_ANGLE_DEG);
    double tmp_r = OH_LENGTH * sin(HOH_ANGLE_RAD), rand_angle_rad;

    for (int i = 0; i < NUM_WATERS; i++) {
        WATER_POSITIONS[i] = new double[9];

        // Oxygen
        for (int j = 0; j < 3; j++)
            WATER_POSITIONS[i][j] = RAN3() * BOX_LENGTH;

        // First Hydrogen
        WATER_POSITIONS[i][3] = WATER_POSITIONS[i][0];
        WATER_POSITIONS[i][4] = WATER_POSITIONS[i][1];
        WATER_POSITIONS[i][5] = WATER_POSITIONS[i][2] + OH_LENGTH;

        // Second Hydrogen
        rand_angle_rad = 2.0 * M_PI * RAN3();
        WATER_POSITIONS[i][6] = WATER_POSITIONS[i][0] + tmp_r * cos(rand_angle_rad);
        WATER_POSITIONS[i][7] = WATER_POSITIONS[i][1] + tmp_r * sin(rand_angle_rad);
        WATER_POSITIONS[i][8] = WATER_POSITIONS[i][2] + OH_LENGTH * cos(HOH_ANGLE_RAD);
    }
    return;
}

void initialize_ions() {
    IONS = new double * [NUM_IONS];
    for (int i = 0; i < NUM_IONS; i++) {
        IONS[i] = new double[4];
        
        // ion position
        for (int j = 0; j < 3; j++)
            IONS[i][j] = RAN3() * BOX_LENGTH;
        
        // ion charge
        IONS[i][3] = (RAN3() < 0.5) ? 1.0 : -1.0;
    }
    return;
}
