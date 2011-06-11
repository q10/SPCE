#include "common.h"

bool use_custom_output_vmd_filename = false;
bool use_custom_output_config_filename = false;
ofstream VMD_FILE;
ofstream OUTPUT_CONFIG_FILE;
string output_vmd_filename;
string output_config_filename;

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
            << 3 * NUM_WATERS << endl
            << "ITEM: BOX BOUNDS" << endl
            << "0 " << BOX_LENGTH << endl
            << "0 " << BOX_LENGTH << endl
            << "0 " << BOX_LENGTH << endl
            << "ITEM: ATOMS" << endl;
    int atom_count = 0;
    for (int i = 0; i < NUM_WATERS; i++) {
        VMD_FILE << ++atom_count << " 1 "
                << water_positions[i][0] / BOX_LENGTH << " "
                << water_positions[i][1] / BOX_LENGTH << " "
                << water_positions[i][2] / BOX_LENGTH << endl;
        VMD_FILE << ++atom_count << " 2 "
                << water_positions[i][3] / BOX_LENGTH << " "
                << water_positions[i][4] / BOX_LENGTH << " "
                << water_positions[i][5] / BOX_LENGTH << endl;
        VMD_FILE << ++atom_count << " 2 "
                << water_positions[i][6] / BOX_LENGTH << " "
                << water_positions[i][7] / BOX_LENGTH << " "
                << water_positions[i][8] / BOX_LENGTH << endl;
    }
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
            OUTPUT_CONFIG_FILE << "\t" << setprecision(10) << water_positions[i][j];
        OUTPUT_CONFIG_FILE << endl;
    }

    OUTPUT_CONFIG_FILE.close();
    return;
}
