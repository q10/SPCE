#include "common.h"

const program_flags_t PROGRAM_FLAGS[] = {
    { "temp", required_argument, NULL, 'T'},
    { NULL, no_argument, NULL, 0}
};

double TEMPERATURE = 1.0;
const double DISPLACEMENT = 0.0034; // max random displacement distance in a direction
const int NUM_MC_CYCLES = 200; // number of MC steps

const double WATER_SIGMA = 3.166; // particle diameter
const double WATER_EPSILON = 0.650; // epsilon value for water
const double WATER_DENSITY = 0.0334225755;

const int DATA_SAMPLING_RATE = 10;

double water_positions[NUM_WATERS][3];
double LJEnergy;
