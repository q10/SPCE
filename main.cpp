/* 
 * File:   main.cpp
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 12:50 PM
 */

#include "common.h"

int main(int argc, char** argv) {
    run_tests(argc, argv);
    string s;
    if (s.compare("") == 0)
        cout<<"true\n";
    else
        cout<<"false\n";
    /*
    read_program_flags(argc, argv);
    initialize();    
    run_mc();
    print_system_vars();
    print_system_config();
    save_config_to_file();
    */
    return 0;
}
