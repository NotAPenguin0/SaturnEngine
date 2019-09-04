#include "Editor/DebugConsole.hpp"

#include <fmt/format.h>

namespace Saturn::Editor {

#ifdef WITH_EDITOR

// clang-format off
std::unordered_map<DebugConsole::LogType, DebugConsole::LogTypeStyle>
    DebugConsole::log_type_styles = {
        {LogType::Debug, {"[Debug]", ImVec4(0.0f, 1.0f, 0.0f, 1.0f)}},
		{LogType::Log, {"[Log]", ImVec4(1.0f, 1.0f, 1.0f, 1.0f)}},
		{LogType::Warning, {"[Warning]", ImVec4(1.0f, 1.0f, 0.0f, 1.0f)}},
		{LogType::Error, {"[Error]", ImVec4(1.0f, 0.0f, 0.0f, 1.0f)}}
};
// clang-format on

DebugConsole::DebugConsole() {
    clear();
    input_buffer.resize(input_buffer_size);
}

void DebugConsole::show() {
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Debug Console", &shown)) {
        if (ImGui::SmallButton("Clear")) { clear(); }
        ImGui::SameLine();
        ImGui::Checkbox("Auto-open", &auto_open);
        ImGui::Separator();

		// 1 reserve space for a separator and an InputText
        const float footer_height = ImGui::GetStyle().ItemSpacing.y +
                                    ImGui::GetFrameHeightWithSpacing();

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
            ImGui::TextUnformatted(
                (style.prefix + " " + log_entry.data).c_str());
            // Reset color
            ImGui::PopStyleColor();
        }

        // Scroll to bottom, #TODO: Option to disable this auto scrolling
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();

		ImGui::Separator();
		// Command-line input
        bool reclaim_focus = false;
		if (ImGui::InputText("Command line", input_buffer.data(),
			input_buffer_size,
			ImGuiInputTextFlags_EnterReturnsTrue)) {
            add_entry(input_buffer);
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

#endif
} // namespace Saturn::Editor
