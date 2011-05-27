#include "common.h"

const program_flags_t PROGRAM_FLAGS[] = {
    { "temp", required_argument, NULL, 't'},
    { NULL, no_argument, NULL, 0 }
};

double TEMPERATURE = 1.0;
const double SIGMA = 3.166; // particle diameter
const double DISPLACEMENT = 0.0034; // max random displacement distance in a direction
const double WATER_EPSILON = 0.650; // epsilon value for water
const int NUM_MC_CYCLES = 200; // number of MC steps

double water_positions[NUM_WATERS][3];
double LJEnergy;
