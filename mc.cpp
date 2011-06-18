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

// define function in .h file if using the inline function somewhere else other than this cpp file

inline void mc_sweep() {
    for (int i = 0; i < NUM_MC_ATTEMPTS_PER_SWEEP; i++) {
        if (RAN3() < 0.5)
            mc_move();
        else
            mc_rotate();
    }
    return;
}

inline void mc_move() {
    int rand_i = RANDINT(0, NUM_WATERS);
    double tmp_old_position[9], rand_displacement, old_energy_diff = energy_of_water_with_index(rand_i);

    // save old position 
    for (int g = 0; g < 9; g++)
        tmp_old_position[g] = water_positions[rand_i][g];

    // move to new position - the coords of H can be outside box limits provided that O is inside the box
    for (int j = 0; j < 3; j++) {
        rand_displacement = DISPLACEMENT * (2 * RAN3() - 1.0);
        water_positions[rand_i][j] += rand_displacement;
        water_positions[rand_i][j + 3] += rand_displacement;
        water_positions[rand_i][j + 6] += rand_displacement;

        if (water_positions[rand_i][j] > BOX_LENGTH) {
            water_positions[rand_i][j] -= BOX_LENGTH;
            water_positions[rand_i][j + 3] -= BOX_LENGTH;
            water_positions[rand_i][j + 6] -= BOX_LENGTH;
        }

        if (water_positions[rand_i][j] < 0) {
            water_positions[rand_i][j] += BOX_LENGTH;
            water_positions[rand_i][j + 3] += BOX_LENGTH;
            water_positions[rand_i][j + 6] += BOX_LENGTH;
        }
    }

    // calculate difference from new energy and attempt to move particle with acceptance probability
    double new_energy_diff = energy_of_water_with_index(rand_i);
    if (RAN3() < exp(-BETA * (new_energy_diff - old_energy_diff)))
        update_energy(old_energy_diff, new_energy_diff);
    else {
        // undo move if move not accepted
        for (int j = 0; j < 9; j++)
            water_positions[rand_i][j] = tmp_old_position[j];
    }
    return;
}

inline void mc_rotate() {
    return;
}
