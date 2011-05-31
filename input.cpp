#include "common.h"

const program_flags_t PROGRAM_FLAGS[] = {
    { "temp", required_argument, NULL, 'T'},
    { "num_waters", required_argument, NULL, 'n'},
    { NULL, no_argument, NULL, 0}
};

void read_program_flags(int argc, char** argv) {
    int option;

    while (1) {
        /* getopt_long stores the option index here. */
        int options_i = 0;
        char * argstr = "t:n:";

        // "abc:d:f:" means that a and b dont have args, while c, d, f do
        option = getopt_long_only(argc, argv, argstr, PROGRAM_FLAGS, &options_i);

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
                if (abs(atof(optarg)) > 0.0)
                    TEMPERATURE = abs(atof(optarg));
                break;
            case 'n':
                if (abs(atoi(optarg)) > 0)
                    NUM_WATERS = abs(atoi(optarg));
                break;
            default:
                break;
        }
    }
    return;
}
