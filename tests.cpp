#include "common.h"

void run_tests(int argc, char** argv) {
    cout << "\n---- BEGIN TEST SUITE ----\n" << endl;
    //test_RANDGAUSS();                         // PASS
    //test_read_program_flags(argc, argv);      // PASS
    //test_energy_of_water_with_index();        // PASS
    //test_run_mc();
    test_radial_dist_sampler();
    //test_write_to_vmd_file(argc, argv);       // PASS
    //test_write_to_config_file(argc, argv);    // PASS
    //test_read_config_file(argc, argv);        // PASS
    //test_rotation();                          // PASS
    //test_mc_acceptances();                    // PASS
    //test_rho_function();                      // PASS
    //test_k_vector_table();                    // PASS
    cout << "---- END TEST SUITE ----\n" << endl;
    return;
}

void test_RANDGAUSS() {
    cout << "---- BEGIN TEST - RANDGAUSS ----" << endl;
    double a;
    int bins[20];
    for (int i = 0; i < 20; i++)
        bins[i] = 0;

    for (int i = 0; i < 1000; i++) {
        a = RANDGAUSS(100.0, 0.5);
        for (int j = 0; j < 20; j++) {
            if (j == 19 || (a < 98.0 + j * 0.2)) {
                bins[j]++;
                break;
            }
        }
    }
    for (int i = 0; i < 20; i++)
        cout << bins[i] << endl;

    cout << "---- END TEST - RANDGAUSS ----\n" << endl;
    return;
}

void test_read_program_flags(int argc, char** argv) {
    cout << "---- BEGIN TEST - READ PROGRAM FLAGS ----" << endl;
    read_program_flags(argc, argv);
    NUM_EQUILIBRATION_SWEEPS = 5;
    initialize();
    cout << "---- END TEST - READ PROGRAM FLAGS ----\n" << endl;
    return;
}

void test_energy_of_water_with_index() {
    cout << "---- BEGIN TEST - ENERGY AT SINGLE PARTICLE ----" << endl;
    NUM_WATERS = 2;
    BOX_LENGTH = 10.0;
    water_positions = new double*[NUM_WATERS];

    for (int i = 0; i < NUM_WATERS; i++)
        water_positions[i] = new double[9];

    // First water - Non-realistic coordinates are given for easy calculation check
    for (int i = 0; i < 3; i++)
        water_positions[0][i] = 1.0;
    for (int i = 3; i < 6; i++)
        water_positions[0][i] = 0.0;
    for (int i = 6; i < 9; i++)
        water_positions[0][i] = 2.0;

    // Second water
    for (int i = 0; i < 3; i++)
        water_positions[1][i] = 5.0;
    for (int i = 3; i < 6; i++)
        water_positions[1][i] = 9.0;
    for (int i = 6; i < 9; i++)
        water_positions[1][i] = 9.0;
    water_positions[1][8] = 8.0;

    cout << "Case 1 (O-O distance < BOX_LENGTH/2):" << endl;
    for (int i = 0; i < NUM_WATERS; i++)
        cout << "Energy of water #" << setprecision(10) << i << " (kJ):"
            << energy_of_water_with_index(i) << endl;

    for (int i = 0; i < 3; i++)
        water_positions[1][i] = 8.0;
    cout << "\nCase 2 (O-O distance > BOX_LENGTH/2):" << endl;
    for (int i = 0; i < NUM_WATERS; i++)
        cout << "Energy of water #" << setprecision(10) << i << " (kJ):"
            << energy_of_water_with_index(i) << endl;

    cout << "---- END TEST - ENERGY AT SINGLE PARTICLE ----\n" << endl;
    return;
}

void test_run_mc() {
    cout << "---- BEGIN TEST - MC STEP ----" << endl;

    NUM_WATERS = 200;
    NUM_MC_SWEEPS = 1;
    initialize_erfc_table();
    initialize_waters();
    initialize_rho_k_values_table();
    calculate_and_init_energy();
    for (int k = 0; k < 10000; k++) {
        run_mc();
        cout << "Current system energy: " << setprecision(10) << LJEnergy << endl;
    }

    cout << "---- END TEST - MC STEPPING ----\n" << endl;
    return;
}

void test_read_config_file(int argc, char** argv) {
    cout << "---- BEGIN TEST - READ CONFIG FILE ----" << endl;

    input_config_filename = "sample.config";
    load_configuration_file();
    calculate_and_init_energy();
    print_all();

    cout << "---- END TEST - READ CONFIG FILE ----\n" << endl;
    return;
}

void test_mc_acceptances() {
    cout << "---- BEGIN TEST - MC ACCEPTANCES ----" << endl;

    NUM_EQUILIBRATION_SWEEPS = 0;
    initialize();
    NUM_EQUILIBRATION_SWEEPS = 100;
    double displacement_distances[] = {0.2, 0.22, 0.24, 0.26, 0.28, 0.3};
    double displacement_rotations[] = {0.15 * M_PI, 0.16 * M_PI, 0.17 * M_PI, 0.175 * M_PI, 0.18 * M_PI, 0.19 * M_PI};

    for (int g = 0; g < 6; g++) {
        DISPLACEMENT_DISTANCE = displacement_distances[g];
        DISPLACEMENT_ROTATION = displacement_rotations[g];
        reset_mc_acceptance_records();
        mc_equilibrate();
        print_mc_acceptance_records();
    }

    cout << "---- END TEST - MC ACCEPTANCES ----\n" << endl;
    return;
}
/*
void time_exp_vs_pow() {
    cout << "---- BEGIN TIME - EXP VS POW ----" << endl;
    int start = clock(), times = 100000000;
    dcomplex d;
    for (int i = 0; i < times; i++) {
        d = dcomplex(1.335, 0.34541);
        d = exp(d);
    }
    cout << (clock() - start) << endl;
    start = clock();

    for (int i = 0; i < 3; i++) {
        d = dcomplex(1.335, 0.34541);
        d = exp(d);
    }
    for (int i = 0; i < times; i++) {
        d = dcomplex(1.335, 0.34541);
        d = pow(d, 5);
    }
    cout << (clock() - start) << endl;
    cout << "---- END TIME - EXP VS POW ----\n" << endl;
    return;
}

void time_new_operator_overhead() {
    cout << "---- BEGIN TIME - NEW OPERATOR OVERHEAD ----" << endl;
    int start = clock(), times = 10000, length = 100000;
    double array[length];
    for (int i = 0; i < times; i++) {
        for (int j = 0; j < length; j++)
            array[j] = pow(j, 5);
    }
    cout << (clock() - start) << endl;
    start = clock();

    double * arr_ptr;
    for (int i = 0; i < times; i++) {
        arr_ptr = new double [length];
        for (int j = 0; j < length; j++)
            arr_ptr[j] = pow(j, 5);
        delete arr_ptr;
    }
    cout << (clock() - start) << endl;
    cout << "---- END TIME - NEW OPERATOR OVERHEAD ----\n" << endl;
    return;
}
*/