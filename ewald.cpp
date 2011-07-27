#include "common.h"

dcomplex exp_kr_O[3][11], exp_kr_H1[3][11], exp_kr_H2[3][11], exp_kr_ion[3][11];
dcomplex COMPLEX_ONE(1.0, 0.0);

void initialize_erfc_table() {
    double sqrt_alpha_1000 = sqrt(EWALD_ALPHA) / 1000.0, key;
    for (double r = 0.0; r < HALF_BOX_LENGTH; r += 0.001) {
        // to optimize map search by using one less division, we keep the keys as x10^4
        key = floor(r * 1000.0);
        (*ERFC_TABLE)[key] = erfc(key * sqrt_alpha_1000);
    }
    return;
}

void initialize_k_vectors_table() {
    // K vector order as follows (go through all ny and nzvalues descending, then ascending):    
    double tmp_k2, half_factor, alpha_inv_4 = -1.0 / (4.0 * EWALD_ALPHA), four_pi_volume_ek = 4.0 * ELECTROSTATIC_K * M_PI / BOX_VOLUME;
    int k = 0;

    for (int nx = 0; nx <= 5; nx++) {
        for (int ny = -5; ny <= 5; ny++) {
            for (int nz = -5; nz <= 5; nz++) {
                if (nx != 0 || ny != 0 || nz != 0) { // removes the K=0 case
                    K_VECTORS[k] = new double[4];
                    K_VECTORS[k][0] = 2.0 * M_PI * nx / BOX_LENGTH;
                    K_VECTORS[k][1] = 2.0 * M_PI * ny / BOX_LENGTH;
                    K_VECTORS[k][2] = 2.0 * M_PI * nz / BOX_LENGTH;

                    // k_entry[3] will be the Fourier coefficient exp(-K^2 / (4*alpha)) / K^2, where K^2, or kx^2 + ky^2 + kz^2
                    tmp_k2 = K_VECTORS[k][0] * K_VECTORS[k][0] + K_VECTORS[k][1] * K_VECTORS[k][1] + K_VECTORS[k][2] * K_VECTORS[k][2];
                    K_VECTORS[k][3] = exp(tmp_k2 * alpha_inv_4) / tmp_k2;

                    // placing the 4*pi/V here allows for smaller number of multiplications and divisions later on
                    // half_factor accounts for double weighting of Fourier coefficients along the nx=0 plane
                    half_factor = (nx == 0) ? 0.5 : 1.0;
                    K_VECTORS[k][3] *= half_factor * four_pi_volume_ek;
                    k++;
                }
            }
        }
    }
    return;
}

void initialize_rho_k_values_table() {
    dcomplex *column, column_sum;

    for (int k = 0; k < 725; k++) {
        column = new dcomplex [NUM_TOTAL_PARTICLES + 2];
        column_sum = dcomplex(0.0, 0.0);

        for (int w = 0; w < NUM_TOTAL_PARTICLES; w++) {
            column[w] = partial_rho(w, K_VECTORS[k]);
            column_sum += column[w];
        }

        column[NUM_TOTAL_PARTICLES] = column_sum;
        RHO_K_VALUES[k] = column;
    }

    for (int i = 0; i < 3; i++)
        exp_kr_O[i][5] = exp_kr_H1[i][5] = exp_kr_H2[i][5] = exp_kr_ion[i][5] = COMPLEX_ONE;

    return;
}

dcomplex partial_rho(int index, double * k_coords) {
    double rx, ry, rz, q;
    dcomplex part_rho(0.0, 0.0);

    if (index >= NUM_WATERS) {
        int ions_index = index - NUM_WATERS;
        rx = IONS[ions_index][0];
        ry = IONS[ions_index][1];
        rz = IONS[ions_index][2];
        part_rho = IONS[ions_index][3] * exp(dcomplex(0.0, k_coords[0] * rx + k_coords[1] * ry + k_coords[2] * rz));
    } else {
        for (int atom = 0; atom < 9; atom += 3) {
            q = (atom == 0) ? WATER_Q_O : WATER_Q_H;
            rx = WATER_POSITIONS[index][atom];
            ry = WATER_POSITIONS[index][atom + 1];
            rz = WATER_POSITIONS[index][atom + 2];
            part_rho += q * exp(dcomplex(0.0, k_coords[0] * rx + k_coords[1] * ry + k_coords[2] * rz));
        }
    }
    return part_rho;
}

double total_ewald_energy() {
    double ewald_energy = 0.0;
    for (int k = 0; k < 725; k++) {
        ewald_energy += norm(RHO_K_VALUES[k][NUM_TOTAL_PARTICLES]) * K_VECTORS[k][3];
    }
    return ewald_energy;
}

double ewald_diff(int index) {
    return (index >= NUM_WATERS) ? ewald_diff_ion(index) : ewald_diff_water(index);
}

double ewald_diff_water(int water_index) {
    set_exp_kr_table_for_water(water_index);
    double sum_of_ewald_diffs = 0.0, old_pk2;
    dcomplex *column, tmp_x_O, tmp_y_O, tmp_x_H1, tmp_y_H1, tmp_x_H2, tmp_y_H2;
    int k = 0;

    for (int nx = 0; nx <= 5; nx++) {
        tmp_x_O = WATER_Q_O * exp_kr_O[0][nx];
        tmp_x_H1 = WATER_Q_H * exp_kr_H1[0][nx];
        tmp_x_H2 = WATER_Q_H * exp_kr_H2[0][nx];

        for (int ny = 0; ny <= 10; ny++) {
            tmp_y_O = tmp_x_O * exp_kr_O[1][ny];
            tmp_y_H1 = tmp_x_H1 * exp_kr_H1[1][ny];
            tmp_y_H2 = tmp_x_H2 * exp_kr_H2[1][ny];

            for (int nz = 0; nz <= 10; nz++) {
                if (nx != 0 || ny != 5 || nz != 5) {
                    column = RHO_K_VALUES[k];
                    old_pk2 = norm(column[NUM_TOTAL_PARTICLES]);

                    // save old rho(K, R)
                    // calculate and save new rho(K, R)
                    // update total rho to rho(K, R)_new - rho(K, R)_old
                    column[NUM_TOTAL_PARTICLES + 1] = column[water_index];
                    column[water_index] = tmp_y_O * exp_kr_O[2][nz] + tmp_y_H1 * exp_kr_H1[2][nz] + tmp_y_H2 * exp_kr_H2[2][nz];
                    column[NUM_TOTAL_PARTICLES] += column[water_index] - column[NUM_TOTAL_PARTICLES + 1];

                    sum_of_ewald_diffs += (norm(column[NUM_TOTAL_PARTICLES]) - old_pk2) * K_VECTORS[k][3];
                    k++;
                }
            }
        }
    }
    return sum_of_ewald_diffs;
}

double ewald_diff_ion(int index) {
    int k = 0, ion_index = index - NUM_WATERS;
    set_exp_kr_table_for_ion(ion_index);
    double sum_of_ewald_diffs = 0.0, old_pk2;
    dcomplex *column, tmp_x_ion, tmp_y_ion;

    for (int nx = 0; nx <= 5; nx++) {
        tmp_x_ion = IONS[ion_index][3] * exp_kr_ion[0][nx];

        for (int ny = 0; ny <= 10; ny++) {
            tmp_y_ion = tmp_x_ion * exp_kr_ion[1][ny];

            for (int nz = 0; nz <= 10; nz++) {
                if (nx != 0 || ny != 5 || nz != 5) {
                    column = RHO_K_VALUES[k];
                    old_pk2 = norm(column[NUM_TOTAL_PARTICLES]);

                    // save old rho(K, R)
                    // calculate and save new rho(K, R)
                    // update total rho to rho(K, R)_new - rho(K, R)_old
                    column[NUM_TOTAL_PARTICLES + 1] = column[index];
                    column[index] = tmp_y_ion * exp_kr_ion[2][nz];
                    column[NUM_TOTAL_PARTICLES] += column[index] - column[NUM_TOTAL_PARTICLES + 1];

                    sum_of_ewald_diffs += (norm(column[NUM_TOTAL_PARTICLES]) - old_pk2) * K_VECTORS[k][3];
                    k++;
                }
            }
        }
    }
    return sum_of_ewald_diffs;
}

void set_exp_kr_table_for_water(int water_index) {
    for (int i = 0; i < 3; i++) {
        exp_kr_O[i][6] = exp(dcomplex(0.0, K_VECTORS[71][i] * WATER_POSITIONS[water_index][i]));
        exp_kr_H1[i][6] = exp(dcomplex(0.0, K_VECTORS[71][i] * WATER_POSITIONS[water_index][i + 3]));
        exp_kr_H2[i][6] = exp(dcomplex(0.0, K_VECTORS[71][i] * WATER_POSITIONS[water_index][i + 6]));

        exp_kr_O[i][4] = COMPLEX_ONE / exp_kr_O[i][6];
        exp_kr_H1[i][4] = COMPLEX_ONE / exp_kr_H1[i][6];
        exp_kr_H2[i][4] = COMPLEX_ONE / exp_kr_H2[i][6];

        for (int j = 2; j <= 5; j++) {
            exp_kr_O[i][5 + j] = exp_kr_O[i][4 + j] * exp_kr_O[i][6];
            exp_kr_O[i][5 - j] = exp_kr_O[i][6 - j] * exp_kr_O[i][4];

            exp_kr_H1[i][5 + j] = exp_kr_H1[i][4 + j] * exp_kr_H1[i][6];
            exp_kr_H1[i][5 - j] = exp_kr_H1[i][6 - j] * exp_kr_H1[i][4];

            exp_kr_H2[i][5 + j] = exp_kr_H2[i][4 + j] * exp_kr_H2[i][6];
            exp_kr_H2[i][5 - j] = exp_kr_H2[i][6 - j] * exp_kr_H2[i][4];
        }
    }
    return;
}

void set_exp_kr_table_for_ion(int ion_index) {
    for (int i = 0; i < 3; i++) {
        exp_kr_ion[i][6] = exp(dcomplex(0.0, K_VECTORS[71][i] * IONS[ion_index][i]));

        exp_kr_ion[i][4] = COMPLEX_ONE / exp_kr_ion[i][6];

        for (int j = 2; j <= 5; j++) {
            exp_kr_ion[i][5 + j] = exp_kr_ion[i][4 + j] * exp_kr_ion[i][6];
            exp_kr_ion[i][5 - j] = exp_kr_ion[i][6 - j] * exp_kr_ion[i][4];
        }
    }
    return;
}

void test_k_vector_table() {
    cout << "---- BEGIN TEST - TEST K VECTOR TABLE ----" << endl;

    BOX_LENGTH = 10.0;
    initialize_k_vectors_table();
    double * coords = K_VECTORS[0];
    cout << setprecision(10) << coords[0] << " " << coords[1] << " " << coords[2] << " " << coords[3] << endl;

    cout << "---- END TEST - TEST K VECTOR TABLE ----\n" << endl;
    return;
}
