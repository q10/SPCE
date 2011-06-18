/* 
 * File:   mc.h
 * Author: BENSON J MA
 *
 * Created on May 26, 2011, 12:03 AM
 */

#pragma once
#ifndef MC_H
#define	MC_H

extern int NUM_MC_ATTEMPTS_PER_SWEEP;             // number of single particle move attempts per MC sweep
extern int NUM_MC_SWEEPS;               // number of MC sweeps to run
extern int NUM_EQUILIBRATION_SWEEPS;    // number of MC sweeps to run to equilibrate the system

void run_mc();
void mc_equilibrate();
void mc_sweep();
void mc_move();
void mc_rotate();

#endif	/* MC_H */
