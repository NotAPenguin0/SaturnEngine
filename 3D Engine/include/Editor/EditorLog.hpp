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
    get_console().add_entry(std::forward<Args>(args)...);
}

} // namespace log

} // namespace Saturn
#else
namespace log {
template<typename... Args>
inline void log(Args&&...) {}
} // namespace log
#endif

#endif
