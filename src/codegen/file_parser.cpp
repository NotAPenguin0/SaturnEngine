#include <saturn/codegen/file_parser.hpp>


namespace saturn::codegen {

namespace {

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


libclang_parse_result parse_single_file(fs::path const& path, cppast::libclang_compile_config const& config,
    thread_safe_logger const& logger) {

    return std::async(std::launch::async, [config, &logger, path]() -> std::unique_ptr<cppast::cpp_file> {
        cppast::libclang_parser parser(type_safe::ref(logger));
        cppast::cpp_entity_index root_index;
        try {
            return parser.parse(root_index, path.generic_string(), config);
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

} // [anonymous namespace]


libclang_parse_result parse_file(fs::path const& path, cppast::libclang_compile_config const& config) {
    thread_safe_logger logger;
    return parse_single_file(path, config, logger);
}

std::vector<libclang_parse_result> parse_directory(fs::path const& path, cppast::libclang_compile_config const& config) {
    std::vector<libclang_parse_result> results;
    if (!fs::is_directory(path)) {
        return results;
    }

    static thread_safe_logger logger;

    for (auto const& entry : fs::directory_iterator(path)) {
        // Only support parsing headers at the moment
        if (entry.path().extension() == ".hpp") {
            results.push_back(parse_single_file(entry.path(), config, logger));
        }
    }
    return results;
}

} // namespace saturn::codegen