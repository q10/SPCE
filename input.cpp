#include "common.h"

bool using_input_config_file = false;
char * input_config_filename;

const program_flags_t PROGRAM_FLAGS[] = {
    { "temp", required_argument, NULL, 'T'},
    { "num_waters", required_argument, NULL, 'n'},
    { "input_config", required_argument, NULL, 'r'},
    { NULL, no_argument, NULL, 0}
};

void read_program_flags(int argc, char** argv) {
    int option;

    while (1) {
        /* getopt_long stores the option index here. */
        int options_i = 0;
        char * argstr = "t:n:r:";

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
            case 'r':
                using_input_config_file = true;
                input_config_filename = optarg;
                break;
            default:
                break;
        }
    }
    return;
}

void load_configuration_file() {
    vector< vector< double > > particles;
    ifstream input_filestream(input_config_filename);
    ASSERT(input_filestream.is_open(), "Could not open input configuration file.");
    
    std::string line;
    while (std::getline(input_filestream, line)) {
        std::vector< double > coords;
        std::istringstream iss(line);
        double val;
        while (iss >> val)
            coords.push_back(val);
        ASSERT((int) coords.size() == 3, "Not enough coordinates.");
        particles.push_back(coords);
    }

    // initialize constants
    NUM_WATERS = (int) particles.size();
    initialize_constants();

    // convert data structure back to primitive array for performance optimization
    water_O_positions = new double*[NUM_WATERS];
    for (int i = 0; i < NUM_WATERS; i++) {
        water_O_positions[i] = new double[3];
        for (int j = 0; j < 3; j++)
            water_O_positions[i][j] = particles[i][j];
    }
    return;
}
