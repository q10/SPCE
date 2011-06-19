#include "common.h"

int NUM_MC_ATTEMPTS_PER_SWEEP = 2000;
int NUM_MC_SWEEPS = 100000;
int NUM_EQUILIBRATION_SWEEPS = 5000;

void run_mc() {
    for (int h = 0; h < NUM_MC_SWEEPS; h++) {
        mc_sweep();
        if (h % DATA_SAMPLING_RATE == 0)
            radial_dist_sample();
    }
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
    for (int i = 0; i < NUM_MC_ATTEMPTS_PER_SWEEP; i++) {
        if (RAN3() < 0.5)
            mc_move();
        else
            mc_rotate();
    }
    return;
}

void mc_move() {
    int rand_i = RANDINT(0, NUM_WATERS);
    double old_position[9], rand_displacement, old_energy_diff = energy_of_water_with_index(rand_i);

    // save old position 
    for (int g = 0; g < 9; g++)
        old_position[g] = water_positions[rand_i][g];

    // move to new position - the coords of H can be outside box limits provided that O is inside the box
    for (int j = 0; j < 3; j++) {
        rand_displacement = DISPLACEMENT * (2.0 * RAN3() - 1.0);
        water_positions[rand_i][j] += rand_displacement;
        water_positions[rand_i][j + 3] += rand_displacement;
        water_positions[rand_i][j + 6] += rand_displacement;
    }

    // make sure the oxygen stays inside boundaries; otherwise shift appropriately
    keep_water_inside_box(rand_i);

    // calculate difference from new energy and attempt to move particle with acceptance probability
    mc_accept(rand_i, old_energy_diff, old_position);
    return;
}

void mc_rotate() {
    int rand_i = RANDINT(0, NUM_WATERS);
    double old_position[9], old_energy_diff = energy_of_water_with_index(rand_i);

    double rand_theta_rad = 0.3 * M_PI * (2.0 * RAN3() - 1.0);
    double * center_of_mass = center_of_mass_of_water_with_index(rand_i);
    double ** rot_matrix = rotation_matrix(RANDUNITVECTOR(), rand_theta_rad);

    // save old position 
    for (int g = 0; g < 9; g++)
        old_position[g] = water_positions[rand_i][g];

    // shift water such that its center of mass is now the origin (use the old_position set of coords)
    for (int g = 0; g < 9; g++)
        old_position[g] -= center_of_mass[g % 3];

    // apply rotation matrix to all 9 coordinates of water
    for (int g = 0; g < 9; g += 3) {
        water_positions[rand_i][g] = rot_matrix[0][0] * old_position[g] + rot_matrix[0][1] * old_position[g + 1] + rot_matrix[0][2] * old_position[g + 2];
        water_positions[rand_i][g + 1] = rot_matrix[1][0] * old_position[g] + rot_matrix[1][1] * old_position[g + 1] + rot_matrix[1][2] * old_position[g + 2];
        water_positions[rand_i][g + 2] = rot_matrix[2][0] * old_position[g] + rot_matrix[2][1] * old_position[g + 1] + rot_matrix[2][2] * old_position[g + 2];
    }

    // un-shift water (use the water_positions[rand_i] set of coords)
    for (int g = 0; g < 9; g++)
        water_positions[rand_i][g] += center_of_mass[g % 3];

    // make sure the oxygen stays inside boundaries; otherwise shift appropriately
    keep_water_inside_box(rand_i);

    // calculate difference from new energy and attempt to rotate particle with acceptance probability
    mc_accept(rand_i, old_energy_diff, old_position);

    return;
}

inline void keep_water_inside_box(int index) {
    for (int j = 0; j < 3; j++) {
        if (water_positions[index][j] > BOX_LENGTH) {
            water_positions[index][j] -= BOX_LENGTH;
            water_positions[index][j + 3] -= BOX_LENGTH;
            water_positions[index][j + 6] -= BOX_LENGTH;
        }

        if (water_positions[index][j] < 0) {
            water_positions[index][j] += BOX_LENGTH;
            water_positions[index][j + 3] += BOX_LENGTH;
            water_positions[index][j + 6] += BOX_LENGTH;
        }
    }
    return;
}

inline void mc_accept(int index, double old_energy_diff, double * old_position) {
    double new_energy_diff = energy_of_water_with_index(index);

    if (RAN3() < exp(-BETA * (new_energy_diff - old_energy_diff)))
        update_energy(old_energy_diff, new_energy_diff);
    else {
        // undo the move if move not accepted
        for (int j = 0; j < 9; j++)
            water_positions[index][j] = old_position[j];
    }

    return;
}
