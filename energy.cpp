#include "common.h"

double energy_of_water_with_index(int index) {
    double energy = 0.0;
    for (int i = 0; i < NUM_WATERS; i++) {
        if (i != index) {
            energy += energy_between_two_waters(index, i);
        }
    }
    return energy;
}

void calculate_and_init_energy() {
    LJEnergy = 0.0;
    for (int i = 0; i < NUM_WATERS - 1; i++) {
        for (int j = i + 1; j < NUM_WATERS; j++) {
            LJEnergy += energy_between_two_waters(i, j);
        }
    }
    return;
}

inline double energy_between_two_waters(int i, int j) {
    static double dx, dy, dz, old_dx, old_dy, old_dz, r, r2, tmp_energy;
    static bool use_same_x, use_same_y, use_same_z;

    tmp_energy = 0.0;
    use_same_x = use_same_y = use_same_z = false;

    // Loop over atoms within the two waters
    for (int atom = 0; atom < 9; atom += 3) {
        for (int atom2 = 0; atom2 < 9; atom2 += 3) {
            dx = old_dx = abs(water_positions[i][atom] - water_positions[j][atom2]);
            dy = old_dy = abs(water_positions[i][atom + 1] - water_positions[j][atom2 + 1]);
            dz = old_dz = abs(water_positions[i][atom + 2] - water_positions[j][atom2 + 2]);

            // Fix the distances to use the same nearest mirror image for each molecule based on O-O distance
            if (atom == 0 and atom2 == 0) {
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

            // Covers the O-O case, the H-O/O-H case, and the H-H case for electrostatics
            if (atom == 0 and atom2 == 0) {
                r2 = WATER_SIGMA / r;
                tmp_energy += 4.0 * WATER_EPSILON * (pow(r2, 12) - pow(r2, 6));
                tmp_energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_O * (*ERFC_TABLE)[floor(r * 1000.0)] / r;
            } else if (atom == 0 or atom2 == 0)
                tmp_energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_H * (*ERFC_TABLE)[floor(r * 1000.0)] / r;
            else
                tmp_energy += ELECTROSTATIC_K * WATER_Q_H * WATER_Q_H * (*ERFC_TABLE)[floor(r * 1000.0)] / r;
        }
    }
    return tmp_energy;
}

void update_energy(double old_energy_diff, double new_energy_diff) {
    LJEnergy += (-old_energy_diff) + new_energy_diff;
    return;
}
