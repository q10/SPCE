#include "common.h"

double ** ROTATION_MATRIX = new double * [3];
double * TMP_CENTER_OF_MASS = new double [3];

void initialize_rotation_matrix() {
    for (int i = 0; i < 3; i++)
        ROTATION_MATRIX[i] = new double[3];
    return;
}

void set_center_of_mass_of_water_with_index(int i) {
    for (int j = 0; j < 3; j++)
        TMP_CENTER_OF_MASS[j] = (O_MASS * WATER_POSITIONS[i][j] + H_MASS * WATER_POSITIONS[i][j + 3] + H_MASS * WATER_POSITIONS[i][j + 6]) / WATER_MASS;
    return;
}

void set_rotation_matrix(double * rand_unit_vector, double theta_rad) {
    double rx = rand_unit_vector[0], ry = rand_unit_vector[1], rz = rand_unit_vector[2], sin_t = sin(theta_rad);
    double cos_t = sqrt(1.0 - sin_t * sin_t);
    double l_cos_t = 1.0 - cos_t, rx2 = rx*rx, ry2 = ry*ry, rz2 = rz*rz, rxry = rx*ry, ryrz = ry*rz, rxrz = rx*rz;

    ROTATION_MATRIX[0][0] = cos_t + rx2 * l_cos_t;
    ROTATION_MATRIX[0][1] = rxry * l_cos_t - rz * sin_t;
    ROTATION_MATRIX[0][2] = rxrz * l_cos_t + ry * sin_t;

    ROTATION_MATRIX[1][0] = rxry * l_cos_t + rz * sin_t;
    ROTATION_MATRIX[1][1] = cos_t + ry2 * l_cos_t;
    ROTATION_MATRIX[1][2] = ryrz * l_cos_t - rx * sin_t;

    ROTATION_MATRIX[2][0] = rxrz * l_cos_t - ry * sin_t;
    ROTATION_MATRIX[2][1] = ryrz * l_cos_t + rx * sin_t;
    ROTATION_MATRIX[2][2] = cos_t + rz2 * l_cos_t;

    delete [] rand_unit_vector;
    return;
}

void test_rotation() {
    cout << "---- BEGIN TEST - ROTATION ----" << endl;

    NUM_WATERS = 1;
    BOX_LENGTH = 10.0;
    initialize_waters();
    initialize_other_variables();
    for (int k = 0; k < 10000; k++) {
        mc_rotate();
        cout << setprecision(10) << WATER_POSITIONS[0][0] << ", " << WATER_POSITIONS[0][1] << ", " << WATER_POSITIONS[0][2] << endl;
    }

    cout << "---- END TEST - ROTATION ----\n" << endl;
    return;
}
