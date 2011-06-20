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
void test_radial_dist_sampler();
void test_write_to_vmd_file(int argc, char** argv);
void test_write_to_config_file(int argc, char** argv);
void test_read_config_file(int argc, char** argv);
void test_rotation();
void test_mc_acceptances();

#endif	/* TESTS_H */
