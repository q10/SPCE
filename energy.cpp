#include "common.h"

double energy_of_water_with_index(int index) {
    double dx, dy, dz, r, energy = 0.0;
    for (int i = 0; i < NUM_WATERS; i++) {
        if (i == index)
            continue;
        dx = abs(water_positions[index][0] - water_positions[i][0]);
        dy = abs(water_positions[index][1] - water_positions[i][1]);
        dz = abs(water_positions[index][2] - water_positions[i][2]);
        if (dx > HALF_BOX_LENGTH)
            dx = fmod(-dx, HALF_BOX_LENGTH);
        if (dy > HALF_BOX_LENGTH)
            dy = fmod(-dy, HALF_BOX_LENGTH);
        if (dz > HALF_BOX_LENGTH)
            dz = fmod(-dz, HALF_BOX_LENGTH);
        r = 1.0 / sqrt(dx * dx + dy * dy + dz * dz);
        energy += 4.0 * (pow(r, 12) - pow(r, 6));
    }
    return energy;
}

void calculate_energy() {
    double dx, dy, dz, r, tmp_energy = 0.0;
    for (int i = 0; i < NUM_WATERS - 1; i++) {
        for (int j = i + 1; j < NUM_WATERS; j++) {
            dx = abs(water_positions[i][0] - water_positions[j][0]);
            dy = abs(water_positions[i][1] - water_positions[j][1]);
            dz = abs(water_positions[i][2] - water_positions[j][2]);
            if (dx > HALF_BOX_LENGTH)
                dx = fmod(-dx, HALF_BOX_LENGTH);
            if (dy > HALF_BOX_LENGTH)
                dy = fmod(-dy, HALF_BOX_LENGTH);
            if (dz > HALF_BOX_LENGTH)
                dz = fmod(-dz, HALF_BOX_LENGTH);
            r = 1.0 / sqrt(dx * dx + dy * dy + dz * dz);
            tmp_energy += 4.0 * (pow(r, 12) - pow(r, 6));
        }
    }
    LJEnergy = tmp_energy;
    return;
}

void update_energy(double old_energy_diff, double new_energy_diff) {
    LJEnergy += (-old_energy_diff) + new_energy_diff;
    return;
}
