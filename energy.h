/* 
 * File:   energy.h
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 5:46 PM
 */

#pragma once
#ifndef ENERGY_H
#define	ENERGY_H

double energy_of_particle_with_index(int index);
double energy_between_two_waters(int i, int j);
void calculate_and_init_energy();
double total_real_space_energy();
double energy_between_ion_and_water(int i, int j);
double energy_between_two_ions(int i, int j);
double energy_between_two_waters(int i, int j);

inline void update_energy(double delta_energy) {
    TOTAL_ENERGY += delta_energy;
    return;
}

#endif	/* ENERGY_H */
