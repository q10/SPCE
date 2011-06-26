/* 
 * File:   rotation.h
 * Author: BENSON J MA
 *
 * Created on June 17, 2011, 7:25 PM
 */

#pragma once
#ifndef ROTATION_H
#define	ROTATION_H

double * center_of_mass_of_water_with_index(int i);
double ** rotation_matrix(double * rand_unit_vector, double theta_rad);

void test_rotation();

#endif	/* ROTATION_H */

