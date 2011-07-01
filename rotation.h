/* 
 * File:   rotation.h
 * Author: BENSON J MA
 *
 * Created on June 17, 2011, 7:25 PM
 */

#pragma once
#ifndef ROTATION_H
#define	ROTATION_H

extern double ** ROTATION_MATRIX;
extern double * TMP_CENTER_OF_MASS;

void initialize_rotation_matrix();
void set_center_of_mass_of_water_with_index(int i);
void set_rotation_matrix(double * rand_unit_vector, double theta_rad);

void test_rotation();

#endif	/* ROTATION_H */

