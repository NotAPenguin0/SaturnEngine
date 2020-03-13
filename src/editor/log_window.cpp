#include <editor/log_window.hpp>

#include <imgui/imgui.h>
#include <iostream>

LogWindow::LogWindow() {
    messages.reserve(stl::tags::uninitialized, max_messages);
}

void LogWindow::show_gui() {
    if (ImGui::Begin("Log", &shown)) {
        ImGui::Checkbox("Auto-scroll", &auto_scroll);
        ImGui::SameLine();
        if (ImGui::SmallButton("Clear")) {
            messages.clear();
        }

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.22f, 0.22f, 0.22f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.22f, 0.22f, 0.22f, 1.0f));
        ImGui::BeginChild("##log_scroll_region");

        // Display messages
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
        for (auto const& message : messages) {
            ImGui::TextWrapped("%s", message.c_str());
        }
        ImGui::PopStyleVar();

        // Scroll to bottom. Only enable this if auto scroll is enabled
        if (auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::PopStyleColor();
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void LogWindow::write(ph::log::Severity severity, std::string_view str) {
    
    if (messages.size() >= max_messages) {
        messages.erase(messages.begin());
    }

    messages.emplace_back(str);
    std::cerr << str << std::endl;
}