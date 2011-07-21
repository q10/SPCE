#include "common.h"

const double BOLTZMANN_K = 0.00831447122;
const double ELECTROSTATIC_K = 1389.354325379097;

int NUM_WATERS = 200;
int NUM_IONS = 1;
int NUM_TOTAL_PARTICLES = NUM_WATERS + NUM_IONS;
double TEMPERATURE = 300.0;
double BETA = 1.0 / (BOLTZMANN_K * TEMPERATURE);
double BOX_LENGTH;
double BOX_VOLUME;
double DISPLACEMENT_DISTANCE = 0.2;
double DISPLACEMENT_ROTATION = 0.17 * M_PI;
double EWALD_ALPHA = 0.0784;

const double WATER_SIGMA = 3.166;
const double ION_SIGMA = 5.0;
const double ION_EPSILON = 1.0;
const double WATER_EPSILON = 0.650;
const double WATER_DENSITY = 0.0334225755;
const double WATER_Q_H = 0.4238;
const double WATER_Q_O = -0.8476;
const double H_MASS = 1.0;
const double O_MASS = 16.0;
const double WATER_MASS = 18.0;
const double OH_LENGTH = 1.0000;
const double HOH_ANGLE_DEG = 109.47;

map <double, double> * ERFC_TABLE = new map <double, double>;
double ** K_VECTORS = new double * [725];
dcomplex ** RHO_K_VALUES = new dcomplex * [725];

double ** WATER_POSITIONS;
double ** IONS;
double TOTAL_ENERGY;
