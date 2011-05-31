#include "common.h"

int num_gr, radial_dist_num_his_bars = 25;
static double delg;
double * radial_dist_data = new double [radial_dist_num_his_bars];

void initialize_radial_dist_sampler() {
    num_gr = 0;
    delg = BOX_LENGTH / (2 * radial_dist_num_his_bars);
    for (int i = 0; i < radial_dist_num_his_bars; i++)
        radial_dist_data[i] = 0;
    return;
}

void radial_dist_sample() {
    num_gr++;
    double dx, dy, dz, dr;
    for (int i = 0; i < NUM_WATERS - 1; i++) {
        for (int j = i + 1; j < NUM_WATERS; j++) {
            dx = abs(water_positions[i][0] - water_positions[j][0]);
            dy = abs(water_positions[i][1] - water_positions[j][1]);
            dz = abs(water_positions[i][2] - water_positions[j][2]);
            dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
            dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
            dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
            dr = sqrt(dx * dx + dy * dy + dz * dz);
            if (dr < BOX_LENGTH / 2.0) {
                int ig = int(dr / delg);
                radial_dist_data[ig] += 2;
            }
        }
    }
    return;
}

void compute_radial_dist_results() {
    double r, vb, nid;
    for (int i = 0; i < radial_dist_num_his_bars; i++) {
        r = delg * (i + 0.5);
        vb = (pow(i + 1, 3.0) - pow(i, 3.0)) * pow(delg, 3.0);
        nid = (4 / 3) * M_PI * vb * WATER_DENSITY;
        radial_dist_data[i] /= num_gr * NUM_WATERS * nid;
    }
    return;
}
