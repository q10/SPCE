/* 
 * File:   constvars.h
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 4:50 PM
 */

#pragma once
#ifndef CONSTVARS_H
#define	CONSTVARS_H

#define BUF_SIZE 1024
#define HALF_BOX_LENGTH BOX_LENGTH/2.0

// physical constants
extern const double BOLTZMANN_K;        // units of kJ/(mol*K)

// system properties
extern double TEMPERATURE;
extern double BETA;
extern int NUM_WATERS;
extern double BOX_LENGTH;               // box side length (Angstroms)
extern const double DISPLACEMENT;       // max random displacement distance per attempted move (Angstroms)

// water properties
extern const double WATER_SIGMA;        // diameter (Angstroms)
extern const double WATER_EPSILON;      // energy unit (kJ/mol)
extern const double WATER_DENSITY;      // particles per Angstroms^3
extern const double WATER_Q_H;          // charge of hydrogen (elementary charge unit)
extern const double WATER_Q_O;          // charge of oxygen (elementary charge unit)

// other system variables
extern const int EQUILIBRATION_TIME;

// state variables
extern double ** water_O_positions;     // table of oxygen positions
extern double LJEnergy;                 // Lennard-Jones Potential (kJ/mol)

#endif	/* CONSTVARS_H */
