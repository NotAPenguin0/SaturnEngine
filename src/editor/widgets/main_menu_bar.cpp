#include <editor/widgets/main_menu_bar.hpp>
#include <editor/widgets/file_dialog.hpp>

#include <saturn/assets/assets.hpp>

#include <imgui/imgui.h>

namespace editor {

static void show_dev_menu(saturn::FrameContext& ctx) {
    if (ImGui::MenuItem("Import model", nullptr, nullptr)) {

        FileDialog dialog;

        stl::vector<FileType> file_types;
        file_types.push_back(FileType{ L"Wavefront object file (*.obj)", L"*.obj" });
        dialog.show(FileDialog::PickFiles, fs::current_path(), file_types);

        fs::path path = fs::relative(dialog.get_result(), fs::current_path());

        if (!path.empty()) {
            saturn::Context load_context { ctx.vulkan, &ctx.scene };
            saturn::assets::load_model(load_context, path);
        }
    }
}

void show_main_menu_bar(saturn::FrameContext& ctx) {
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("dev menu##menu_bar")) {
            show_dev_menu(ctx);

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}   

}