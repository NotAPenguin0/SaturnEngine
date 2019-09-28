#include "Editor/EntityTree.hpp"

#ifdef WITH_EDITOR

#    include "Editor/EditorLog.hpp"
#    include "Editor/SelectFileDialog.hpp"
#    include "Subsystems/AssetManager/AssetManager.hpp"
#    include "Subsystems/AssetManager/Resource.hpp"
#    include "Subsystems/ECS/ComponentList.hpp"
#    include "Subsystems/ECS/Components.hpp"
#    include "Subsystems/Scene/Scene.hpp"
#    include "Subsystems/Scene/SceneObject.hpp"
#    include "Subsystems/Serialization/ComponentMetaInfo.hpp"
#    include "Utility/Color.hpp"

#    include "imgui/imgui.h"

#    include <type_traits>

namespace Saturn::Editor {

namespace impl {

const char* entity_payload_id = "p_entity";

// #TODO: extract this into meta info using a CATEGORY("some_category")
// macro
template<typename C>
std::string_view get_component_category() {
    using namespace ::Saturn::Components;
#    define CAT(comp, str_id)                                                  \
        if constexpr (std::is_same_v<C, ::Saturn::Components::comp>) {         \
            return str_id;                                                     \
        }

    CAT(BoxCollider, "Physics")
    CAT(Camera, "Camera")
    CAT(CameraZoomController, "Camera")
    CAT(ClickEffect, "User-defined")
    CAT(ColliderRenderer, "Editor-only")
    CAT(DirectionalLight, "Light")
    CAT(EditorCameraController, "Editor-only")
    CAT(FPSCameraController, "Camera")
    CAT(FreeLookController, "Camera")
    CAT(Material, "Rendering")
    CAT(MusicController, "Sound")
    CAT(Name, "Editor-only")
    CAT(OutlineRenderer, "Editor-only")
    CAT(ParticleEmitter, "Particle System")
    CAT(PointLight, "Light")
    CAT(Rigidbody, "Physics")
    CAT(Rotator, "User-defined")
    CAT(Sound, "Sound")
    CAT(SoundListener, "Sound")
    CAT(SpotLight, "Light")
    CAT(StaticMesh, "Rendering")
    CAT(Transform, "Rendering")

#    undef CAT

    return "Category-Unknown";
}

std::vector<std::string_view> get_component_categories() {
    return {"Camera",  "Editor-only", "Light", "Particle System",
            "Physics", "Rendering",   "Sound", "User-defined"};
}

struct ComponentFieldVisitor {

    // Define overloaded visitor for all types supported in
    // ComponentMetaInfo.hpp

    std::string_view field_name;

    void operator()(std::size_t* field) {
        ImGui::DragScalar(field_name.data(), ImGuiDataType_U64, field, 0.2f);
    }

    void operator()(unsigned int* field) {
        ImGui::DragScalar(field_name.data(), ImGuiDataType_U64, field, 0.2f);
    }

    void operator()(float* field) {
        ImGui::DragScalar(field_name.data(), ImGuiDataType_Float, field, 0.2f);
    }

    void operator()(int* field) {
        ImGui::DragScalar(field_name.data(), ImGuiDataType_S32, field, 0.2f);
    }

    void operator()(std::string* field) {
        static constexpr std::size_t buf_size = 128;
        field->resize(buf_size);
        ImGui::InputText(field_name.data(), field->data(), buf_size);
    }

    void operator()(glm::vec3* field) {
        ImGui::InputFloat3(field_name.data(), &field->x);
    }

    void operator()(glm::vec4* field) {
        ImGui::InputFloat4(field_name.data(), &field->x);
    }

    void operator()(glm::bvec3* field) {
        ImGui::Text("%s: ", field_name.data());
        ImGui::SameLine();
        ImGui::Checkbox("x", &field->x);
        ImGui::SameLine();
        ImGui::Checkbox("y", &field->y);
        ImGui::SameLine();
        ImGui::Checkbox("z", &field->z);
    }

    void operator()(bool* field) { ImGui::Checkbox(field_name.data(), field); }

    void operator()(color3* field) {
        ImGui::ColorEdit3((std::string(field_name) + " color").c_str(),
                          &field->x);
    }

    void operator()(color4* field) {
        ImGui::ColorEdit4((std::string(field_name) + " color").c_str(),
                          &field->x);
    }

    template<typename R>
    void operator()(Resource<R>* field) {
        ImGui::Text("%s:", field_name.data());
        ImGui::SameLine();
        ImGui::Text("%s", fs::path(field->get_path()).stem().string().c_str());
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(field->get_path().c_str());
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
        ImGui::SameLine();
        if (ImGui::SmallButton(("...##" + std::string(field_name)).c_str())) {
            SelectFileDialog dialog;
            dialog.show(SelectFileDialog::PickFiles, FileTypes<R>::types);
            fs::path result = dialog.get_result();
            if (result != "") {
                *field = AssetManager<R>::get_resource(result.string());
            }
        }
    }
};

template<typename C>
void display_component(SceneObject* entity) {
    using namespace ::Saturn::Meta;
    using namespace ::Saturn::Components;
    using ComponentMeta = ComponentsMeta<COMPONENT_LIST>;
    auto& comp = entity->get_component<C>();
    ComponentInfo const& component_meta =
        ComponentMeta::get_component_meta_info<C>();
    static constexpr int imgui_mouse_right_button = 1;

    std::string popup_name = "Component Actions##" + component_meta.name;

    if (ImGui::CollapsingHeader(component_meta.name.c_str())) {
        if (ImGui::IsItemClicked(imgui_mouse_right_button)) {
            // Show popup menu containing actions to delete this component
            ImGui::OpenPopup(popup_name.c_str());
        }

        if (ImGui::BeginPopup(popup_name.c_str())) {
            if (ImGui::Selectable("Delete component")) {
                log::log("Deleting component: {0}from entity: {1}",
                         component_meta.name,
                         entity->get_component<Name>().name);
                entity->remove_component<C>();
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                return;
            }
            ImGui::EndPopup();
        }

        for (auto const& [field_name, field_type] : component_meta.fields) {
            ComponentFieldPtr field_info =
                ComponentMeta::get_component_field(comp, field_name);
            // Check if pointer isn't null
            if (!field_info) { continue; }
            std::visit(ComponentFieldVisitor{field_name}, field_info.get());
        }
    }
}

template<typename C, typename... Cs>
void display_components(SceneObject* entity) {
    using namespace Components;
    using namespace Meta;
    using ComponentMeta = ComponentsMeta<COMPONENT_LIST>;
    ComponentInfo const& meta_info =
        ComponentMeta::get_component_meta_info<C>();
    if (!meta_info.hide_in_editor && entity->has_component<C>()) {
        // Temporary
        if constexpr (!std::is_same_v<C, ParticleEmitter>) {
            display_component<C>(entity);
        }
    };
    if constexpr (sizeof...(Cs) != 0) { display_components<Cs...>(entity); }
}

template<typename C>
void show_add_component_entry(SceneObject* entity) {
    using namespace ::Saturn::Meta;
    using ComponentMeta = ComponentsMeta<COMPONENT_LIST>;
    ComponentInfo const& meta_info =
        ComponentMeta::get_component_meta_info<C>();
    if (meta_info.hide_in_editor) { return; }
    if (ImGui::Selectable((meta_info.name + "##AddComponent").c_str())) {
        if (!entity->has_component<C>()) { entity->add_component<C>(); }
    }
}

template<typename C, typename... Cs>
void show_add_component_list(SceneObject* entity, std::string_view cat) {
    using namespace ::Saturn::Components;
    if (!std::is_same_v<C, Name> && !std::is_same_v<C, ParticleEmitter> &&
        cat == get_component_category<C>()) {
        show_add_component_entry<C>(entity);
    }

    if constexpr (sizeof...(Cs) != 0) {
        show_add_component_list<Cs...>(entity, cat);
    }
}

template<typename C>
void do_component_copy(Scene& scene, SceneObject* src, SceneObject* dest) {
    auto& new_c = scene.get_ecs().get_with_id<C>(dest->add_component<C>());
    auto id = new_c.id;
    new_c = src->get_component<C>();
    new_c.id = id;
    new_c.entity = dest;
}

template<typename C, typename... Cs>
void copy_components(Scene& scene, SceneObject* src, SceneObject* dest) {

    if (get_component_category<C>() != "Editor-only" &&
        src->has_component<C>()) {
        do_component_copy<C>(scene, src, dest);
    }

    if constexpr (sizeof...(Cs) != 0) {
        copy_components<Cs...>(scene, src, dest);
    }
}

void clone_entity(Scene& scene, SceneObject* entity) {
    using namespace Components;
    std::string new_name = "Duplicate of " + entity->get_component<Name>().name;
    auto& new_entity = scene.create_object(entity->parent());
    scene.get_ecs().get_with_id<Name>(new_entity.add_component<Name>()).name =
        std::move(new_name);
    copy_components<COMPONENT_LIST>(scene, entity, &new_entity);
}

void show_entity_actions_popup(Scene& scene,
                               SceneObject*& entity,
                               SceneObject*& selected_entity,
                               const char* popup_name) {
    using namespace Components;
    if (ImGui::BeginPopup(popup_name)) {
        if (ImGui::MenuItem("Delete entity")) {
            log::log("Deleting entity: {}", entity->get_component<Name>().name);
            scene.destroy_object(selected_entity);
            selected_entity = nullptr;
            entity = nullptr;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::MenuItem("Duplicate entity")) {
            clone_entity(scene, selected_entity);
        }
        if (ImGui::BeginMenu("Add Component")) {
            for (auto cat : impl::get_component_categories()) {
                if (ImGui::BeginMenu(cat.data())) {
                    impl::show_add_component_list<COMPONENT_LIST>(entity, cat);
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

} // namespace impl

EntityTree::EntityTree() {
    // Show by default
    shown = true;
}

SceneObject* EntityTree::get_selected_entity() const { return selected_entity; }
void EntityTree::reset_selected_entity() {
    if (selected_entity) { on_entity_deselect(selected_entity); }
    selected_entity = nullptr;
}

void EntityTree::show(Scene& scene) {
    if (ImGui::Begin("Entity Tree", get_shown_pointer())) {
        auto tree = build_entity_tree(scene);
        // Start columns section
        ImGui::Columns(2, "EntityTree", true);
        show_entity_tree(tree, scene);
        ImGui::NextColumn();
        if (selected_entity != nullptr) {
            show_entity_details(selected_entity, scene);
        }
        // End column section
        ImGui::Columns();
    }

    ImGui::End();
}

EntityTree::tree_t EntityTree::build_entity_tree(Scene& scene) {
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

void EntityTree::show_entity_tree(tree_t& enttree, Scene& scene) {
    std::size_t i = 0;
    for (auto entity_it = enttree.begin(); entity_it != enttree.end();
         ++entity_it) {
        auto entity = *entity_it;

        // Only display if this is a root entity
        if (entity->get_parent_id() == 0) {
            entity_it = show_self_and_children(scene, enttree, entity_it);
        }

        if (entity_it == enttree.end()) break;
    }
    ImGui::Selectable("##MakeRootEntity");
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload =
                ImGui::AcceptDragDropPayload(impl::entity_payload_id)) {

            assert(payload->DataSize == sizeof(SceneObject**));
            // Set the parent of the drag-drop payload to the current entity
            SceneObject* new_child =
                *reinterpret_cast<SceneObject**>(payload->Data);
            new_child->set_parent(nullptr);
            new_child->set_parent_id(0);
        }
        ImGui::EndDragDropTarget();
    }
}

bool EntityTree::has_child(tree_t& tree, tree_t::iterator entity) {
    return entity != tree.end() - 1 &&
           (*entity)->get_id() == (*(entity + 1))->get_parent_id();
}

EntityTree::tree_t::iterator EntityTree::show_self_and_children(
    Scene& scene, tree_t& tree, tree_t::iterator entity) {
    using namespace ::Saturn::Components;
    std::string to_display = "";
    if (SceneObject* obj = *entity; obj->has_component<Name>()) {
        auto& name_component = obj->get_component<Name>();
        to_display = name_component.name;
    } else {
        to_display =
            "Unknown entity with ID " + std::to_string((*entity)->get_id());
    }

    if ((*entity)->has_component<EditorCameraController>()) { return entity; }

    auto cur = entity;

    bool is_leaf = false;
    ImGuiTreeNodeFlags node_flags =
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (!has_child(tree, entity)) {
        // If the node has no children, give it the 'leaf' flags so you
        // cannot see the expand arrow next to it
        node_flags |=
            ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        is_leaf = true;
    }

    if (*entity == selected_entity) {
        node_flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::TreeNodeEx(to_display.c_str(), node_flags)) {

        // Our entity is a drag-drop source and a drag-drop target
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload(impl::entity_payload_id, &*entity,
                                      sizeof(*entity));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload =
                    ImGui::AcceptDragDropPayload(impl::entity_payload_id)) {

                assert(payload->DataSize == sizeof(&*entity));
                // Set the parent of the drag-drop payload to the current
                // entity
                SceneObject* new_child =
                    *reinterpret_cast<SceneObject**>(payload->Data);
                new_child->set_parent(*entity);
                new_child->set_parent_id((*entity)->get_id());
            }
            ImGui::EndDragDropTarget();
        }

        if (ImGui::IsItemClicked()) {
            if (selected_entity) { on_entity_deselect(selected_entity); }
            selected_entity = *entity;
            on_entity_select(scene, selected_entity);
        }
        // 1 is right click
        std::string popup_name = "Entity actions##" + to_display;
        if (ImGui::IsItemClicked(1)) {
            if (selected_entity) { on_entity_deselect(selected_entity); }
            selected_entity = *entity;
            on_entity_select(scene, selected_entity);
            ImGui::OpenPopup(popup_name.c_str());
        }
        impl::show_entity_actions_popup(scene, *entity, selected_entity,
                                        popup_name.c_str());
        ++cur;
        // For each next entity, show it if it's parent is the current
        // entity.
        while (cur != tree.end()) {
            const std::size_t parent = (*cur)->get_parent_id();
            // If the parent is the current entity's ID, show it and its
            // children.
            if (parent == (*entity)->get_id()) {
                cur = show_self_and_children(scene, tree, cur);
            } else {
                // If the parent is not the current entity, we are in the
                // next section of our tree, so break out of the loop. We
                // have to increment until we encounter the first element
                // that is either a root entity or has the current entity as
                // parent, and then decrement once so it is next in the loop

                auto depth = [](SceneObject* s) -> int {
                    int d = 0;
                    auto parent = s->parent();
                    while (parent) {
                        ++d;
                        parent = parent->parent();
                    }
                    return d;
                };
                /*
                 auto prev = cur;
                 while (cur != tree.end()) {
                     prev = cur;
                     ++cur;
                     if (cur == tree.end()) break;
                     auto dcur = depth(*cur), dprev = depth(*prev);
                     if (dcur == 0 || dprev == 0 || dcur >= dprev) {
                         // Should not have incremented
                         --cur;
                         break;
                     }
                 }*/

                auto is_child_of_entity = [entity](SceneObject* s) -> bool {
                    auto parent = s;
                    while (parent) {
                        if (s->get_parent_id() == (*entity)->get_id()) {
                            return true;
                        }
                        parent = parent->parent();
                    }
                    return false;
                };

                while (cur != tree.end() && is_child_of_entity(*cur)) { ++cur; }

                --cur;
                break;
            }
            // Move to next entity
            if (cur != tree.end()) { ++cur; }
        }
        if (!is_leaf) { ImGui::TreePop(); }
    }
    return cur;
}

void EntityTree::show_entity_details(SceneObject* entity, Scene& scene) {
    using namespace ::Saturn::Components;
    static constexpr std::size_t name_buffer_size = 256;
    std::string& name = entity->get_component<Name>().name;
    name.resize(name_buffer_size);
    ImGui::InputText("Entity name", name.data(), name_buffer_size);
    if (entity) { impl::display_components<COMPONENT_LIST>(entity); }
}

void EntityTree::on_entity_select(Scene& scene, SceneObject* entity) {
    using namespace Components;
    auto& outline = scene.get_ecs().get_with_id<OutlineRenderer>(
        entity->add_component<OutlineRenderer>());
    // Set the color to an orange-like color
    outline.color = color3(0.92, 0.67, 0.2);
    entity->add_component<ColliderRenderer>();
}

void EntityTree::on_entity_deselect(SceneObject* entity) {
    using namespace Components;
    entity->remove_component<OutlineRenderer>();
    entity->remove_component<ColliderRenderer>();
}

} // namespace Saturn::Editor

#endif
