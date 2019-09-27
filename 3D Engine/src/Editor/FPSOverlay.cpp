#include "Editor/FPSOverlay.hpp"

#include "Subsystems/Time/Time.hpp"
#include "imgui/imgui.h"

#include <numeric>
#include <string>

#ifdef WITH_EDITOR

namespace Saturn::Editor {

void FPSOverlay::show() {
    ImGui::SetNextWindowPos(ImVec2(15, 25));
    ImGui::SetNextWindowBgAlpha(0.25f);
    if (ImGui::Begin(
            "FPS##Overlay", &shown,
            ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {
        // Update frame times
        if (frame_times.size() > num_frames) {
            sum -= frame_times.front();
            frame_times.erase(frame_times.begin());
            // If there are still too many frames recorded, start removing at
            // the end
            if (frame_times.size() > num_frames) {
                frame_times.erase(frame_times.begin() + num_frames,
                                  frame_times.end());
                sum =
                    std::accumulate(frame_times.begin(), frame_times.end(), 0);
            }
        }
        const float fps = 1.0f / Time::deltaTime;
        frame_times.push_back(fps);
        sum += fps;

		// Truncate average fps
        int average = sum / frame_times.size();

        std::string overlay = "average: " + std::to_string(average) + " fps";

        ImGui::DragScalar("No. of frames", ImGuiDataType_U64, &num_frames,
                          0.4f);

        ImGui::PlotLines("##FPSPlot", frame_times.data(), frame_times.size(), 0,
                         overlay.c_str(), 0, 2 * average, ImVec2(300, 80));
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(
                "FPS over specified number of frames. The middle of the graph "
                "is also the average fps");
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }

        ImGui::End();
    }
}

} // namespace Saturn::Editor

#endif
