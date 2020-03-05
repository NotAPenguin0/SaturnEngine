#include <saturn/codegen/utility.hpp>

#include <fstream>

namespace saturn::codegen {

std::string read_file(fs::path const& path) {
    std::ifstream in(path);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

}