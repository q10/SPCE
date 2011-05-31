/* 
 * File:   constvars.h
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 4:50 PM
 */

#pragma once
#ifndef CONSTVARS_H
#define	CONSTVARS_H

#define BUF_SIZE 256
#define HALF_BOX_LENGTH BOX_LENGTH/2.0

typedef struct option program_flags_t;

extern const program_flags_t PROGRAM_FLAGS[];
extern const double DISPLACEMENT; // max random displacement distance 
extern const int NUM_MC_CYCLES; // number of MC cycles to run
extern const double BOLTZMANN_K;
extern double TEMPERATURE;
extern double BOX_LENGTH;
extern int NUM_WATERS;


// water properties
extern const double WATER_SIGMA; // diameter
extern const double WATER_EPSILON;
extern const double WATER_DENSITY;

// sampling
extern const int EQUILIBRATION_TIME;
extern const int DATA_SAMPLING_RATE;
extern int radial_dist_num_his_bars;
extern double * radial_dist_data;

extern double ** water_positions;
extern double LJEnergy;

#endif	/* CONSTVARS_H */
