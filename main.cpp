/* 
 * File:   main.cpp
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 12:50 PM
 */

#include "common.h"

int main(int argc, char** argv) {
    for(int i=0; i<100; i++)
        cout<<RANDGAUSS(100.0, 2.0)<<endl;
    return 0;
}

