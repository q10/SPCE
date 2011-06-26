/* 
 * File:   output.h
 * Author: BENSON J MA
 *
 * Created on June 4, 2011, 10:06 PM
 */

#pragma once
#ifndef OUTPUT_H
#define	OUTPUT_H

using namespace std;

//extern int vmd_timestep;
extern bool use_custom_output_vmd_filename;
extern bool use_custom_output_config_filename;
extern ofstream VMD_FILE;
extern ofstream OUTPUT_CONFIG_FILE;
extern string output_vmd_filename;
extern string output_config_filename;

void open_vmd_file();
void write_config_to_vmd_file();
void close_vmd_file();
void save_config_to_file();

void test_write_to_vmd_file(int argc, char** argv);
void test_write_to_config_file(int argc, char** argv);

#endif	/* OUTPUT_H */
