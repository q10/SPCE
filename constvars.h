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
#define TEMPERATURE 1.0
#define NUM_WATERS 108

extern const double SIGMA; // particle diameter
extern const double DISPLACEMENT; // max random displacement distance 
extern const int NUM_MC_STEPS; // number of MC steps

extern double water_positions[NUM_WATERS][3];
extern double LJEnergy;

#endif	/* CONSTVARS_H */
