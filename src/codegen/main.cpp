#include <cppast/libclang_parser.hpp>

#include <filesystem>
#include <thread>
#include <iostream>
#include <vector>
#include <memory>
#include <future>

namespace fs = std::filesystem;

class thread_safe_logger : public cppast::diagnostic_logger {
public:
    using cppast::diagnostic_logger::diagnostic_logger;
       
    void write_raw(const char* str) const {
        std::lock_guard lock(write_mutex);
        std::fprintf(stderr, "%s\n", str);
   }

private:
    bool do_log(const char* source, cppast::diagnostic const& d) const override {
        std::lock_guard lock(write_mutex);
        auto loc = d.location.to_string();
        if (loc.empty()) {
            std::fprintf(stderr, "[%s] [%s] %s\n", source, to_string(d.severity), d.message.c_str());
        } else {
            std::fprintf(stderr, "[%s] [%s] %s %s\n", source, to_string(d.severity),
                d.location.to_string().c_str(), d.message.c_str());
        }
        return true;
    }

    // Has to be mutable since the do_log function has to be const
    mutable std::mutex write_mutex;
};

using parse_result = std::future<std::unique_ptr<cppast::cpp_file>>;

parse_result parse_file(cppast::libclang_compile_config const& config, 
    thread_safe_logger const& logger, fs::path path) {
    return std::async(std::launch::async, [&config, &logger, path]() -> std::unique_ptr<cppast::cpp_file> {
        cppast::libclang_parser parser(type_safe::ref(logger));
        cppast::cpp_entity_index root_index;
        try {
            return parser.parse(root_index, fs::absolute(path).generic_string(), config);
        }
        catch (std::exception const& e) {
            logger.write_raw(e.what());
            return nullptr;
        }
        catch (cppast::libclang_error const& e) {
            logger.write_raw(e.what());
            return nullptr;
        }
    });
}

int main() {

    cppast::libclang_compile_config config;
    config.set_flags(cppast::cpp_standard::cpp_1z);
    // TODO: extract include directories from cmake
    config.add_include_dir(fs::absolute(fs::path("external/phobos/external/glm")).generic_string());
    config.add_include_dir(fs::absolute(fs::path("external/phobos/include")).generic_string());
    config.add_include_dir("C:/VulkanSDK/1.2.131.1/Include");
    config.add_include_dir(fs::absolute(fs::path("external/phobos/external/fmt/include")).generic_string());
    thread_safe_logger logger;

    std::vector<parse_result> results;

    for (auto file : fs::directory_iterator("include/saturn/components/")) {
        if (file.is_regular_file() && file.path().extension() == ".hpp") {
            results.push_back(parse_file(config, logger, file.path()));
        }
    }
    

    for (auto& result : results) {
        result.wait();
        auto parsed = result.get();
        if (parsed != nullptr) {
            std::cout << parsed->name() << std::endl;
        }
    }

    return 0;
}