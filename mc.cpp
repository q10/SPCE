#include "common.h"

void run_mc() {
    for (int i = 0; i < NUM_MC_STEPS; i++) {
        int rand_i = RANDINT(0, NUM_WATERS);
        double old_energy_diff = energy_of_water_with_index(rand_i);
        double tmp_old_position[3] = {water_positions[rand_i][0], water_positions[rand_i][1], water_positions[rand_i][2]};
        for (int j = 0; j < 3; j++)
            water_positions[rand_i][j] = fmod(water_positions[rand_i][j] + (DISPLACEMENT * (RAN3() - 0.5)), BOX_LENGTH);

        // calculate difference from new energy and attempt to move particle with acceptance probability
        double new_energy_diff = energy_of_water_with_index(rand_i);
        if (RAN3() < MIN(1.0, exp(-((new_energy_diff - old_energy_diff) / TEMPERATURE))))
            update_energy(old_energy_diff, new_energy_diff);
        else {
            for (int j = 0; j < 3; j++)
                water_positions[rand_i][j] = tmp_old_position[j];
        }
    }
    return;
}