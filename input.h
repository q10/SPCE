/* 
 * File:   input.h
 * Author: BENSON J MA
 *
 * Created on May 26, 2011, 5:19 PM
 */

#pragma once
#ifndef INPUT_H
#define	INPUT_H

extern bool using_input_config_file;
extern std::string input_config_filename;

typedef struct option program_flags_t;
extern const program_flags_t PROGRAM_FLAGS[];

void read_program_flags(int argc, char** argv);
void load_configuration_file();

#endif	/* INPUT_H */
