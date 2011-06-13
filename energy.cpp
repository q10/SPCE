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
    static double dx, dy, dz, r, r2, tmp_energy, **coords = new double*[2];
    for (int k = 0; k < 2; k++)
        coords[k] = new double[9];
    tmp_energy = 0.0;

    // Set the coordinates in nearest mirror image convention in real non-modular space
    get_corrected_coords(coords, i, j);

    // Loop over atoms within the two waters
    for (int atom = 0; atom < 9; atom += 3) {
        for (int atom2 = 0; atom2 < 9; atom2 += 3) {
            dx = abs(coords[0][atom] - coords[1][atom2]);
            dy = abs(coords[0][atom + 1] - coords[1][atom2 + 1]);
            dz = abs(coords[0][atom + 2] - coords[1][atom2 + 2]);

            r = sqrt(dx * dx + dy * dy + dz * dz);

            // Covers the O-O case, the H-O/O-H case, and the H-H case
            if (atom == 0 and atom2 == 0) {
                r2 = WATER_SIGMA / r;
                tmp_energy += 4.0 * WATER_EPSILON * (pow(r2, 12) - pow(r2, 6));
                tmp_energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_O / r;
            } else if (atom == 0 or atom2 == 0)
                tmp_energy += ELECTROSTATIC_K * WATER_Q_O * WATER_Q_H / r;
            else
                tmp_energy += ELECTROSTATIC_K * WATER_Q_H * WATER_Q_H / r;
        }
    }
    return tmp_energy;
}

inline void get_corrected_coords(double ** coords, int i, int j) {
    static double dOOr;

    // Copy coordinates over for work
    for (int h = 0; h < 9; h++)
        coords[0][h] = water_positions[i][h];
    for (int h = 0; h < 9; h++)
        coords[1][h] = water_positions[j][h];

    // Fix the O-O distance in non-mod space such that nearest mirror image convention is obeyed
    for (int m = 0; m < 3; m++) {
        dOOr = coords[i][m] - coords[j][m];
        if (dOOr > HALF_BOX_LENGTH)
            coords[i][m] -= BOX_LENGTH;
        if (dOOr < -HALF_BOX_LENGTH)
            coords[i][m] += BOX_LENGTH;
    }

    // Fix the H's accordingly, so they don't appear to be disconnected from their oxygens
    for (int m = 3; m < 9; m++) {
        while (coords[i][m] - coords[i][m % 3] > OH_LENGTH)
            coords[i][m] -= BOX_LENGTH;
        while (coords[i][m] - coords[i][m % 3] < -OH_LENGTH)
            coords[i][m] += BOX_LENGTH;
        
        while (coords[j][m] - coords[j][m % 3] > OH_LENGTH)
            coords[j][m] -= BOX_LENGTH;
        while (coords[j][m] - coords[j][m % 3] < -OH_LENGTH)
            coords[j][m] += BOX_LENGTH;
    }
    return;
}

void update_energy(double old_energy_diff, double new_energy_diff) {
    LJEnergy += (-old_energy_diff) + new_energy_diff;
    return;
}
