#include "common.h"

void initialize_erfc_table() {
    double sqrt_alpha_1000 = sqrt(EWALD_ALPHA) / 1000.0, key;
    for (double r = 0.0; r < HALF_BOX_LENGTH; r += 0.001) {
        // to optimize map search by using one less division, we keep the keys as x10^4
        key = floor(r * 1000.0);
        (*ERFC_TABLE)[key] = erfc(key * sqrt_alpha_1000);
    }
    return;
}

void initialize_k_values_table() {
    K_VALUES->clear();
    double * k_entry, tmp_k2, alpha_inv_4 = -1.0 / (4.0 * EWALD_ALPHA);

    for (int nx = 0; nx <= 5; nx++) {
        for (int ny = -5; ny <= 5; ny++) {
            for (int nz = -5; nz <= 5; nz++) {
                if (nx != 0 || ny != 0 || nz != 0) { // removes the K=0 case
                    k_entry = new double [4];
                    k_entry[0] = 2.0 * M_PI * nx / BOX_LENGTH;
                    k_entry[1] = 2.0 * M_PI * ny / BOX_LENGTH;
                    k_entry[2] = 2.0 * M_PI * nz / BOX_LENGTH;

                    // k_entry[3] will be exp(-K^2 / (4*alpha)), where K^2, or kx^2 + ky^2 + kz^2, to reduce redundant computation later on
                    tmp_k2 = k_entry[0] * k_entry[0] + k_entry[1] * k_entry[1] + k_entry[2] * k_entry[2];
                    k_entry[3] = exp(tmp_k2 * alpha_inv_4) / tmp_k2;

                    K_VALUES->push_back(k_entry);
                }
            }
        }
    }
    return;
}

void initialize_rho_k_values_table() {
    for (int k = 0; k < K_VALUES->size(); k++)
        RHO_K_VALUES->push_back(rho((*K_VALUES)[k]));
    return;
}

dcomplex rho_k_diff(int water_index, double * k_vect, double * old_position) {
    double q, rxn, ryn, rzn;
    dcomplex rho_diff(0.0, 0.0);
    for (int atom = 0; atom < 9; atom += 3) {
        q = (atom == 0) ? WATER_Q_O : WATER_Q_H;
        rxn = water_positions[water_index][atom];
        ryn = water_positions[water_index][atom + 1];
        rzn = water_positions[water_index][atom + 2];
        rho_diff += q * (exp(dcomplex(0.0, k_vect[0] * rxn + k_vect[1] * ryn + k_vect[2] * rzn)) -
                exp(dcomplex(0.0, k_vect[0] * old_position[0] + k_vect[1] * old_position[1] + k_vect[2] * old_position[2])));
    }
    return rho_diff;
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

double ewald_sum() {
    double sum = 0.0;
    for (int k = 0; k < K_VALUES->size(); k++)
        sum += norm(rho((*K_VALUES)[k])) * (*K_VALUES)[k][3];
    return sum * 4.0 * M_PI / pow(BOX_LENGTH, 3.0);
}

double ewald_diff(int water_index, double * old_position, vector <dcomplex> * rho_k_diff_values) {
    double sum_of_ewald_diffs = 0.0, old_pk2;
    dcomplex diff(0.0, 0.0);

    for (int k = 0; k < RHO_K_VALUES->size(); k++) {
        diff = rho_k_diff(water_index, (*K_VALUES)[k], old_position);
        rho_k_diff_values->push_back(diff);
        old_pk2 = norm((*RHO_K_VALUES)[k]);
        (*RHO_K_VALUES)[k] += diff;
        sum_of_ewald_diffs += (norm((*RHO_K_VALUES)[k]) - old_pk2) * (*K_VALUES)[k][3];
    }
    return sum_of_ewald_diffs * 4.0 * M_PI / pow(BOX_LENGTH, 3.0);
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

void test_k_table() {
    cout << "---- BEGIN TEST - TEST K TABLE ----" << endl;

    BOX_LENGTH = 10.0;
    initialize_k_values_table();
    double * coords = (*K_VALUES)[0];
    cout << setprecision(10) << coords[0] << " " << coords[1] << " " << coords[2] << " " << coords[3] << endl;

    cout << "---- END TEST - TEST K TABLE ----\n" << endl;
    return;
}
