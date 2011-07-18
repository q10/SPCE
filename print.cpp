#include "common.h"

void print_all() {
    print_system_vars();
    print_program_flags();
    //print_system_config();
    return;
}

void print_system_vars() {
    cout << "System temperature (K): " << setprecision(10) << TEMPERATURE << endl
            << "Number of waters: " << NUM_WATERS << endl
            << "Number of ions: " << NUM_IONS << endl
            << "Box length (Angstroms): " << BOX_LENGTH << endl
            << "Water sigma (Angstroms): " << WATER_SIGMA << endl
            << "System energy (kJ): " << TOTAL_ENERGY << endl << endl
            << "Number of single particle move attempts per MC sweep: " << NUM_MC_ATTEMPTS_PER_SWEEP << endl
            << "Number of MC sweeps: " << NUM_MC_SWEEPS << endl
            << "Number of equilibration sweeps: " << NUM_EQUILIBRATION_SWEEPS << endl << endl;
    return;
}

void print_program_flags() {
    if (using_input_config_file)
        cout << "--input (-r) flag raised; using file \'" << input_config_filename << "\'" << endl;
    if (use_custom_output_vmd_filename)
        cout << "--output_vmd (-v) flag raised; will write VMD to \'" << output_vmd_filename << "\'" << endl;
    if (use_custom_output_config_filename)
        cout << "--output_config (-w) flag raised; will save config to \'" << output_config_filename << "\'" << endl << endl;
    return;
}

void print_system_config() {
    cout << "Atom configuration (/ charge):" << endl;
    for (int i = 0; i < NUM_WATERS; i++) {
        cout << "Water " << i + 1 << " of " << NUM_WATERS << ":";
        for (int j = 0; j < 9; j++)
            cout << "\t" << setprecision(10) << WATER_POSITIONS[i][j];
        cout << endl;
    }
    cout << endl;
    for (int i = 0; i < NUM_IONS; i++) {
        cout << "ION " << i + 1 << " of " << NUM_IONS << ":";
        for (int j = 0; j < 4; j++)
            cout << "\t" << setprecision(10) << IONS[i][j];
        cout << endl;
    }
    cout << endl;
    return;
}
