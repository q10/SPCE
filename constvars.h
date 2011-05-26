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

const double SIGMA = 2; // particle diameter
const double DISPLACEMENT = 0.0034; // max random displacement distance 
const int NUM_MC_STEPS = 200; // number of MC steps

static double water_positions[NUM_WATERS][3];
static double LJEnergy;

#endif	/* CONSTVARS_H */