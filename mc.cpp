#include "common.h"

void run_mc() {
    double BETA = 1.0 / (BOLTZMANN_K * TEMPERATURE);
    for (int i = 0; i < NUM_MC_CYCLES; i++) {
        int rand_i = RANDINT(0, NUM_WATERS);
        double old_energy_diff = energy_of_water_with_index(rand_i);
        double tmp_old_position[3] = {water_positions[rand_i][0], water_positions[rand_i][1], water_positions[rand_i][2]};
        for (int j = 0; j < 3; j++)
            water_positions[rand_i][j] = fmod(water_positions[rand_i][j] + (DISPLACEMENT * (RAN3() - 0.5)), BOX_LENGTH);

        // calculate difference from new energy and attempt to move particle with acceptance probability
        double new_energy_diff = energy_of_water_with_index(rand_i);
        if (RAN3() < exp(-BETA * (new_energy_diff - old_energy_diff)))
            update_energy(old_energy_diff, new_energy_diff);
        else {
            // undo move if move not accepted
            for (int j = 0; j < 3; j++)
                water_positions[rand_i][j] = tmp_old_position[j];
        }
        
        // data sampling
        if (i > EQUILIBRATION_TIME && i % DATA_SAMPLING_RATE == 0)
            radial_dist_sample();
    }
    return;
}
