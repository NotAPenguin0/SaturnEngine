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
    static void write_colored(T const& data, rang::fgB color = rang::fgB::gray) {
        target_stream.get() << color;
        write_raw(data);
        target_stream.get() << rang::fgB::gray;
    }

    template<typename T>
    static void write(Severity sev, T const& data) {
        rang::fgB color;
        std::string prefix;
        switch (sev) {
            case Severity::Info:
                prefix = "[INFO]: ";
                color = rang::fgB::gray;
                break;
            case Severity::Warning:
                prefix = "[WARNING]: ";
                color = rang::fgB::yellow;
                break;
            case Severity::Error:
                prefix = "[ERROR]: ";
                color = rang::fgB::red;
                break;
            case Severity::FatalError:
                prefix = "[FATAL ERORR]: ";
                color = rang::fgB::red;
                break;
        }
        write_colored(prefix, color);
        write_colored(data, color);
        write_colored("\n", color);
    }

private:
    // default to std::cout
    static inline std::reference_wrapper<std::ostream> target_stream =
        std::cout;
};

} // namespace Saturn

#endif
