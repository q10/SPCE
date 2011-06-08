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
    return;
}

// define function in .h file if using the inline function somewhere else other than this cpp file
inline void mc_sweep() {
    for (int i = 0; i < NUM_MC_ATTEMPTS_PER_SWEEP; i++) {
        int rand_i = RANDINT(0, NUM_WATERS);
        double old_energy_diff = energy_of_water_with_index(rand_i);
        double tmp_old_position[3] = {water_O_positions[rand_i][0], water_O_positions[rand_i][1], water_O_positions[rand_i][2]};
        for (int j = 0; j < 3; j++)
            water_O_positions[rand_i][j] = fmod(water_O_positions[rand_i][j] + (DISPLACEMENT * (RAN3() - 0.5)), BOX_LENGTH);

        // calculate difference from new energy and attempt to move particle with acceptance probability
        double new_energy_diff = energy_of_water_with_index(rand_i);
        if (RAN3() < exp(-BETA * (new_energy_diff - old_energy_diff)))
            update_energy(old_energy_diff, new_energy_diff);
        else {
            // undo move if move not accepted
            for (int j = 0; j < 3; j++)
                water_O_positions[rand_i][j] = tmp_old_position[j];
        }
    }
    return;
}
