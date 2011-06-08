#include "common.h"

const double BOLTZMANN_K = 0.00831447122;
const double ELECTROSTATIC_K = 1389.354325379097;

double TEMPERATURE = 300.0;
double BETA = 1.0 / (BOLTZMANN_K * TEMPERATURE);
int NUM_WATERS = 200;
double BOX_LENGTH; 
const double DISPLACEMENT = 0.0068;

const double WATER_SIGMA = 3.166;
const double WATER_EPSILON = 0.650;
const double WATER_DENSITY = 0.0334225755;
const double WATER_Q_H = 0.4238;
const double WATER_Q_O = -0.8476;

const int EQUILIBRATION_TIME = 0;

double ** water_O_positions;
double LJEnergy;
