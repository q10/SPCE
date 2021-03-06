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
dcomplex partial_rho(int water_index, double * k_coords);
double total_ewald_energy();
double ewald_diff(int index);
double ewald_diff_water(int water_index);
double ewald_diff_ion(int index);
void set_exp_kr_table_for_water(int water_index);
void set_exp_kr_table_for_ion(int ion_index);

void test_rho_function();
void test_k_vector_table();

#endif	/* EWALD_H */
