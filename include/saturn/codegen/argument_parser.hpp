#ifndef SATURN_CODEGEN_ARGUMENT_PARSER_HPP_
#define SATURN_CODEGEN_ARGUMENT_PARSER_HPP_

#include <unordered_map>
#include <string>
#include <string_view>

namespace saturn::codegen {

class argument_parser {
public:
    argument_parser(int argc, char** argv);

    int get_argc() const {
        return argc;
    }

    char const* const* get_argv() const {
        return argv;
    }

    std::string_view get_executable_name() const {
        return executable_name;
    }

    std::string const& get_argument(std::string const& key) const {
        return parsed.at(key);
    }

private:
    int argc;
    char** argv;
  
    std::string executable_name;
    std::unordered_map<std::string, std::string> parsed;

    void parse();
  
};

}

#endif