#include "common.h"

void run_tests(int argc, char** argv) {
    cout << "---- BEGIN TEST SUITE ----\n" << endl;
    //test_RANDGAUSS();
    //test_read_program_flags(argc, argv);
    //test_energy_of_water_with_index();
    //test_calculate_energy();
    //test_run_mc();
    //test_radial_dist_sampler();
    test_write_to_vmd_file(argc, argv);
    test_read_config_file();
    cout << "---- END TEST SUITE ----" << endl;
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
    initialize();
    print_system_vars();
    cout << "---- END TEST - READ PROGRAM FLAGS ----\n" << endl;
    return;
}

void test_energy_of_water_with_index() {
    cout << "---- BEGIN TEST - ENERGY AT SINGLE PARTICLE ----" << endl;
    NUM_WATERS = 2;
    BOX_LENGTH = 10.0;
    water_O_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++)
        water_O_positions[i] = new double[3];

    water_O_positions[0][0] = 0.0;
    water_O_positions[0][1] = 0.0;
    water_O_positions[0][2] = 0.0;

    water_O_positions[1][0] = 9.0;
    water_O_positions[1][1] = 9.0;
    water_O_positions[1][2] = 9.0;

    for (int i = 0; i < NUM_WATERS; i++)
        cout << "Energy of particle " << setprecision(10) << i << " (kJ):"
            << energy_of_water_with_index(i) << endl;

    cout << "---- END TEST - ENERGY AT SINGLE PARTICLE ----\n" << endl;
    return;
}

void test_calculate_energy() {
    cout << "---- BEGIN TEST - CALCULATE SYSTEM ENERGY ----" << endl;
    NUM_WATERS = 4;
    BOX_LENGTH = 10.0;
    water_O_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++)
        water_O_positions[i] = new double[3];

    water_O_positions[0][0] = 0.0;
    water_O_positions[0][1] = 0.0;
    water_O_positions[0][2] = 0.0;

    water_O_positions[1][0] = 9.0;
    water_O_positions[1][1] = 9.0;
    water_O_positions[1][2] = 9.0;

    water_O_positions[2][0] = 1.0;
    water_O_positions[2][1] = 2.0;
    water_O_positions[2][2] = 3.0;

    water_O_positions[3][0] = 3.0;
    water_O_positions[3][1] = 1.0;
    water_O_positions[3][2] = 5.0;

    calculate_energy();
    cout << "System Energy: " << setprecision(10) << LJEnergy << endl;
    cout << "---- END TEST - CALCULATE SYSTEM ENERGY ----\n" << endl;
    return;
}

void test_run_mc() {
    cout << "---- BEGIN TEST - MC STEP ----" << endl;
    NUM_WATERS = 4;
    BOX_LENGTH = 10.0;
    water_O_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++)
        water_O_positions[i] = new double[3];

    water_O_positions[0][0] = 0.0;
    water_O_positions[0][1] = 0.0;
    water_O_positions[0][2] = 0.0;

    water_O_positions[1][0] = 9.0;
    water_O_positions[1][1] = 9.0;
    water_O_positions[1][2] = 9.0;

    water_O_positions[2][0] = 1.0;
    water_O_positions[2][1] = 2.0;
    water_O_positions[2][2] = 3.0;

    water_O_positions[3][0] = 3.0;
    water_O_positions[3][1] = 1.0;
    water_O_positions[3][2] = 5.0;

    calculate_energy();
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
    for (int h = 0; h < 10; h++)
        mc_sweep();
    write_config_to_vmd_file();
    close_vmd_file();

    cout << "---- END TEST - WRITE TO VMD FILE ----" << endl;
    return;
}

void test_read_config_file() {
    cout << "---- BEGIN TEST - READ CONFIG FILE ----" << endl;
    
    input_config_filename = "sample.config";
    load_configuration_file();
    calculate_energy();
    print_system_vars();
    print_system_config();

    cout << "---- END TEST - READ CONFIG FILE ----" << endl;
    return;
}

void print_system_vars() {
    cout << "System temperature (K): " << setprecision(10) << TEMPERATURE << endl
            << "Number of water particles: " << NUM_WATERS << endl
            << "Box length (Angstroms): " << BOX_LENGTH << endl
            << "Water sigma (Angstroms): " << WATER_SIGMA << endl
            << "System energy (kJ): " << LJEnergy << endl;
    return;
}

void print_system_config() {
    cout << "Atom configuration:" << endl;
    for (int i = 0; i < NUM_WATERS; i++)
        cout << "Water " << i + 1 << " of " << NUM_WATERS << ": " << setprecision(10)
            << water_O_positions[i][0] << "\t"
            << water_O_positions[i][1] << "\t"
            << water_O_positions[i][2] << endl;
    return;
}
