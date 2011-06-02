#include "common.h"

const double DISPLACEMENT = 0.0034; // max random displacement distance in a direction
const int NUM_MC_CYCLES = 2000; // number of MC steps
double TEMPERATURE = 300.0;
int NUM_WATERS = 200;

const double WATER_SIGMA = 3.166; // particle diameter
const double WATER_EPSILON = 0.650; // epsilon value for water, kJ/mol
const double WATER_DENSITY = 0.0334225755;
const double BOLTZMANN_K = 0.00831447122; // units of kJ/(K*mol)

double BOX_LENGTH; // in Angstroms

const int EQUILIBRATION_TIME = 0;
const int DATA_SAMPLING_RATE = 200;

double ** water_positions;
double LJEnergy;
