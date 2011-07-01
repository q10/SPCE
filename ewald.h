/* 
 * File:   ewald.h
 * Author: BENSON J MA
 *
 * Created on June 24, 2011, 10:55 PM
 */

#ifndef EWALD_H
#define	EWALD_H

void initialize_erfc_table();
void initialize_k_vectors_table();
void initialize_rho_k_values_table();
dcomplex rho_k_diff(int water_index, double * k_vect, double * old_position);
dcomplex rho(double * k_coords);
dcomplex partial_rho(int water_index, double * k_coords);
double ewald_sum();
double ewald_diff(int water_index);
void set_exp_kr_table(int water_index);

void test_rho_function();
void test_k_vector_table();

#endif	/* EWALD_H */
