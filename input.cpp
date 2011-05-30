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
                if (PROGRAM_FLAGS[options_i].flag != 0)
                    break;
                printf("option %s", PROGRAM_FLAGS[options_i].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;
            case '?':
                break;
            case 'T':
                TEMPERATURE = atof(optarg);
                break;
            case 'n':
                NUM_WATERS = abs(atoi(optarg));
                break;
            default:
                break;
        }
    }
    return;
}
