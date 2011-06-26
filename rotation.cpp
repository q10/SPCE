#include "common.h"

double * center_of_mass_of_water_with_index(int i) {
    double * center_of_mass = new double[3];
    for (int j = 0; j < 3; j++) {
        center_of_mass[j] = O_MASS * water_positions[i][j] + H_MASS * water_positions[i][j + 3] + H_MASS * water_positions[i][j + 6];
        center_of_mass[j] /= WATER_MASS;
    }
    return center_of_mass;
}

double ** rotation_matrix(double * rand_unit_vector, double theta_rad) {
    double ** rot_matrix = new double * [3];
    for (int i = 0; i < 3; i++)
        rot_matrix[i] = new double[3];

    double rx = rand_unit_vector[0], ry = rand_unit_vector[1], rz = rand_unit_vector[2];
    double cos_t = cos(theta_rad), sin_t = sin(theta_rad);
    double l_cos_t = 1.0 - cos_t, rx2 = rx*rx, ry2 = ry*ry, rz2 = rz*rz, rxry = rx*ry, ryrz = ry*rz, rxrz = rx*rz;

    rot_matrix[0][0] = cos_t + rx2 * l_cos_t;
    rot_matrix[0][1] = rxry * l_cos_t - rz * sin_t;
    rot_matrix[0][2] = rxrz * l_cos_t + ry * sin_t;

    rot_matrix[1][0] = rxry * l_cos_t + rz * sin_t;
    rot_matrix[1][1] = cos_t + ry2 * l_cos_t;
    rot_matrix[1][2] = ryrz * l_cos_t - rx * sin_t;

    rot_matrix[2][0] = rxrz * l_cos_t - ry * sin_t;
    rot_matrix[2][1] = ryrz * l_cos_t + rx * sin_t;
    rot_matrix[2][2] = cos_t + rz2 * l_cos_t;

    delete [] rand_unit_vector;

    return rot_matrix;
}

void test_rotation() {
    cout << "---- BEGIN TEST - ROTATION ----" << endl;

    NUM_WATERS = 1;
    BOX_LENGTH = 10.0;
    initialize_waters();
    for (int k = 0; k < 10000; k++) {
        mc_rotate();
        cout << setprecision(10) << water_positions[0][0] << ", " << water_positions[0][1] << ", " << water_positions[0][2] << endl;
    }

    cout << "---- END TEST - ROTATION ----\n" << endl;
    return;
}
