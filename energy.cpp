#include "common.h"

double energy_of_water_with_index(int index) {
    double dx, dy, dz, old_dx, old_dy, old_dz, r, r2, energy = 0.0;
    for (int i = 0; i < NUM_WATERS; i++) {
        if (i != index) {
            bool use_same_x = false, use_same_y = false, use_same_z = false;
            
            // Loop over atoms within the two waters
            for (int skip = 0; skip < 9; skip += 3) {
                for (int skip2 = 0; skip2 < 9; skip += 3) {
                    dx = old_dx = abs(water_positions[index][skip] - water_positions[i][skip2]);
                    dy = old_dy = abs(water_positions[index][skip + 1] - water_positions[i][skip2 + 1]);
                    dz = old_dz = abs(water_positions[index][skip + 2] - water_positions[i][skip2 + 2]);

                    if (skip == 0 and skip2 == 0) {
                        dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
                        dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
                        dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
                        if (dx == old_dx)
                            use_same_x = true;
                        if (dy == old_dy)
                            use_same_y = true;
                        if (dz == old_dz)
                            use_same_z = true;
                    } else {
                        if (!use_same_x)
                            dx = BOX_LENGTH - old_dx;
                        if (!use_same_y)
                            dy = BOX_LENGTH - old_dy;
                        if (!use_same_z)
                            dz = BOX_LENGTH - old_dz;
                    }
                    
                    r = sqrt(dx * dx + dy * dy + dz * dz);                    
                    
                    // Covers the O-O case, the H-O/O-H case, and the H-H case
                    if (skip == 0 and skip2 == 0) {                        
                        r2 = WATER_SIGMA / r;
                        energy += 4.0 * WATER_EPSILON * (pow(r2, 12) - pow(r2, 6));
                        energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_O / r;
                    } else if (skip == 0 or skip2 == 0)
                        energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_H / r;
                    else
                        energy += ELECTROSTATIC_K * WATER_Q_H * WATER_Q_H / r;                    
                }
            }
        }
    }
    return energy;
}

void calculate_energy() {
    double dx, dy, dz, old_dx, old_dy, old_dz, r, r2, tmp_energy = 0.0;

    // Loop over waters
    for (int i = 0; i < NUM_WATERS - 1; i++) {
        for (int j = i + 1; j < NUM_WATERS; j++) {
            bool use_same_x = false, use_same_y = false, use_same_z = false;

            // Loop over atoms within the two waters
            for (int skip = 0; skip < 9; skip += 3) {
                for (int skip2 = 0; skip2 < 9; skip += 3) {
                    dx = old_dx = abs(water_positions[i][skip] - water_positions[j][skip2]);
                    dy = old_dy = abs(water_positions[i][skip + 1] - water_positions[j][skip2 + 1]);
                    dz = old_dz = abs(water_positions[i][skip + 2] - water_positions[j][skip2 + 2]);

                    // Fix the distances to use the same nearest mirror image for each molecule based on O-O distance
                    if (skip == 0 and skip2 == 0) {
                        dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
                        dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
                        dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
                        if (dx == old_dx)
                            use_same_x = true;
                        if (dy == old_dy)
                            use_same_y = true;
                        if (dz == old_dz)
                            use_same_z = true;
                    } else {
                        if (!use_same_x)
                            dx = BOX_LENGTH - old_dx;
                        if (!use_same_y)
                            dy = BOX_LENGTH - old_dy;
                        if (!use_same_z)
                            dz = BOX_LENGTH - old_dz;
                    }
                    
                    r = sqrt(dx * dx + dy * dy + dz * dz);
                    
                    // Covers the O-O case, the H-O/O-H case, and the H-H case (LJ is only counted in O-O case
                    if (skip == 0 and skip2 == 0) {                        
                        r2 = WATER_SIGMA / r;
                        tmp_energy += 4.0 * WATER_EPSILON * (pow(r2, 12) - pow(r2, 6));
                        tmp_energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_O / r;
                    } else if (skip == 0 or skip2 == 0)
                        tmp_energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_H / r;
                    else
                        tmp_energy += ELECTROSTATIC_K * WATER_Q_H * WATER_Q_H / r;                    
                }
            }
        }
    }
    LJEnergy = tmp_energy;
    return;
}

void update_energy(double old_energy_diff, double new_energy_diff) {
    LJEnergy += (-old_energy_diff) + new_energy_diff;
    return;
}
