#ifndef SATURN_DEBUG_CONSOLE_HPP_
#define SATURN_DEBUG_CONSOLE_HPP_

#include "imgui/imgui.h"
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "EditorWidget.hpp"

namespace Saturn::Editor {

#ifdef WITH_EDITOR

class DebugConsole : public EditorWidget {
public:
    enum LogType { Raw, Debug, Log, Warning, Error };

    struct LogEntry {
        std::string data;
        LogType type;
    };

    struct LogTypeStyle {
        std::string prefix;
        ImVec4 color;
    };

    struct CommandContext {
        std::string command;
        std::vector<std::string> args;
    };

    using command_callback_t = std::function<void(CommandContext const&)>;

    DebugConsole();

    void show();
    void clear();
    void add_entry(std::string_view entry, LogType type = LogType::Log);

    void add_command(std::string command, command_callback_t const& callback);

private:
    void show_log_section();
    void show_command_line_section();
    void process_command(std::string const& input);

    int text_edit_callback(ImGuiInputTextCallbackData* data);
    void do_autocomplete(ImGuiInputTextCallbackData* data);
    std::vector<std::string>
    build_autocomplete_candidates(ImGuiInputTextCallbackData* data);
    std::vector<std::string>::iterator
    find_exact_match(std::string const& candidate,
                     std::vector<std::string>& candidates);

    // Specifies whether the console will automatically open when an entry
    // is added
    bool auto_open = true;

    std::vector<LogEntry> log_entries;
    std::string input_buffer;
    std::unordered_map<std::string, command_callback_t> commands;
    // When this capacity is hit, the first log entry will be removed when
    // adding a new one
    static constexpr std::size_t max_log_entries = 999;
    static constexpr std::size_t input_buffer_size = 256;
    static std::unordered_map<LogType, LogTypeStyle> log_type_styles;
};

#endif

} // namespace Saturn::Editor

#endif
