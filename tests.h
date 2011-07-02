/* 
 * File:   tests.h
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 4:40 PM
 */

#pragma once
#ifndef TESTS_H
#define	TESTS_H

void run_tests(int argc, char** argv);
void test_RANDGAUSS();
void test_read_program_flags(int argc, char** argv);
void test_energy_of_water_with_index();
void test_run_mc();
void test_read_config_file(int argc, char** argv);
void test_mc_acceptances();

void time_exp_vs_pow ();
void time_new_operator_overhead();
void time_pow_vs_division();

#endif	/* TESTS_H */
