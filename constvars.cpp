#include "common.h"

const program_flags_t PROGRAM_FLAGS[] = {
    { "temp", required_argument, NULL, 'T'},
    { "num_waters", required_argument, NULL, 'n'},
    { NULL, no_argument, NULL, 0}
};

const double DISPLACEMENT = 0.0034; // max random displacement distance in a direction
const int NUM_MC_CYCLES = 200; // number of MC steps
double TEMPERATURE = 300.0;
int NUM_WATERS = 200;

const double WATER_SIGMA = 3.166; // particle diameter
const double WATER_EPSILON = 0.650; // epsilon value for water
const double WATER_DENSITY = 0.0334225755;

double BOX_LENGTH; // in Angstroms

const int DATA_SAMPLING_RATE = 10;

double ** water_positions;
double LJEnergy;
