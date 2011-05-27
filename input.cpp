#include "common.h"

void read_program_flags(int argc, char** argv) {
    int option;

    while (1) {
        /* getopt_long stores the option index here. */
        int options_i = 0;

        // "abc:d:f:" means that a and b dont have args, while c, d, f do
        option = getopt_long_only(argc, argv, "t:", PROGRAM_FLAGS, &options_i);

        /* Detect the end of the options. */
        if (option == -1)
            break;

        switch (option) {
            case 0:
                break;
            case 't':
                TEMPERATURE = atof(optarg);
                break;
            case '?':
                break;
            default:
                abort();
        }
    }
    return;
}
