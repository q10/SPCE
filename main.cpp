/* 
 * File:   main.cpp
 * Author: BENSON J MA
 *
 * Created on May 25, 2011, 12:50 PM
 */

#include "common.h"

int main(int argc, char** argv) {
    for(int i=0; i<100; i++)
        cout<<RANDINT(100, 10000)<<endl;
    return 0;
}

