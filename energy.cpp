#include "common.h"

double energy_of_water_with_index(int index) {
    double dx, dy, dz, r, energy = 0.0;
    for (int i = 0; i < NUM_WATERS; i++) {
        if (i != index) {
            dx = abs(water_positions[index][0] - water_positions[i][0]);
            dy = abs(water_positions[index][1] - water_positions[i][1]);
            dz = abs(water_positions[index][2] - water_positions[i][2]);
            dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
            dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
            dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
            r = WATER_SIGMA / sqrt(dx * dx + dy * dy + dz * dz);
            energy += 4.0 * WATER_EPSILON * (pow(r, 12) - pow(r, 6));
        }
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
            dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
            dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
            dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
            r = WATER_SIGMA / sqrt(dx * dx + dy * dy + dz * dz);
            tmp_energy += 4.0 * WATER_EPSILON * (pow(r, 12) - pow(r, 6));
        }
    }
    LJEnergy = tmp_energy;
    return;
}

void update_energy(double old_energy_diff, double new_energy_diff) {
    LJEnergy += (-old_energy_diff) + new_energy_diff;
    return;
}
