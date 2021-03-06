#include "common.h"

double energy_of_particle_with_index(int index) {
    double energy = 0.0;
    if (index < NUM_WATERS) {
        for (int i = 0; i < NUM_WATERS; i++) {
            if (i != index)
                energy += energy_between_two_waters(index, i);
        }
        for (int i = 0; i < NUM_IONS; i++)
            energy += energy_between_ion_and_water(i, index);
    } else {
        index = index - NUM_WATERS;
        for (int i = 0; i < NUM_IONS; i++) {
            if (i != index)
                energy += energy_between_two_ions(index, i);
        }
        for (int i = 0; i < NUM_WATERS; i++)
            energy += energy_between_ion_and_water(index, i);
    }
    return energy;
}

void calculate_and_init_energy() {
    TOTAL_ENERGY = total_real_space_energy() + total_ewald_energy();
    return;
}

double total_real_space_energy() {
    double real_space_energy = 0.0;
    for (int i = 0; i < NUM_TOTAL_PARTICLES; i++)
        real_space_energy += energy_of_particle_with_index(i);
    return real_space_energy / 2.0;
}

double energy_between_ion_and_water(int i, int j) {
    double dx, dy, dz, old_dx, old_dy, old_dz, r, r2, tmp_energy = 0.0;
    bool use_same_x, use_same_y, use_same_z;

    for (int atom = 0; atom < 9; atom += 3) {
        dx = old_dx = abs(IONS[i][0] - WATER_POSITIONS[j][atom]);
        dy = old_dy = abs(IONS[i][1] - WATER_POSITIONS[j][atom + 1]);
        dz = old_dz = abs(IONS[i][2] - WATER_POSITIONS[j][atom + 2]);

        // Fix the distances to use the same nearest mirror image for each molecule based on O-O distance
        if (atom == 0) {
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

        if (atom == 0) {
            r2 = (WATER_SIGMA + ION_SIGMA) / (2 * r);
            tmp_energy += 4.0 * WATER_EPSILON * (pow(r2, 12) - pow(r2, 6));
            tmp_energy += ELECTROSTATIC_K * IONS[i][3] * WATER_Q_O * (*ERFC_TABLE)[floor(r * 1000.0)] / r;
        } else
            tmp_energy += ELECTROSTATIC_K * IONS[i][3] * WATER_Q_H * (*ERFC_TABLE)[floor(r * 1000.0)] / r;
    }
    return tmp_energy;
}

double energy_between_two_ions(int i, int j) {
    double dx, dy, dz, r, r2, tmp_energy = 0.0;
    dx = abs(IONS[i][0] - IONS[j][0]);
    dy = abs(IONS[i][1] - IONS[j][1]);
    dz = abs(IONS[i][2] - IONS[j][2]);
    dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
    dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
    dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
    r = sqrt(dx * dx + dy * dy + dz * dz);

    r2 = ION_SIGMA / r;
    tmp_energy += 4.0 * ION_EPSILON * (pow(r2, 12) - pow(r2, 6));
    return tmp_energy + ELECTROSTATIC_K * IONS[i][3] * IONS[j][3] * (*ERFC_TABLE)[floor(r * 1000.0)] / r;
}

double energy_between_two_waters(int i, int j) {
    double dx, dy, dz, old_dx, old_dy, old_dz, r, r2, tmp_energy = 0.0;
    bool use_same_x, use_same_y, use_same_z;

    use_same_x = use_same_y = use_same_z = false;

    // Loop over atoms within the two waters
    for (int atom = 0; atom < 9; atom += 3) {
        for (int atom2 = 0; atom2 < 9; atom2 += 3) {
            dx = old_dx = abs(WATER_POSITIONS[i][atom] - WATER_POSITIONS[j][atom2]);
            dy = old_dy = abs(WATER_POSITIONS[i][atom + 1] - WATER_POSITIONS[j][atom2 + 1]);
            dz = old_dz = abs(WATER_POSITIONS[i][atom + 2] - WATER_POSITIONS[j][atom2 + 2]);

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
