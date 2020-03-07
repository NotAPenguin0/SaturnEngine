#ifndef SATURN_AST_VISITOR_HPP_
#define SATURN_AST_VISITOR_HPP_

#include <cppast/libclang_parser.hpp>

#include <vector>
#include <string>
#include <atomic>
#include <thread>
#include <condition_variable>

#include <stl/bit_flag.hpp>

namespace saturn::codegen {

struct FieldFlags : stl::bit_flag<stl::uint32_t> {
    using bit_flag::bit_flag;

    static constexpr stl::uint32_t SerializeIgnore = 1;
};

struct Field {
    FieldFlags flags;

    std::string type;
    std::string name;
};

struct ComponentMeta {
    std::string name;
    std::string unscoped_name;
    std::vector<Field> fields;
};

struct VisitResult {
    std::vector<ComponentMeta> components;
};

struct VisitorSync {
    std::condition_variable* cv;
    std::mutex* mutex;
    std::atomic<int>* thread_counter;
    std::mutex* results_mutex;
};

std::thread visit_file_ast(std::unique_ptr<cppast::cpp_file> file, VisitResult& result, VisitorSync sync);

}

#endif