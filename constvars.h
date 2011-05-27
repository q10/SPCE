/* 
 * File:   constvars.h
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 4:50 PM
 */

#ifndef CONSTVARS_H
#define	CONSTVARS_H

#define BUF_SIZE 256
#define BOX_LENGTH 5.0
#define HALF_BOX_LENGTH BOX_LENGTH/2.0
#define NUM_WATERS 108

typedef struct option program_flags_t;

extern const program_flags_t PROGRAM_FLAGS[];
extern double TEMPERATURE;
extern const double SIGMA; // particle diameter
extern const double WATER_EPSILON;
extern const double DISPLACEMENT; // max random displacement distance 
extern const int NUM_MC_CYCLES; // number of MC cycles to run

extern double water_positions[NUM_WATERS][3];
extern double LJEnergy;

#endif	/* CONSTVARS_H */
