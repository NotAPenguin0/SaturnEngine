#ifndef MVG_LOG_SYSTEM_HPP_
#define MVG_LOG_SYSTEM_HPP_

#include <functional>
#include <iostream>
#include <string>
#include <string_view>

#include "rang.hpp"

namespace Saturn {

class LogSystem {
public:
    enum class Severity { Info, Warning, Error, FatalError };

    static void initialize(std::ostream& target);

    template<typename T>
    static void write_raw(T const& data) {
        target_stream.get() << data;
    }

    template<typename T>
    static void write_colored(T const& data, rang::fg color = rang::fg::reset) {
        target_stream.get() << color;
        write_raw(str);
        target_stream.get() << rang::fg::reset;
    }

    template<typename T>
    static void write(Severity sev, T const& data) {
        rang::fg color;
        std::string prefix;
        switch (sev) {
            case Severity::Info:
                prefix = "[INFO]: ";
                color = rang::fg::reset;
                break;
            case Severity::Warning:
                prefix = "[WARNING]: ";
                color = rang::fg::yellow;
                break;
            case Severity::Error:
                prefix = "[ERROR]: ";
                color = rang::fg::red;
                break;
            case Severity::FatalError:
                prefix = "[FATAL ERORR]: " color = rang::fg::red;
                break;
        }
        write_colored(prefix, color);
        write_colored(data, color);
        write_colored("\n", color);
    }

private:
    static inline std::reference_wrapper<std::ostream> target_stream;
};

} // namespace Saturn

#endif
