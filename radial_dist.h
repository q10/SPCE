/* 
 * File:   radial_dist.h
 * Author: BENSON J MA
 *
 * Created on May 27, 2011, 2:07 AM
 */

#pragma once
#ifndef RADIAL_DIST_H
#define	RADIAL_DIST_H

extern int radial_dist_num_his_bars;
extern double * radial_dist_distance;
extern double * radial_dist_data;

void initialize_radial_dist_sampler();
void radial_dist_sample();
void compute_radial_dist_results();
void print_radial_dist_results();

void test_radial_dist_sampler();

#endif	/* RADIAL_DIST_H */
