#include "common.h"

bool use_custom_output_vmd_filename = false;
bool use_custom_output_config_filename = false;
ofstream VMD_FILE, OUTPUT_CONFIG_FILE;
string output_vmd_filename, output_config_filename;

void open_vmd_file() {
    if (!use_custom_output_vmd_filename)
        output_vmd_filename = "SPCE_" + TIMESTAMP() + ".lammpstrj";
    VMD_FILE.open(output_vmd_filename.c_str());
    ASSERT(VMD_FILE, "Could not open VMD output file.");
    return;
}

void write_config_to_vmd_file() {
    static int vmd_timestep = 0;
    cerr << "Writing timestep " << vmd_timestep << " to VMD file.\n" << endl;

    VMD_FILE << "ITEM: TIMESTEP" << endl
            << vmd_timestep++ << endl
            << "ITEM: NUMBER OF ATOMS" << endl
            << (3 * NUM_WATERS + NUM_IONS) << endl
            << "ITEM: BOX BOUNDS" << endl
            << "0 " << BOX_LENGTH << endl
            << "0 " << BOX_LENGTH << endl
            << "0 " << BOX_LENGTH << endl
            << "ITEM: ATOMS" << endl;
    int atom_count = 0;
    for (int i = 0; i < NUM_WATERS; i++) {
        VMD_FILE << ++atom_count << " 1 "
                << WATER_POSITIONS[i][0] / BOX_LENGTH << " "
                << WATER_POSITIONS[i][1] / BOX_LENGTH << " "
                << WATER_POSITIONS[i][2] / BOX_LENGTH << endl;
        VMD_FILE << ++atom_count << " 2 "
                << WATER_POSITIONS[i][3] / BOX_LENGTH << " "
                << WATER_POSITIONS[i][4] / BOX_LENGTH << " "
                << WATER_POSITIONS[i][5] / BOX_LENGTH << endl;
        VMD_FILE << ++atom_count << " 2 "
                << WATER_POSITIONS[i][6] / BOX_LENGTH << " "
                << WATER_POSITIONS[i][7] / BOX_LENGTH << " "
                << WATER_POSITIONS[i][8] / BOX_LENGTH << endl;
    }

    for (int i = 0; i < NUM_IONS; i++)
        VMD_FILE << ++atom_count << " 3 "
            << IONS[i][0] / BOX_LENGTH << " "
            << IONS[i][1] / BOX_LENGTH << " "
            << IONS[i][2] / BOX_LENGTH << endl;
    return;
}

void close_vmd_file() {
    VMD_FILE.close();
    return;
}

void save_config_to_file() {
    if (!use_custom_output_config_filename)
        output_config_filename = "SPCE_" + TIMESTAMP() + ".config";

    OUTPUT_CONFIG_FILE.open(output_config_filename.c_str());
    ASSERT(OUTPUT_CONFIG_FILE, "Could not open config output file.");

    OUTPUT_CONFIG_FILE << "TEMPERATURE\t" << TEMPERATURE << endl;

    for (int i = 0; i < NUM_WATERS; i++) {
        OUTPUT_CONFIG_FILE << "WATER";
        for (int j = 0; j < 9; j++)
            OUTPUT_CONFIG_FILE << "\t" << setprecision(10) << WATER_POSITIONS[i][j];
        OUTPUT_CONFIG_FILE << endl;
    }

    for (int i = 0; i < NUM_IONS; i++) {
        OUTPUT_CONFIG_FILE << "ION";
        for (int j = 0; j < 4; j++)
            OUTPUT_CONFIG_FILE << "\t" << setprecision(10) << IONS[i][j];
        OUTPUT_CONFIG_FILE << endl;
    }

    OUTPUT_CONFIG_FILE.close();
    return;
}

void test_write_to_vmd_file(int argc, char** argv) {
    cout << "---- BEGIN TEST - WRITE TO VMD FILE ----" << endl;

    read_program_flags(argc, argv);
    NUM_EQUILIBRATION_SWEEPS = 0;
    initialize();
    open_vmd_file();
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
    initialize_prog_flag_affected_constants();
    initialize_waters();
    save_config_to_file();
    cout << "---- END TEST - WRITE CONFIG FILE ----\n" << endl;
    return;
}
