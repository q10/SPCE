#include "common.h"

void print_all() {
    print_universal_constants();
    print_program_flags();
    print_water_parameters();
    print_ion_parameters();
    print_system_vars();
    print_energy();
    print_system_config();
    return;
}

void print_universal_constants() {
    cout << "Boltzmann constant kJ/(mol*K): " << setprecision(10) << BOLTZMANN_K << endl
            << "Electrostatic constant (kJ*mol*Angstrom)/(elementary_charge^2): " << ELECTROSTATIC_K << endl << endl;
}

void print_system_vars() {
    cout << "System temperature (K): " << setprecision(10) << TEMPERATURE << endl
            << "Number of waters: " << NUM_WATERS << endl
            << "Number of ions: " << NUM_IONS << endl
            << "Box length (Angstroms): " << BOX_LENGTH << endl << endl
            << "Number of single particle move attempts per MC sweep: " << NUM_MC_ATTEMPTS_PER_SWEEP << endl
            << "Number of MC sweeps: " << NUM_MC_SWEEPS << endl
            << "Number of equilibration sweeps: " << NUM_EQUILIBRATION_SWEEPS << endl << endl
            << "Alpha value used for ERFC function: " << EWALD_ALPHA << endl << endl;
    return;
}

void print_energy() {
    cout << "Total system energy (kJ): " << TOTAL_ENERGY << endl
            << "Lennard-Jones and ERFC-adjusted Coulombic portion (all real space) combined: " << total_real_space_energy() << endl
            << "Ewald K-space portion: " << total_ewald_energy() << endl << endl;
    return;
}

void print_water_parameters() {
    cout << "Water sigma (Angstroms): " << setprecision(10) << WATER_SIGMA << endl
            << "Water LJ epsilon(kJ/mol): " << WATER_EPSILON << endl
            << "Water Density (particles/Angstroms^3): " << WATER_DENSITY << endl
            << "Partial charge of water hydrogen: " << WATER_Q_H << endl
            << "Partial charge of water oxygen: " << WATER_Q_O << endl
            << "Mass of hydrogen: " << H_MASS << endl
            << "Mass of oxygen: " << O_MASS << endl
            << "Mass of water: " << WATER_MASS << endl
            << "Length of oxygen-hydrogen bond (Angstroms): " << OH_LENGTH << endl
            << "H-O-H bond angle (degrees): " << HOH_ANGLE_DEG << endl << endl;
    return;
}

void print_ion_parameters() {
    cout << "Ion sigma (Angstroms): " << setprecision(10) << ION_SIGMA << endl
            << "Ion LJ epsilon (kJ/mol): " << ION_EPSILON << endl
            << "Ion charge: +1.0/-1.0" << endl << endl;
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
