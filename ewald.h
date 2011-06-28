/* 
 * File:   ewald.h
 * Author: BENSON J MA
 *
 * Created on June 24, 2011, 10:55 PM
 */

#ifndef EWALD_H
#define	EWALD_H

void initialize_erfc_table();
void initialize_k_values_table();
void initialize_rho_k_values_table();
dcomplex rho_k_diff(int water_index, double * k_vect, double * old_position);
dcomplex rho(double * k_coords);
double ewald_sum();
double ewald_diff(int water_index, double * old_position, std::vector <dcomplex> * rho_k_diff_values);

void test_rho_function();
void test_k_table();

#endif	/* EWALD_H */
