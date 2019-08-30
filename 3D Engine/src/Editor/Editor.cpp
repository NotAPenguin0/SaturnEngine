#ifdef WITH_EDITOR

#    include "Editor/Editor.hpp"
#    include "Core/Application.hpp"
#    include "Subsystems/ECS/Components.hpp"
#    include "Subsystems/Input/Input.hpp"
#    include "Subsystems/Scene/Scene.hpp"
#    include "Subsystems/Scene/SceneObject.hpp"
#    include "imgui/imgui.h"
#    include "imgui/imgui_impl_glfw.h"
#    include "imgui/imgui_impl_opengl3.h"

namespace Saturn::Editor {

Editor::Editor(Application& app) : app(&app) {
    this->app->set_editor_instance(this);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->app->window_handle, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void Editor::setup_viewports() {
    // Setup viewport for scene view
    auto main_cam = this->app->get_renderer()->get_viewport(0).get_camera();
    auto scene_view = Viewport(main_cam, 0, 0, 800, 600);
    scene_view_viewport_id =
        this->app->get_renderer()->add_viewport(scene_view);

    auto& main_view = this->app->get_renderer()->get_viewport(0);
    main_view.set_camera(-1);
}

void Editor::render(Scene& scene) {
    // #TODO: camera controls in editor
    // #TODO: specify which ECS systems run in editor and which don't
    // Enable mouse capture when space is pressed
    Input::set_mouse_capture(RawInput::get_key(Key::Space).down);
    ImGui::GetIO().WantCaptureMouse = false;

    static bool show_demo = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //    Viewport::set_active(app->renderer->get_viewport(main_vp_id));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0, 0, 0, 0);
    glViewport(0, 0, app->window_dimensions.x, app->window_dimensions.y);
    glClear(GL_COLOR_BUFFER_BIT);
    if (show_demo) { ImGui::ShowDemoWindow(&show_demo); }

    show_scene_tree(scene);
    show_menu_bar();

    ImGui::Render();
    Viewport::set_active(
        app->get_renderer()->get_viewport(scene_view_viewport_id));
    glViewport(0, 0, 800, 600);
}

void Editor::show_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Nothing here yet");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Nothing here yet");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Entity Tree", nullptr, &show_widgets.entity_tree);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Nothing here yet");
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

static void show_entity(SceneObject& entity) {
    ImGui::Text("An entity with id: %i", entity.get_id());
    ImGui::Text("Parent id: %i", entity.get_parent_id());
}

static std::vector<SceneObject*>::iterator
show_self_and_children(std::vector<SceneObject*>& enttree,
                       std::vector<SceneObject*>::iterator entity) {
    using namespace ::Saturn::Components;
    std::string to_display = "";
    if (SceneObject* obj = *entity; obj->has_component<Name>()) {
        Name& name_component = obj->get_component<Name>();
        to_display = name_component.name;
    } else {
        to_display =
            "Unknown entity with ID " + std::to_string((*entity)->get_id());
    }
    auto cur = entity;
    if (ImGui::TreeNode(to_display.c_str())) {
        show_entity(**entity);
        ++cur;
        // For each next entity, show it if it's parent is the current
        // entity.
        while (cur != enttree.end()) {
            const std::size_t parent = (*cur)->get_parent_id();
            // If the parent is the current entity's ID, show it and its
            // children.
            if (parent == (*entity)->get_id()) {
                cur = show_self_and_children(enttree, cur);
            } else {
                // If the parent is not the current entity, we are in the
                // next section of our tree, so break out of the loop We
                // first decrement cur because it will be incremented in the
                // next iteration of the loop
                --cur;
                break;
            }
            // Move to next entity
            if (cur != enttree.end()) { ++cur; }
        }

        ImGui::TreePop();
    }
    return cur;
}

void Editor::show_scene_tree(Scene& scene) {
    if (show_widgets.entity_tree) {
        auto enttree = build_entity_tree(scene);
        ImGui::Begin("Entity Tree", &show_widgets.entity_tree);
        std::size_t i = 0;
        for (auto entity_it = enttree.begin(); entity_it != enttree.end();
             ++entity_it) {
            auto entity = *entity_it;

            // Only display if this is a root entity
            if (entity->get_parent_id() == 0) {
                entity_it = show_self_and_children(enttree, entity_it);
            }

            // #TODO: Use ImGui::CollapsingHeader() to display components
            if (entity_it == enttree.end()) break;
        }

        ImGui::End();
    }
}

void Editor::frame_end() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::vector<SceneObject*> Editor::build_entity_tree(Scene& scene) {
    //#Optimize Maybe
    std::vector<SceneObject*> tree;
    auto& objects = scene.objects;
    tree.reserve(objects.size());
    std::vector<bool> done(objects.size());

    auto all_done = [&done]() {
        for (auto v : done) {
            if (!v) return false;
        }
        return true;
    };

    while (!all_done()) {
        for (std::size_t i = 0; i < objects.size(); ++i) {
            auto& obj = objects[i];
            if (!done[i]) {
                if (!obj->has_parent()) {
                    tree.push_back(obj.get());
                    done[i] = true;
                } else {
                    SceneObject* parent = obj->parent();
                    if (auto it = std::find(tree.begin(), tree.end(), parent);
                        it != tree.end()) {
                        // Insert after the parent
                        tree.insert(it + 1, obj.get());
                        done[i] = true;
                    }
                }
            }
        }
    }
    return tree;
}

} // namespace Saturn::Editor

#endif
