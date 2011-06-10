#include "common.h"

void print_system_vars() {
    cout << "System temperature (K): " << setprecision(10) << TEMPERATURE << endl
            << "Number of water particles: " << NUM_WATERS << endl
            << "Box length (Angstroms): " << BOX_LENGTH << endl
            << "Water sigma (Angstroms): " << WATER_SIGMA << endl
            << "System energy (kJ): " << LJEnergy << endl << endl
            << "Number of single particle move attempts per MC sweep: " << NUM_MC_ATTEMPTS_PER_SWEEP << endl
            << "Number of MC sweeps: " << NUM_MC_SWEEPS << endl
            << "Number of equilibration sweeps: " << NUM_EQUILIBRATION_SWEEPS << endl << endl;

    if (using_input_config_file)
        cout << "--input (-r) flag raised; using file \'" << input_config_filename << "\'" << endl;
    if (use_custom_output_vmd_filename)
        cout << "--output_vmd (-v) flag raised; will write VMD to \'" << output_vmd_filename << "\'" << endl;
    if (use_custom_output_config_filename)
        cout << "--output_config (-w) flag raised; will save config to \'" << output_config_filename << "\'" << endl << endl;
    return;
}

void print_system_config() {
    cout << "Atom configuration:" << endl;
    for (int i = 0; i < NUM_WATERS; i++) {
        cout << "Water " << i + 1 << " of " << NUM_WATERS << ": ";
        for (int j = 0; j < 9; j++)
            cout << setprecision(10) << water_positions[i][j] << j == 8 ? endl : "\t";
    }
    return;
}
