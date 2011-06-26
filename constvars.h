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

typedef std::complex<double> dcomplex;

// physical constants
extern const double BOLTZMANN_K;        // units of kJ/(mol*K)
extern const double ELECTROSTATIC_K;    // units of (kJ*mol*Angstrom)/(elementary_charge^2) (see below for notes on obtaining this constant)

// system properties
extern double TEMPERATURE;              // Kelvins
extern double BETA;                     // mol/kJ
extern int NUM_WATERS;
extern double BOX_LENGTH;               // box side length (Angstroms)
extern double DISPLACEMENT_DISTANCE;    // max random displacement distance per attempted move (Angstroms)
extern double DISPLACEMENT_ROTATION;    // max random displacement angle per attempted molecule rotation (radians)
extern double EWALD_ALPHA;              // Ewald screening parameter

// water properties
extern const double WATER_SIGMA;        // diameter (Angstroms)
extern const double WATER_EPSILON;      // energy unit (kJ/mol)
extern const double WATER_DENSITY;      // particles per Angstroms^3
extern const double WATER_Q_H;          // charge of hydrogen (elementary charge unit)
extern const double WATER_Q_O;          // charge of oxygen (elementary charge unit)
extern const double H_MASS;             // mass of hydrogen (amu)
extern const double O_MASS;             // mass of oxygen (amu)
extern const double WATER_MASS;         // mass of water (amu)
extern const double OH_LENGTH;          // oxygen-hydrogen bond length (Angstroms)
extern const double HOH_ANGLE_DEG;      // HOH bond angle (degrees)

// other system variables
extern std::map <double, double> * ERFC_TABLE;
extern std::map <int, std::map <int, std::map <int, double *> *> *> * K_VALUES;

// state variables
extern double ** water_positions;     // table of oxygen positions
extern double LJEnergy;               // Lennard-Jones Potential (kJ/mol)

#endif	/* CONSTVARS_H */

/*
 Obtaining the electrostatic constant ELECTROSTATIC_K:
 
 Since charge is in partial elementary charges and distance is in Angstroms, it 
 is easier to convert elementary charges into cgs units (statcoulombs).  From 
 there, convert statcoulombs^2 into ergs*cm.  Convert ergs into Joules, then to 
 kiloJoules, and convert the cm into meters.  Convert the Angstroms on the 
 bottom to meters to cancel out, and multiply this amount by 6.0221407818E23 to 
 obtain kJ/mol instead of kJ.  The resulting calculations should simplify to:
 
((4.8032042712E-10)**2)*(6.0221407818E23)*(1E-2)
 */
