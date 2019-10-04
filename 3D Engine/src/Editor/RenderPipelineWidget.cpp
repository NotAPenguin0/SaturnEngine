#ifdef WITH_EDITOR

#    include "Editor/RenderPipelineWidget.hpp"

#    include "Core/Application.hpp"
#    include "Editor/EditorLog.hpp"
#    include "imgui/imgui.h"

#    include "Subsystems/Renderer/Modules/BlitPass.hpp"
#    include "Subsystems/Renderer/Modules/DebugModule.hpp"
#    include "Subsystems/Renderer/Modules/DepthMapPass.hpp"
#    include "Subsystems/Renderer/Modules/EditorModule.hpp"
#    include "Subsystems/Renderer/Modules/MeshRenderModule.hpp"
#    include "Subsystems/Renderer/Modules/ParticleModule.hpp"
#    include "Subsystems/Renderer/Modules/TransferModule.hpp"

namespace Saturn::Editor {

using namespace RenderModules;

template<typename V>
static void
add_render_stage(V const& v, Application* app, std::string_view stage) {
    // Make sure not to add any duplicates
    if (std::find_if(v.begin(), v.end(), [stage](auto& s) {
            return s->str_id() == stage;
        }) != v.end()) {
        return;
    }
    if (stage == "DepthMapPass") {
        app->get_renderer()->add_pre_render_stage(
            std::make_unique<DepthMapPass>());
    } else if (stage == "DebugModule") {
        app->get_renderer()->add_render_module(std::make_unique<DebugModule>());
    } else if (stage == "EditorModule") {
        app->get_renderer()->add_render_module(
            std::make_unique<EditorModule>());
    } else if (stage == "MeshRenderModule") {
        app->get_renderer()->add_render_module(
            std::make_unique<MeshRenderModule>());
    } else if (stage == "ParticleModule") {
        app->get_renderer()->add_render_module(
            std::make_unique<ParticleModule>());
    } else if (stage == "TransferModule") {
        app->get_renderer()->add_render_module(
            std::make_unique<TransferModule>());
    } else if (stage == "BlitPass") {
        app->get_renderer()->add_post_render_stage(
            std::make_unique<BlitPass>());
    }
}

void RenderPipelineWidget::show(Application& app) {
//    ImGui::Begin("Render pipeline", &shown);

    ImGui::Columns(2);

    list_render_stages(app);

    // Add stage
    bool open_add_render_stage_popup = false;
    if (ImGui::Button("Add##RenderStageWidget")) {
        open_add_render_stage_popup = true;
    }

    if (open_add_render_stage_popup) {
        ImGui::OpenPopup("Add render stage##PipelineWidget");
    }

    show_add_stage_popup(app);

    ImGui::NextColumn();
    display_stage_info(app);
    ImGui::Columns();

//    ImGui::End();
}

void RenderPipelineWidget::add_stage(Application& app,
                                     std::string_view type,
                                     std::string_view stage) {
    if (type == "PreRenderStage") {
        add_render_stage(app.get_renderer()->get_pre_render_stages(), &app,
                         stage);
    } else if (type == "PostRenderStage") {
        add_render_stage(app.get_renderer()->get_post_render_stages(), &app,
                         stage);
    } else if (type == "RenderModule") {
        add_render_stage(app.get_renderer()->get_render_modules(), &app, stage);
    } else {
        log::warn("Unknown render stage type {}", type);
    }
}

void RenderPipelineWidget::list_render_stages(Application& app) {

    ImGui::Text("Pre-Render stages:");
    for (auto& stage : app.get_renderer()->get_pre_render_stages()) {
        if (ImGui::Selectable(stage->str_id().data())) {
            selected = stage.get();
        }
    }
    ImGui::Text("");
    ImGui::Text("Render modules: ");
    for (auto& stage : app.get_renderer()->get_render_modules()) {
        if (ImGui::Selectable(stage->str_id().data())) {
            selected = stage.get();
        }
    }
    ImGui::Text("");
    ImGui::Text("Post-Render stages: ");
    for (auto& stage : app.get_renderer()->get_post_render_stages()) {
        if (ImGui::Selectable(stage->str_id().data())) {
            selected = stage.get();
        }
    }
    ImGui::Text("");
}

void RenderPipelineWidget::show_add_stage_popup(Application& app) {
    if (ImGui::BeginPopupModal("Add render stage##PipelineWidget", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize)) {

        static int stage_type = -1;
        ImGui::Combo("Stage type ", &stage_type,
                     "PreRenderStage\0RenderModule\0PostRenderStage\0");

        static const char* pre_stages[] = {"DepthMapPass"};
        static const char* modules[] = {"DebugModule", "EditorModule",
                                        "MeshRenderModule", "ParticleModule",
                                        "TransferModule"};
        static const char* post_stages[] = {"BlitPass"};

        static int stage_index = -1;
        switch (stage_type) {
            case 0:
                ImGui::Combo("Stage ", &stage_index, pre_stages,
                             sizeof(pre_stages) / sizeof(const char*));
                if (ImGui::Button("Add##RenderStage")) {
                    if (stage_index >= 0) {
                        add_render_stage(
                            app.get_renderer()->get_pre_render_stages(), &app,
                            pre_stages[stage_index]);
                        ProjectFile::render_stage_added(
                            "PreRenderStage", pre_stages[stage_index]);
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel##RenderStage")) {
                    ImGui::CloseCurrentPopup();
                }
                break;
            case 1:
                ImGui::Combo("Module ", &stage_index, modules,
                             sizeof(modules) / sizeof(const char*));
                if (ImGui::Button("Add##RenderStage")) {
                    if (stage_index >= 0) {
                        add_render_stage(
                            app.get_renderer()->get_render_modules(), &app,
                            modules[stage_index]);
                        ProjectFile::render_stage_added("RenderModule",
                                                        modules[stage_index]);
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel##RenderStage")) {
                    ImGui::CloseCurrentPopup();
                }
                break;
            case 2:
                ImGui::Combo("Post-Render Stage ", &stage_index, post_stages,
                             sizeof(post_stages) / sizeof(const char*));
                if (ImGui::Button("Add##RenderStage")) {

                    if (stage_index >= 0) {
                        add_render_stage(
                            app.get_renderer()->get_post_render_stages(), &app,
                            post_stages[stage_index]);
                        ProjectFile::render_stage_added(
                            "PostRenderStage", post_stages[stage_index]);
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel##RenderStage")) {
                    ImGui::CloseCurrentPopup();
                }
                break;
            default:
                if (ImGui::Button("Cancel##RenderStage")) {
                    ImGui::CloseCurrentPopup();
                }
        }

        ImGui::EndPopup();
    }
}

void RenderPipelineWidget::display_stage_info(Application& app) {
    if (!selected) { return; }

    ImGui::Text("%s", selected->str_id().data());

    ImGui::TextWrapped("%s", selected->description().data());

    if (ImGui::Button("Remove stage")) {
        static auto remove_stage = [this](auto& vec) -> bool {
            auto it = std::find_if(vec.begin(), vec.end(), [this](auto& elem) {
                return elem->str_id() == selected->str_id();
            });
            if (it == vec.end()) return false;
            vec.erase(it);

            return true;
        };

        auto str_id = selected->str_id();

        if (remove_stage(app.get_renderer()->get_pre_render_stages())) {
            ProjectFile::render_stage_removed("PreRenderStage", str_id);
            selected = nullptr;

            return;
        }
        if (remove_stage(app.get_renderer()->get_render_modules())) {
            ProjectFile::render_stage_removed("RenderModule", str_id);
            selected = nullptr;
            return;
        }
        if (remove_stage(app.get_renderer()->get_post_render_stages())) {
            ProjectFile::render_stage_removed("PostRenderStage", str_id);
            selected = nullptr;
            return;
        }
    }
}

} // namespace Saturn::Editor

#endif
