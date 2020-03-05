#ifndef SATURN_CODEGEN_UTILITY_HPP_
#define SATURN_CODEGEN_UTILITY_HPP_

#include <string>
#include <filesystem>

namespace fs = std::filesystem;

namespace saturn::codegen {

std::string read_file(fs::path const& path);

}

#endif