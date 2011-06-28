/* 
 * File:   energy.h
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 5:46 PM
 */

#pragma once
#ifndef ENERGY_H
#define	ENERGY_H

double energy_of_water_with_index(int index);
double energy_between_two_waters(int i, int j);
void calculate_and_init_energy();
double energy_between_two_waters(int i, int j);

inline void update_energy(double delta_energy) {
    LJEnergy += delta_energy;
    return;
}

#endif	/* ENERGY_H */
