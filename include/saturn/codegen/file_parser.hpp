#ifndef SATURN_CODEGEN_FILE_PARSER_HPP_
#define SATURN_CODEGEN_FILE_PARSER_HPP_

#include <future>
#include <vector>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

#include <cppast/libclang_parser.hpp>


namespace saturn::codegen {

using libclang_parse_result = std::future<std::unique_ptr<cppast::cpp_file>>;

libclang_parse_result parse_file(fs::path const& path, cppast::libclang_compile_config const& config);

std::vector<libclang_parse_result> parse_directory(fs::path const& path, cppast::libclang_compile_config const& config);

std::vector<libclang_parse_result> parse_directory_recursive(fs::path const& path, cppast::libclang_compile_config const& config);

}

#endif