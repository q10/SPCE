#include "common.h"

void run_tests(int argc, char** argv) {
    cout << "\n---- BEGIN TEST SUITE ----\n" << endl;
    //test_RANDGAUSS();                         // PASS
    //test_read_program_flags(argc, argv);      // PASS
    //test_energy_of_water_with_index(); // PASS
    //test_run_mc();
    //test_radial_dist_sampler();
    //test_write_to_vmd_file(argc, argv);       // PASS
    //test_write_to_config_file(argc, argv);    // PASS
    //test_read_config_file(argc, argv);        // PASS
    //test_rotation();                          // PASS
    test_mc_acceptances();
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
    initialize_waters();
    BOX_LENGTH = 10.0;
    NUM_MC_SWEEPS = 1;
    calculate_and_init_energy();
    for (int k = 0; k < 10000; k++) {
        run_mc();
        cout << "Current system energy: " << setprecision(10) << LJEnergy << endl;
    }

    cout << "---- END TEST - MC STEPPING ----\n" << endl;
    return;
}

void test_radial_dist_sampler() {
    cout << "---- BEGIN TEST - RADIAL DISTRIBUTION SAMPLER ----" << endl;

    initialize();
    run_mc();
    compute_radial_dist_results();

    cout << "\nr(Angstroms)\tg(r)" << endl;
    for (int k = 0; k < radial_dist_num_his_bars; k++)
        cout << setprecision(10) << radial_dist_distance[k] << "\t" << radial_dist_data[k] << endl;

    cout << "\n---- END TEST - RADIAL DISTRIBUTION SAMPLER ----\n" << endl;
    return;
}

void test_write_to_vmd_file(int argc, char** argv) {
    cout << "---- BEGIN TEST - WRITE TO VMD FILE ----" << endl;

    read_program_flags(argc, argv);
    open_vmd_file();
    initialize_constants();
    initialize_waters();
    write_config_to_vmd_file();
    NUM_EQUILIBRATION_SWEEPS = 50;
    for (int g = 0; g < 10; g++) {
        mc_equilibrate();
        write_config_to_vmd_file();
    }
    close_vmd_file();

    cout << "---- END TEST - WRITE TO VMD FILE ----\n" << endl;
    return;
}

void test_write_to_config_file(int argc, char** argv) {
    cout << "---- BEGIN TEST - WRITE CONFIG FILE ----" << endl;

    read_program_flags(argc, argv);
    initialize_constants();
    initialize_waters();
    save_config_to_file();
    cout << "---- END TEST - WRITE CONFIG FILE ----\n" << endl;
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

void test_mc_acceptances() {
    cout << "---- BEGIN TEST - MC ACCEPTANCES ----" << endl;

    initialize_constants();
    initialize_waters();
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
