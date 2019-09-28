#ifndef SATURN_EDITOR_LOG_HPP_
#define SATURN_EDITOR_LOG_HPP_

#ifdef WITH_EDITOR

#    include "DebugConsole.hpp"
#    include <fmt/format.h>
#    include <functional>

namespace Saturn {

namespace log {
inline Editor::DebugConsole& get_console() {
    static Editor::DebugConsole console;
    return console;
}

template<typename... Args>
void log(Args&&... args) {
    get_console().add_entry(fmt::format(std::forward<Args>(args)...));
}

template<typename... Args>
void warn(Args&&... args) {
    get_console().add_entry(fmt::format(std::forward<Args>(args)...),
                            Editor::DebugConsole::Warning);
}
template<typename... Args>
void error(Args&&... args) {
    get_console().add_entry(fmt::format(std::forward<Args>(args)...),
                            Editor::DebugConsole::Error);
}

} // namespace log
} // namespace Saturn
#else
namespace log {
template<typename... Args>
inline void log(Args&&...) {}
template<typename... Args>
inline void warn(Args&&...) {}
template<typename... Args>
void error(Args&&... args) {}
} // namespace log
#endif

#endif
