#include "common.h"

ofstream VMD_FILE;
string output_vmd_filename = "SPCE_" + TIMESTAMP() + ".vmd";

void open_vmd_file() {
    VMD_FILE.open(output_vmd_filename.c_str());
    ASSERT(VMD_FILE, "Could not open VMD output file.");
    return;
}

void write_config_to_vmd_file() {
    static int vmd_timestep = 0;
    VMD_FILE << "ITEM: TIMESTEP" << endl
            << vmd_timestep++ << endl
            << "ITEM: NUMBER OF ATOMS" << endl
            << NUM_WATERS << endl
            << "ITEM: BOX BOUNDS" << endl
            << "0 " << BOX_LENGTH << endl
            << "0 " << BOX_LENGTH << endl
            << "0 " << BOX_LENGTH << endl
            << "ITEM: ATOMS" << endl;
    int atom_count = 0;
    for (int i = 0; i < NUM_WATERS; i++)
        VMD_FILE << ++atom_count << " 1 "
            << water_O_positions[i][0] << " "
            << water_O_positions[i][1] << " "
            << water_O_positions[i][2] << endl;
    return;
}

void close_vmd_file() {
    VMD_FILE.close();
    return;
}


void write_config_to_vmd_file() {}