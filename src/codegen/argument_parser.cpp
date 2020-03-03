#include <saturn/codegen/argument_parser.hpp>

namespace saturn::codegen {

argument_parser::argument_parser(int argc, char** argv) : argc(argc), argv(argv) {
    parse();
}

void argument_parser::parse() {
    executable_name = argv[0];
    for (size_t i = 1; i < argc; ++i) {
        char const* arg = argv[i];
        // If this is the beginning of a flag parameter
        if (arg[0] == '-') {
            // Remove the starting character from this C-style string by simply incrementing the pointer
            std::string arg_name(arg + 1);
            if (i == argc - 1) {
                // There is no next argument
                parsed[arg_name] = "";
            } else {
                // Check if the next argument is not a flag. In this case, it's a parameter for this flag
                char const* potential_param = argv[i  + 1];
                if (potential_param[0] == '-') {
                    // There is no next argument
                    parsed[arg_name] = "";
                } else {
                    parsed[arg_name] = potential_param;
                    // The next value in the argv array is processed and is our parameter. Advance the index to skip this element
                    ++i;
                }
            }
        }
    }
}

}