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

extern int vmd_timestep;
extern ofstream VMD_FILE;
extern string vmd_filename;

void open_vmd_file(char * filename);
void write_config_to_vmd_file();
void close_vmd_file();

#endif	/* OUTPUT_H */
