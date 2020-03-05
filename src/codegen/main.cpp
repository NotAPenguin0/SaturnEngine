#include <thread>
#include <iostream>
#include <sstream>
#include <atomic>
#include <condition_variable>

#include <saturn/codegen/category_generator.hpp>
#include <saturn/codegen/argument_parser.hpp>
#include <saturn/codegen/file_parser.hpp>
#include <saturn/codegen/ast_visitor.hpp>

#include <chrono>

namespace fs = std::filesystem;

std::vector<std::string> str_split(std::string const& s, char delim = ' ') {
    std::stringstream ss;
    ss << s;
    std::vector<std::string> result;
    std::string elem;
    while(std::getline(ss, elem, delim)) {
        result.push_back(elem);
    }
    return result;
}

void fill_include_dirs(saturn::codegen::argument_parser const& args, cppast::libclang_compile_config& config) {
    auto split = str_split(args.get_argument("i"), ';');
    for (auto const& dir : split) {
        config.add_include_dir(dir);
    }
}

int main(int argc, char** argv) {
    saturn::codegen::argument_parser args(argc, argv);

    cppast::libclang_compile_config config;
    config.set_flags(cppast::cpp_standard::cpp_1z);
    fill_include_dirs(args, config);

    using namespace std::chrono;
    milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    auto results = saturn::codegen::parse_directory("include/saturn/components", config);
    
    std::vector<std::thread> visitor_threads;
    saturn::codegen::VisitResult visit_results;

    std::atomic<int> thread_counter = 0;
    std::condition_variable cv;
    std::mutex mutex;
    std::mutex results_mutex;

    saturn::codegen::VisitorSync sync {&cv, &mutex, &thread_counter, &results_mutex};

    for (size_t i = 0; i < results.size(); ++i) {
        auto& result = results[i];
        result.wait();
        auto parsed = result.get();
        // Launch a new thread
        visitor_threads.emplace_back(saturn::codegen::visit_file_ast(std::move(parsed), visit_results, sync)).detach();
        ++thread_counter;
    }

    std::unique_lock lock(mutex);
    cv.wait(lock, [&]() -> bool { return thread_counter == 0; });

    milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    std::cout << "Parsing all components took " << (end - start).count() << " ms" << std::endl;

    saturn::codegen::generate_type_category_file(visit_results);

    return 0;
}