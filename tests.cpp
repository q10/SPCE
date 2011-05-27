#include "common.h"

void test_RANDGAUSS() {
    double a;
    int bins[20];
    for (int i = 0; i < 20; i++)
        bins[i] = 0;
    
    for (int i = 0; i < 1000; i++) {
        a = RANDGAUSS(100.0, 0.5);        
        for(int j=0; j<20; j++) {
            if (j == 19 || (a < 98.0 + j*0.2)) {
                bins[j]++;
                break;
            }
        }
    }
    for (int i = 0; i < 20; i++)
        cout << bins[i] << endl;

    return;
}

void test_read_flags(int argc, char** argv) {
    read_program_flags(argc, argv);
    cout << TEMPERATURE << endl;
    return;
}
