/* 
 * File:   mc.h
 * Author: BENSON J MA
 *
 * Created on May 26, 2011, 12:03 AM
 */

#pragma once
#ifndef MC_H
#define	MC_H

extern int NUM_MC_ATTEMPTS; // number of single particle move attempts per MC sweep
extern int NUM_MC_SWEEPS;   // number of MC sweeps to run

void run_mc();
void mc_sweep();

#endif	/* MC_H */
