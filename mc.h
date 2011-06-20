/* 
 * File:   mc.h
 * Author: BENSON J MA
 *
 * Created on May 26, 2011, 12:03 AM
 */

#pragma once
#ifndef MC_H
#define	MC_H

extern int NUM_MC_ATTEMPTS_PER_SWEEP;   // number of single particle move attempts per MC sweep
extern int NUM_MC_SWEEPS;               // number of MC sweeps to run
extern int NUM_EQUILIBRATION_SWEEPS;    // number of MC sweeps to run to equilibrate the system

extern int num_successful_mc_moves;
extern int num_successful_mc_rotations;
extern int total_attempted_mc_moves;
extern int total_attempted_mc_rotations;

void reset_mc_acceptance_records();
void print_mc_acceptance_records();
void run_mc();
void mc_equilibrate();
void mc_sweep();
void mc_move();
void mc_rotate();
void keep_water_inside_box(int index);
bool mc_accept(int index, double old_energy_diff, double * old_position);

#endif	/* MC_H */
