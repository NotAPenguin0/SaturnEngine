#include <saturn/codegen/utility.hpp>

#include <fstream>

namespace saturn::codegen {

std::string read_file(fs::path const& path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

std::string to_filename(std::string_view in) {
    std::string s;
    bool begin = true;
    for (auto c : in) {
        if (std::isupper(c) && !begin) {
            s.push_back('_');
        }
        s.push_back(std::tolower(c));

        begin = false;
    }
    return s;
}

}