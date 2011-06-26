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
dcomplex rho(double * coords);

void test_rho_function();
void test_k_table();

#endif	/* EWALD_H */
