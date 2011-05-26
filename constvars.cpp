#include "common.h"

const double SIGMA = 2.0; // particle diameter
const double DISPLACEMENT = 0.0034; // max random displacement distance 
const int NUM_MC_STEPS = 200; // number of MC steps

double water_positions[NUM_WATERS][3];
double LJEnergy;
