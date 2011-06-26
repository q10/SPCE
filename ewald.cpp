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
    double * coords;
    map <int, map <int, double *> *> * current_x;
    map <int, double *> * current_y;
    for (int nx = 0; nx <= 5; nx++) {
        current_x = new map <int, map <int, double *> *>;
        (*K_VALUES)[nx] = current_x;

        for (int ny = -5; ny <= 5; ny++) {
            current_y = new map <int, double *>;
            (*current_x)[ny] = current_y;

            for (int nz = -5; nz <= 5; nz++) {
                coords = new double [4];
                coords[0] = 2.0 * M_PI * nx / BOX_LENGTH;
                coords[1] = 2.0 * M_PI * ny / BOX_LENGTH;
                coords[2] = 2.0 * M_PI * nz / BOX_LENGTH;
                // coords[3] will be K^2, or kx^2 + ky^2 + kz^2, to reduce redundant computation later on
                coords[3] = coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2];
                (*current_y)[nz] = coords;
            }
        }
    }
    return;
}

dcomplex rho(double * coords) {
    double rx, ry, rz, q;
    dcomplex rho(0.0, 0.0);

    for (int i = 0; i < NUM_WATERS; i++) {
        for (int atom = 0; atom < 9; atom += 3) {
            q = (atom == 0) ? WATER_Q_O : WATER_Q_H;
            rx = water_positions[i][atom];
            ry = water_positions[i][atom + 1];
            rz = water_positions[i][atom + 2];
            rho += q * exp(dcomplex(0.0, coords[0] * rx + coords[1] * ry + coords[2] * rz));
        }
    }
    return rho;
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
    double * coords = (*(*(*K_VALUES)[0])[1])[3];
    cout << setprecision(10) << coords[0] << " " << coords[1] << " " << coords[2] << " " << coords[3] << endl;

    cout << "---- END TEST - TEST K TABLE ----\n" << endl;
    return;
}
