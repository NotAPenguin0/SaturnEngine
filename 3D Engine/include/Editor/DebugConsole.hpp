#ifndef SATURN_DEBUG_CONSOLE_HPP_
#define SATURN_DEBUG_CONSOLE_HPP_

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "imgui/imgui.h"

#include "EditorWidget.hpp"

namespace Saturn::Editor {

#ifdef WITH_EDITOR

class DebugConsole : public EditorWidget {
public:
    enum LogType { Debug, Log, Warning, Error };

    struct LogEntry {
        std::string data;
        LogType type;
    };

    struct LogTypeStyle {
        std::string prefix;
        ImVec4 color;
    };

    DebugConsole();

    void show();
    void clear();
    void add_entry(std::string_view entry, LogType type = LogType::Log);

private:
    // Specifies whether the console will automatically open when an entry is
    // added
    bool auto_open = true;

    std::vector<LogEntry> log_entries;
    std::string input_buffer;
    // When this capacity is hit, the first log entry will be removed when
    // adding a new one
    static constexpr std::size_t max_log_entries = 999;
    static constexpr std::size_t input_buffer_size = 256;
    static std::unordered_map<LogType, LogTypeStyle> log_type_styles;
};

#endif

} // namespace Saturn::Editor

#endif
