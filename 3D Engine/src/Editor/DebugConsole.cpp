#include "Editor/DebugConsole.hpp"

#include <fmt/format.h>

#include "Utility/Utility.hpp"

namespace Saturn::Editor {

#ifdef WITH_EDITOR

// clang-format off
std::unordered_map<DebugConsole::LogType, DebugConsole::LogTypeStyle>
    DebugConsole::log_type_styles = {
        {LogType::Debug, {"[Debug]", ImVec4(0.0f, 1.0f, 0.0f, 1.0f)}},
		{LogType::Log, {"[Log]", ImVec4(1.0f, 1.0f, 1.0f, 1.0f)}},
		{LogType::Info, {"[Info]", ImVec4(1.0f, 1.0f, 1.0f, 1.0f)}},
		{LogType::Warning, {"[Warning]", ImVec4(1.0f, 1.0f, 0.0f, 1.0f)}},
		{LogType::Error, {"[Error]", ImVec4(1.0f, 0.0f, 0.0f, 1.0f)}},
		{LogType::Raw, {"", ImVec4(1.0f, 1.0f, 1.0f, 1.0f)}}
};
// clang-format on

DebugConsole::DebugConsole() {
    clear();
    input_buffer.resize(input_buffer_size);
}

void DebugConsole::show() {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Debug Console", &shown)) {
        show_log_section();
        show_command_line_section();
    }

    ImGui::End();
}

void DebugConsole::clear() {
    if (log_entries.capacity() < max_log_entries) {
        log_entries.reserve(max_log_entries);
    }

    log_entries.clear();
}

void DebugConsole::add_entry(std::string_view entry, LogType type) {
    if (log_entries.size() >= max_log_entries) {
        log_entries.erase(log_entries.begin());
    }
    LogEntry log_entry;
    log_entry.data = entry;
    log_entry.type = type;
    log_entries.push_back(log_entry);

    if (auto_open) { shown = true; }
}

void DebugConsole::add_command(std::string command,
                               command_callback_t const& callback) {
    commands[command] = callback;
}

void DebugConsole::show_log_section() {
    if (ImGui::SmallButton("Clear")) { clear(); }
    ImGui::SameLine();
    ImGui::Checkbox("Auto-open", &auto_open);
    ImGui::Separator();

    // 1 reserve space for a separator and an InputText
    const float footer_height =
        ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

    // Pass a negative value to the vec2's y to leave room for things below
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height), false,
                      ImGuiWindowFlags_HorizontalScrollbar);

    // Tighten spacing a little bit
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
    // Show logged text
    for (auto const& log_entry : log_entries) {
        auto const& style = log_type_styles[log_entry.type];
        // Set color
        ImGui::PushStyleColor(ImGuiCol_Text, style.color);
        // Add prefix and render text
        ImGui::TextUnformatted((style.prefix + " " + log_entry.data).c_str());
        // Reset color
        ImGui::PopStyleColor();
    }

    // Scroll to bottom, #TODO: Option to disable this auto scrolling
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void DebugConsole::show_command_line_section() {
    ImGui::Separator();
    // Command-line input
    bool reclaim_focus = false;
    if (ImGui::InputText(
            "Command line", input_buffer.data(), input_buffer_size,
            ImGuiInputTextFlags_EnterReturnsTrue |
                ImGuiInputTextFlags_CallbackHistory |
                ImGuiInputTextFlags_CallbackCompletion,
            [](ImGuiInputTextCallbackData* data) {
                auto* console = reinterpret_cast<DebugConsole*>(data->UserData);
                return console->text_edit_callback(data);
            },
            reinterpret_cast<void*>(this))) {

        process_command(input_buffer);
        input_buffer.clear();
        input_buffer.resize(input_buffer_size);
        reclaim_focus = true;
    }

    // Default focus on the command line
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus) {
        // Focus command line if we just entered a command
        ImGui::SetKeyboardFocusHere(-1);
    }
}

void DebugConsole::process_command(std::string const& input) {
    const std::size_t first_space = input.find(' ');
    const std::string command = input.substr(0, first_space);
    if (auto cmd_it = commands.find(command); cmd_it != commands.end()) {
        // first_space + 1 so it doesn't include the space before the arguments
        std::string args = input.substr(first_space + 1);
        CommandContext context;
        context.command = command;
        context.args = split(args, ' ');
        cmd_it->second(context);
    }
}

int DebugConsole::text_edit_callback(ImGuiInputTextCallbackData* data) {
    switch (data->EventFlag) {
        case ImGuiInputTextFlags_CallbackCompletion: do_autocomplete(data);
    }
    return 0;
}

void DebugConsole::do_autocomplete(ImGuiInputTextCallbackData* data) {
    // Get pointers to start and end of word we are autocompleting. This snippet
    // was taken from the imgui example console app in imgui_demo.cpp
    const char* word_end = data->Buf + data->CursorPos;
    const char* word_start = word_end;
    while (word_start > data->Buf) {
        const char c = word_start[-1];
        if (c == ' ' || c == '\t' || c == ',' || c == ';') break;
        word_start--;
    }

    auto complete_to = [&data, &word_end,
                        &word_start](std::string const& new_data) {
        // Clear and insert so it inserts the command with correct
        // capitalization
        data->DeleteChars((int)(word_start - data->Buf),
                          (int)(word_end - word_start));
        data->InsertChars(data->CursorPos, new_data.c_str());
    };

    // First, we need to build a list of autocomplete candidates
    auto candidates = build_autocomplete_candidates(data);
    // If there is only a single candidate, complete it immediately
    if (candidates.empty()) {
        add_entry("No matches found", LogType::Raw);
    } else if (candidates.size() == 1) {
        complete_to(candidates[0]);
        // Also add a space
        data->InsertChars(data->CursorPos, " ");
    } else if (candidates.size() > 1) {
        // Complete as much as possible
        std::size_t match_len = 0;
        std::size_t word_size = word_end - word_start;

        if (auto match = find_exact_match(std::string(word_start), candidates);
            match != candidates.end()) {
            complete_to(*match);
            add_entry(fmt::format("Possible match: {}", *match), LogType::Raw);
            candidates.erase(match);
        }
        // Only continue looking if there are no exact matches
        char next = candidates[0][word_size];
        // If all match the next character
        while (all_satisfy_condition(
            candidates.begin(), candidates.end(),
            [&next, &match_len, &word_size](std::string const& candidate) {
                return candidate[match_len + word_size] == next;
            })) {
            ++match_len;
            // Check for exact matches
            if (auto match = find_exact_match(
                    std::string(word_start, match_len), candidates);
                match != candidates.end()) {
                complete_to(*match);
                break;
            }
        }
        // Print all candidates to the console
        for (auto const& candidate : candidates) {
            add_entry(fmt::format("Possible match: {}", candidate),
                      LogType::Raw);
        }
    }
}

std::vector<std::string>
DebugConsole::build_autocomplete_candidates(ImGuiInputTextCallbackData* data) {
    std::vector<std::string> candidates;

    std::string input(data->Buf);

    if (input.empty()) return candidates;

    static auto case_insensitive_compare = [](char a, char b) {
        return tolower(a) == tolower(b);
    };

    for (auto const& [command, _] : commands) {
        if (string_match_n(input, command, input.size(),
                           case_insensitive_compare)) {
            candidates.push_back(command);
        }
    }

    return candidates;
}

std::vector<std::string>::iterator
DebugConsole::find_exact_match(std::string const& candidate,
                               std::vector<std::string>& candidates) {
    return std::find(candidates.begin(), candidates.end(), candidate);
}

#endif
} // namespace Saturn::Editor
