/* 
 * File:   main.cpp
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 12:50 PM
 */

#include "common.h"

int main(int argc, char** argv) {
    test_read_program_flags(argc, argv);
    initialize();
    test_RANDGAUSS();
    /*
    initialize();
    cout<<LJEnergy<<endl;
    run_mc();
    cout<<LJEnergy<<endl;
    cout<<SIGMA<<endl;
     */
    return 0;
}
