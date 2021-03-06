#include "common.h"

int num_gr, radial_dist_num_his_bars = 100;
static double delg;
double * radial_dist_distance = new double [radial_dist_num_his_bars];
double * radial_dist_data = new double [radial_dist_num_his_bars];
double * ion_radial_dist_data = new double [radial_dist_num_his_bars];

void initialize_radial_dist_sampler() {
    num_gr = 0;
    delg = BOX_LENGTH / (2 * radial_dist_num_his_bars);
    for (int i = 0; i < radial_dist_num_his_bars; i++)
        radial_dist_data[i] = ion_radial_dist_data[i] = 0;
    return;
}

void radial_dist_sample() {
    num_gr++;
    double dx, dy, dz, dr;

    // water-water
    for (int i = 0; i < NUM_WATERS - 1; i++) {
        for (int j = i + 1; j < NUM_WATERS; j++) {
            dx = abs(WATER_POSITIONS[i][0] - WATER_POSITIONS[j][0]);
            dy = abs(WATER_POSITIONS[i][1] - WATER_POSITIONS[j][1]);
            dz = abs(WATER_POSITIONS[i][2] - WATER_POSITIONS[j][2]);
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

    // ion-water
    for (int i = 0; i < NUM_IONS; i++) {
        for (int j = 0; j < NUM_WATERS; j++) {
            dx = abs(IONS[i][0] - WATER_POSITIONS[j][0]);
            dy = abs(IONS[i][1] - WATER_POSITIONS[j][1]);
            dz = abs(IONS[i][2] - WATER_POSITIONS[j][2]);
            dx -= BOX_LENGTH * ROUND(dx / BOX_LENGTH);
            dy -= BOX_LENGTH * ROUND(dy / BOX_LENGTH);
            dz -= BOX_LENGTH * ROUND(dz / BOX_LENGTH);
            dr = sqrt(dx * dx + dy * dy + dz * dz);
            if (dr < BOX_LENGTH / 2.0) {
                int ig = int(dr / delg);
                ion_radial_dist_data[ig] += 2;
            }
        }
    }

    return;
}

void compute_radial_dist_results() {
    double r, vb, nid;
    for (int i = 0; i < radial_dist_num_his_bars; i++) {
        r = delg * (i + 0.5);
        radial_dist_distance[i] = r;
        vb = (pow(i + 1, 3.0) - pow(i, 3.0)) * pow(delg, 3.0);
        nid = (4 / 3) * M_PI * vb * WATER_DENSITY;
        radial_dist_data[i] /= num_gr * NUM_WATERS * nid;

        //nid = (4 / 3) * M_PI * vb * ION_DENSITY;
        ion_radial_dist_data[i] /= num_gr * NUM_IONS * nid;
    }
    return;
}

void print_radial_dist_results() {
    cout << "\nr(Angstroms)\tg(r)[O-O]\tg(r)[ion-O]" << endl;
    for (int k = 0; k < radial_dist_num_his_bars; k++)
        cout << setprecision(10) << radial_dist_distance[k] << "\t" << radial_dist_data[k] << "\t" << ion_radial_dist_data[k] << endl;
    return;
}

void test_radial_dist_sampler() {
    cout << "---- BEGIN TEST - RADIAL DISTRIBUTION SAMPLER ----" << endl;

    NUM_EQUILIBRATION_SWEEPS = 0;
    NUM_MC_SWEEPS = 100000;
    initialize();
    run_mc();
    compute_radial_dist_results();
    print_radial_dist_results();

    cout << "\n---- END TEST - RADIAL DISTRIBUTION SAMPLER ----\n" << endl;
    return;
}
