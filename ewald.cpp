#include "common.h"

dcomplex exp_krx_O[11], exp_kry_O[11], exp_krz_O[11], exp_krx_H1[11], exp_kry_H1[11], exp_krz_H1[11], exp_krx_H2[11], exp_kry_H2[11], exp_krz_H2[11];

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
    K_VECTORS->clear();
    double * k_entry, tmp_k2, alpha_inv_4 = -1.0 / (4.0 * EWALD_ALPHA);

    for (int nx = 0; nx <= 5; nx++) {
        for (int ny = -5; ny <= 5; ny++) {
            for (int nz = -5; nz <= 5; nz++) {
                if (nx != 0 || ny != 0 || nz != 0) { // removes the K=0 case
                    k_entry = new double [4];
                    k_entry[0] = 2.0 * M_PI * nx / BOX_LENGTH;
                    k_entry[1] = 2.0 * M_PI * ny / BOX_LENGTH;
                    k_entry[2] = 2.0 * M_PI * nz / BOX_LENGTH;

                    // k_entry[3] will be the Fourier coefficient exp(-K^2 / (4*alpha)) / K^2, where K^2, or kx^2 + ky^2 + kz^2
                    tmp_k2 = k_entry[0] * k_entry[0] + k_entry[1] * k_entry[1] + k_entry[2] * k_entry[2];
                    k_entry[3] = exp(tmp_k2 * alpha_inv_4) / tmp_k2;

                    K_VECTORS->push_back(k_entry);
                }
            }
        }
    }
    return;
}

void initialize_rho_k_values_table() {
    RHO_K_VALUES->clear();
    dcomplex *column, column_sum;

    for (int k = 0; k < K_VECTORS->size(); k++) {
        column = new dcomplex [NUM_WATERS + 2];
        column_sum = dcomplex(0.0, 0.0);

        for (int w = 0; w < NUM_WATERS; w++) {
            column[w] = partial_rho(w, (*K_VECTORS)[k]);
            column_sum += column[w];
        }

        column[NUM_WATERS] = column_sum;
        RHO_K_VALUES->push_back(column);
    }
    return;
}

dcomplex rho(double * k_coords) {
    double rx, ry, rz, q;
    dcomplex rho(0.0, 0.0);

    for (int i = 0; i < NUM_WATERS; i++) {
        for (int atom = 0; atom < 9; atom += 3) {
            q = (atom == 0) ? WATER_Q_O : WATER_Q_H;
            rx = water_positions[i][atom];
            ry = water_positions[i][atom + 1];
            rz = water_positions[i][atom + 2];
            rho += q * exp(dcomplex(0.0, k_coords[0] * rx + k_coords[1] * ry + k_coords[2] * rz));
        }
    }
    return rho;
}

dcomplex partial_rho(int water_index, double * k_coords) {
    double rx, ry, rz, q;
    dcomplex part_rho(0.0, 0.0);

    for (int atom = 0; atom < 9; atom += 3) {
        q = (atom == 0) ? WATER_Q_O : WATER_Q_H;
        rx = water_positions[water_index][atom];
        ry = water_positions[water_index][atom + 1];
        rz = water_positions[water_index][atom + 2];
        part_rho += q * exp(dcomplex(0.0, k_coords[0] * rx + k_coords[1] * ry + k_coords[2] * rz));
    }
    return part_rho;
}

double ewald_sum() {
    double sum = 0.0;
    for (int k = 0; k < K_VECTORS->size(); k++)
        sum += norm(rho((*K_VECTORS)[k])) * (*K_VECTORS)[k][3];
    return sum * 4.0 * M_PI / pow(BOX_LENGTH, 3.0);
}

double ewald_diff(int water_index) {
    double sum_of_ewald_diffs = 0.0, old_pk2;
    dcomplex *column, tmp_exp_kr_O, tmp_exp_kr_H1, tmp_exp_kr_H2;
    set_exp_kr_table(water_index);
    int k = 0;

    for (int nx = 0; nx <= 5; nx++) {
        tmp_exp_kr_O = WATER_Q_O * exp_krx_O[nx];
        tmp_exp_kr_H1 = WATER_Q_H * exp_krx_H1[nx];
        tmp_exp_kr_H2 = WATER_Q_H * exp_krx_H2[nx];

        for (int ny = 0; ny <= 10; ny++) {
            tmp_exp_kr_O *= exp_kry_O[ny];
            tmp_exp_kr_H1 *= exp_kry_H1[ny];
            tmp_exp_kr_H2 *= exp_kry_H2[ny];

            for (int nz = 0; nz <= 10; nz++) {
                if (nx != 0 || ny != 5 || nz != 5) {
                    column = (*RHO_K_VALUES)[k];
                    old_pk2 = norm(column[NUM_WATERS]);

                    // save old rho(K, R)
                    // calculate and save new rho(K, R)
                    // update total rho to rho(K, R)_new - rho(K, R)_old
                    column[NUM_WATERS + 1] = column[water_index];
                    column[water_index] = tmp_exp_kr_O * exp_krz_O[nz] + tmp_exp_kr_H1 * exp_krz_H1[nz] + tmp_exp_kr_H2 * exp_krz_H2[nz];
                    column[NUM_WATERS] += column[water_index] - column[NUM_WATERS + 1];
                    
                    sum_of_ewald_diffs += (norm(column[NUM_WATERS]) - old_pk2) * (*K_VECTORS)[k][3];
                    k++;
                }
            }
        }
    }
    return sum_of_ewald_diffs * 4.0 * M_PI / pow(BOX_LENGTH, 3.0);
}

inline void set_exp_kr_table(int water_index) {
    dcomplex exp_x_O = exp(dcomplex(0.0, (*K_VECTORS)[71][0] * water_positions[water_index][0]));
    dcomplex exp_y_O = exp(dcomplex(0.0, (*K_VECTORS)[71][1] * water_positions[water_index][1]));
    dcomplex exp_z_O = exp(dcomplex(0.0, (*K_VECTORS)[71][2] * water_positions[water_index][2]));
    dcomplex exp_x_H1 = exp(dcomplex(0.0, (*K_VECTORS)[71][0] * water_positions[water_index][3]));
    dcomplex exp_y_H1 = exp(dcomplex(0.0, (*K_VECTORS)[71][1] * water_positions[water_index][4]));
    dcomplex exp_z_H1 = exp(dcomplex(0.0, (*K_VECTORS)[71][2] * water_positions[water_index][5]));
    dcomplex exp_x_H2 = exp(dcomplex(0.0, (*K_VECTORS)[71][0] * water_positions[water_index][6]));
    dcomplex exp_y_H2 = exp(dcomplex(0.0, (*K_VECTORS)[71][1] * water_positions[water_index][7]));
    dcomplex exp_z_H2 = exp(dcomplex(0.0, (*K_VECTORS)[71][2] * water_positions[water_index][8]));

    dcomplex exp_x_1_O = pow(exp_x_O, -1), exp_y_1_O = pow(exp_y_O, -1), exp_z_1_O = pow(exp_z_O, -1);
    dcomplex exp_x_1_H1 = pow(exp_x_H1, -1), exp_y_1_H1 = pow(exp_y_H1, -1), exp_z_1_H1 = pow(exp_z_H1, -1);
    dcomplex exp_x_1_H2 = pow(exp_x_H2, -1), exp_y_1_H2 = pow(exp_y_H2, -1), exp_z_1_H2 = pow(exp_z_H2, -1);

    exp_krx_O[5] = exp_kry_O[5] = exp_krz_O[5] = dcomplex(1.0, 0.0);

    for (int i = 1; i <= 5; i++) {
        exp_krx_O[5 + i] = exp_krx_O[4 + i] * exp_x_O;
        exp_krx_O[5 - i] = exp_krx_O[6 - i] * exp_x_1_O;
        exp_kry_O[5 + i] = exp_kry_O[4 + i] * exp_y_O;
        exp_kry_O[5 - i] = exp_kry_O[6 - i] * exp_y_1_O;
        exp_krz_O[5 + i] = exp_krz_O[4 + i] * exp_z_O;
        exp_krz_O[5 - i] = exp_krz_O[6 - i] * exp_z_1_O;

        exp_krx_H1[5 + i] = exp_krx_H1[4 + i] * exp_x_H1;
        exp_krx_H1[5 - i] = exp_krx_H1[6 - i] * exp_x_1_H1;
        exp_kry_H1[5 + i] = exp_kry_H1[4 + i] * exp_y_H1;
        exp_kry_H1[5 - i] = exp_kry_H1[6 - i] * exp_y_1_H1;
        exp_krz_H1[5 + i] = exp_krz_H1[4 + i] * exp_z_H1;
        exp_krz_H1[5 - i] = exp_krz_H1[6 - i] * exp_z_1_H1;


        exp_krx_H2[5 + i] = exp_krx_H2[4 + i] * exp_x_H2;
        exp_krx_H2[5 - i] = exp_krx_H2[6 - i] * exp_x_1_H2;
        exp_kry_H2[5 + i] = exp_kry_H2[4 + i] * exp_y_H2;
        exp_kry_H2[5 - i] = exp_kry_H2[6 - i] * exp_y_1_H2;
        exp_krz_H2[5 + i] = exp_krz_H2[4 + i] * exp_z_H2;
        exp_krz_H2[5 - i] = exp_krz_H2[6 - i] * exp_z_1_H2;
    }
    return;
}

void test_rho_function() {
    cout << "---- BEGIN TEST - RHO FUNCTION ----" << endl;

    NUM_WATERS = 1;
    water_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++)
        water_positions[i] = new double[9];
    for (int i = 0; i < 3; i++)
        water_positions[0][i] = 2.0;
    for (int i = 3; i < 6; i++)
        water_positions[0][i] = 1.0;
    for (int i = 6; i < 9; i++)
        water_positions[0][i] = 3.0;

    double * coords = new double [3];
    coords[0] = 5.5;
    coords[1] = 6.8;
    coords[2] = 9.2;
    dcomplex pk = rho(coords);
    delete coords;
    cout << "Output of rho function with k=(5.5, 6.8, 9.2): "
            << setprecision(10) << pk << endl
            << "Norm of rho(5.5, 6.8, 9.2) is: " << norm(pk) << endl;
    // should be -0.885409418 + 1.32668626i and 2.54404627

    cout << "---- END TEST - RHO FUNCTION ----\n" << endl;
    return;
}

void test_k_vector_table() {
    cout << "---- BEGIN TEST - TEST K VECTOR TABLE ----" << endl;

    BOX_LENGTH = 10.0;
    initialize_k_vectors_table();
    double * coords = (*K_VECTORS)[0];
    cout << setprecision(10) << coords[0] << " " << coords[1] << " " << coords[2] << " " << coords[3] << endl;

    cout << "---- END TEST - TEST K VECTOR TABLE ----\n" << endl;
    return;
}
