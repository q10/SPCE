#include "common.h"

int NUM_MC_ATTEMPTS_PER_SWEEP = 1000;
int NUM_MC_SWEEPS = 1000000;
int NUM_EQUILIBRATION_SWEEPS = 5000;

int num_successful_mc_moves = 0;
int num_successful_mc_rotations = 0;
int total_attempted_mc_moves = 0;
int total_attempted_mc_rotations = 0;

void reset_mc_acceptance_records() {
    num_successful_mc_moves = num_successful_mc_rotations = total_attempted_mc_moves = total_attempted_mc_rotations = 0;
    return;
}

void print_mc_acceptance_records() {
    cout << "Displacement distance (Angstroms): " << DISPLACEMENT_DISTANCE << endl
            << "Displacement rotation (radians): " << DISPLACEMENT_ROTATION << endl
            << "Move Acceptance Rate: " << setprecision(10)
            << 100.0 * (double) num_successful_mc_moves / (double) total_attempted_mc_moves << "%" << endl
            << "Rotation Acceptance Rate: "
            << 100.0 * (double) num_successful_mc_rotations / (double) total_attempted_mc_rotations << "%\n" << endl;
    return;
}

void run_mc() {
    int run_start = clock();
    for (int h = 0; h < NUM_MC_SWEEPS; h++) {
        mc_sweep();
        if (h % DATA_SAMPLING_RATE == 0)
            radial_dist_sample();
        cerr << "MC sweep " << h + 1 << " of " << NUM_MC_SWEEPS << " complete." << endl;
    }
    cerr << "MC run completed in " << setprecision(10) << ((double) (clock() - run_start) / (double) CLOCKS_PER_SEC) / 3600 << " hours." << endl;
    return;
}

void mc_equilibrate() {
    for (int h = 0; h < NUM_EQUILIBRATION_SWEEPS; h++) {
        mc_sweep();
        cerr << "MC equilibration sweep " << h + 1 << " of " << NUM_EQUILIBRATION_SWEEPS << " complete." << endl;
    }
    cerr << endl;
    return;
}

inline void mc_sweep() {
    int sweep_start = clock();
    for (int i = 0; i < NUM_MC_ATTEMPTS_PER_SWEEP; i++) {
        if (RAN3() < 0.5) {
            total_attempted_mc_moves++;
            mc_move();
        } else {
            total_attempted_mc_rotations++;
            mc_rotate();
        }
    }
    cerr << (double) (clock() - sweep_start) / (double) CLOCKS_PER_SEC << endl;
    return;
}

void mc_move() {
    int rand_i = RANDINT(0, NUM_TOTAL_PARTICLES);
    double old_position[9], rand_displacement, old_energy_particle_i = energy_of_particle_with_index(rand_i);
    bool is_water = (rand_i < NUM_WATERS);

    // save old position
    // move to new position - the coords of H can be outside box limits provided that O is inside the box
    if (is_water) {
        for (int g = 0; g < 9; g++)
            old_position[g] = WATER_POSITIONS[rand_i][g];

        for (int j = 0; j < 3; j++) {
            rand_displacement = DISPLACEMENT_DISTANCE * (2.0 * RAN3() - 1.0);
            WATER_POSITIONS[rand_i][j] += rand_displacement;
            WATER_POSITIONS[rand_i][j + 3] += rand_displacement;
            WATER_POSITIONS[rand_i][j + 6] += rand_displacement;
        }
    } else {
        int ion_index = rand_i - NUM_WATERS;
        for (int g = 0; g < 3; g++)
            old_position[g] = IONS[ion_index][g];

        for (int j = 0; j < 3; j++) {
            rand_displacement = DISPLACEMENT_DISTANCE * (2.0 * RAN3() - 1.0);
            IONS[ion_index][j] += rand_displacement;
        }
    }

    // make sure the oxygen/ion stays inside boundaries; otherwise shift appropriately
    keep_particle_inside_box(rand_i);

    // calculate difference from new energy and attempt to move particle with acceptance probability
    if (mc_accept(rand_i, old_energy_particle_i, old_position))
        num_successful_mc_moves++;
    return;
}

void mc_rotate() {
    int rand_i = RANDINT(0, NUM_WATERS);
    double old_position[9], old_energy_particle_i = energy_of_particle_with_index(rand_i);

    double rand_theta_rad = DISPLACEMENT_ROTATION * (2.0 * RAN3() - 1.0);
    set_center_of_mass_of_water_with_index(rand_i);
    set_rotation_matrix(RANDUNITVECTOR(), rand_theta_rad);

    // save old position 
    for (int g = 0; g < 9; g++)
        old_position[g] = WATER_POSITIONS[rand_i][g];

    // shift water such that its center of mass is now the origin (use the old_position set of coords)
    for (int g = 0; g < 9; g++)
        old_position[g] -= TMP_CENTER_OF_MASS[g % 3];

    // apply rotation matrix to all 9 coordinates of water
    for (int g = 0; g < 9; g += 3) {
        WATER_POSITIONS[rand_i][g] = ROTATION_MATRIX[0][0] * old_position[g] + ROTATION_MATRIX[0][1] * old_position[g + 1] + ROTATION_MATRIX[0][2] * old_position[g + 2];
        WATER_POSITIONS[rand_i][g + 1] = ROTATION_MATRIX[1][0] * old_position[g] + ROTATION_MATRIX[1][1] * old_position[g + 1] + ROTATION_MATRIX[1][2] * old_position[g + 2];
        WATER_POSITIONS[rand_i][g + 2] = ROTATION_MATRIX[2][0] * old_position[g] + ROTATION_MATRIX[2][1] * old_position[g + 1] + ROTATION_MATRIX[2][2] * old_position[g + 2];
    }

    // un-shift water (use the WATER_POSITIONS[rand_i]), and restore old_position to proper original coords
    for (int g = 0; g < 9; g++) {
        WATER_POSITIONS[rand_i][g] += TMP_CENTER_OF_MASS[g % 3];
        old_position[g] += TMP_CENTER_OF_MASS[g % 3];
    }
    // make sure the oxygen stays inside boundaries; otherwise shift appropriately
    keep_particle_inside_box(rand_i);

    // calculate difference from new energy and attempt to rotate particle with acceptance probability (use old_position set of coords)
    if (mc_accept(rand_i, old_energy_particle_i, old_position))
        num_successful_mc_rotations++;

    return;
}

inline void keep_particle_inside_box(int index) {
    if (index >= NUM_WATERS) {
        index =  index - NUM_WATERS;
        for (int j = 0; j < 3; j++) {
            if (IONS[index][j] > BOX_LENGTH)
                IONS[index][j] -= BOX_LENGTH;
            if (IONS[index][j] < 0.0)
                IONS[index][j] += BOX_LENGTH;
        }
    } else {
        for (int j = 0; j < 3; j++) {
            if (WATER_POSITIONS[index][j] > BOX_LENGTH) {
                WATER_POSITIONS[index][j] -= BOX_LENGTH;
                WATER_POSITIONS[index][j + 3] -= BOX_LENGTH;
                WATER_POSITIONS[index][j + 6] -= BOX_LENGTH;
            }

            if (WATER_POSITIONS[index][j] < 0.0) {
                WATER_POSITIONS[index][j] += BOX_LENGTH;
                WATER_POSITIONS[index][j + 3] += BOX_LENGTH;
                WATER_POSITIONS[index][j + 6] += BOX_LENGTH;
            }
        }
    }
    return;
}

bool mc_accept(int index, double old_energy_particle_i, double * old_position) {
    double total_energy_diff = ewald_diff(index) + energy_of_particle_with_index(index) - old_energy_particle_i;

    if (RAN3() < exp(-BETA * total_energy_diff))
        update_energy(total_energy_diff);
    else {
        // undo the move if move not accepted
        if (index >= NUM_WATERS)
            for (int j = 0; j < 3; j++)
                IONS[index - NUM_WATERS][j] = old_position[j];
        else
            for (int j = 0; j < 9; j++)
                WATER_POSITIONS[index][j] = old_position[j];

        // reset partial rho_k's
        dcomplex *column;
        for (int k = 0; k < 725; k++) {
            column = RHO_K_VALUES[k];
            column[NUM_TOTAL_PARTICLES] += column[NUM_TOTAL_PARTICLES + 1] - column[index];
            column[index] = column[NUM_TOTAL_PARTICLES + 1];
        }
        return false;
    }
    return true;
}
