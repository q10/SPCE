#include "common.h"

bool using_input_config_file = false;
string input_config_filename;

const program_flags_t PROGRAM_FLAGS[] = {
    { "temp", required_argument, NULL, 't'},
    { "num_waters", required_argument, NULL, 'n'},
    { "input", required_argument, NULL, 'r'},
    { "output_config", required_argument, NULL, 'w'},
    { "output_vmd", required_argument, NULL, 'v'},
    { NULL, no_argument, NULL, 0}
};

void read_program_flags(int argc, char** argv) {
    int option;

    while (1) {
        /* getopt_long stores the option index here. */
        int options_i = 0;
        string argstr = "t:n:r:w:v:ABCDEFGHIJKLMNNOPQRSTUVWXYZabcdefghijklmopqsuxyz";

        // "abc:d:f:" means that a and b don't have args, while c, d, f do
        option = getopt_long_only(argc, argv, argstr.c_str(), PROGRAM_FLAGS, &options_i);

        /* Detect the end of the options. */
        if (option == -1 or option == '?')
            break;
        else if (option == 0) {
            if (PROGRAM_FLAGS[options_i].flag != 0)
                break;
            printf("option %s", PROGRAM_FLAGS[options_i].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;
        } else if ((option == 't') && (abs(atof(optarg)) > 0.0))
            TEMPERATURE = abs(atof(optarg));
        else if ((option == 'n') && (abs(atoi(optarg)) > 0))
            NUM_WATERS = abs(atoi(optarg));
        else if (option == 'r') {
            using_input_config_file = true;
            input_config_filename = optarg;
            if (input_config_filename[0] == '=')
                input_config_filename = input_config_filename.substr(1);
        } else if (option == 'w') {
            use_custom_output_config_filename = true;
            output_config_filename = optarg;
            if (output_config_filename[0] == '=')
                output_config_filename = output_config_filename.substr(1);
        } else if (option == 'v') {
            use_custom_output_vmd_filename = true;
            output_vmd_filename = optarg;
            if (output_vmd_filename[0] == '=')
                output_vmd_filename = output_vmd_filename.substr(1);
        } else
            ASSERT(false, "Invalid program flag or missing a flag parameter.");
    }
    return;
}

void load_configuration_file() {
    ifstream input_filestream(input_config_filename.c_str());
    ASSERT(input_filestream.is_open(), "Could not open input configuration file " + STRING(input_config_filename));

    vector< vector< double > > particles;
    int line_num = 0;
    string line, line_key;

    while (std::getline(input_filestream, line)) {
        std::istringstream iss(line);
        line_num++;
        iss >> line_key;

        if (line_key.compare("TEMPERATURE") == 0) {
            double temp;
            iss >> temp;
            if (abs(temp) > 0.0)
                TEMPERATURE = abs(temp);

        } else if (line_key.compare("ION") == 0) {

        } else if (line_key.compare("WATER") == 0) {
            std::vector< double > coords;
            double val;

            while (iss >> val)
                coords.push_back(val);
            ASSERT((int) coords.size() == 3, "Not enough coordinates on line " + STRING(line_num) + " of config file.");
            particles.push_back(coords);
        } else
            cerr << "WARNING: Malformed line " << line_num << " in config file - Ignoring this line." << endl;
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
