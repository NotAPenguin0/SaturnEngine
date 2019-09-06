#include "Editor/EntityTree.hpp"

#ifdef WITH_EDITOR

#    include "Editor/EditorLog.hpp"
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

struct ComponentFieldVisitor {
    // Define overloaded visitor for types:
    // std::size_t, float, int, std::string, glm::vec3, glm::vec4, bool

    std::string_view field_name;

    void operator()(std::size_t* field) {
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

    if (ImGui::CollapsingHeader(component_meta.name.c_str())) {
        if (ImGui::IsItemClicked(imgui_mouse_right_button)) {
            // Show popup menu containing actions to delete this component
            if (!ImGui::IsPopupOpen("Component Actions")) {
                ImGui::OpenPopup("Component Actions");
            }
        }

        if (ImGui::BeginPopup("Component Actions")) {
            if (ImGui::Selectable("Delete component")) {
                log::log(fmt::format("Deleting component: {0} from entity: {1}",
                                     component_meta.name,
                                     entity->get_component<Name>().name));
                entity->remove_component<C>();
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                return;
            }
            ImGui::EndPopup();
        }

        for (auto const& [field_name, field_type] : component_meta.fields) {
            if (field_type.find("Resource") != std::string::npos) {
                continue; // unsupported for now, this will be added once we
                          // have an asset browser
            }
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
    if (ImGui::Selectable(("New " + meta_info.name).c_str())) {
        if (!entity->has_component<C>()) { entity->add_component<C>(); }
    }
}

template<typename C, typename... Cs>
void show_add_component_list(SceneObject* entity) {
    using namespace ::Saturn::Components;
    if (!std::is_same_v<C, Name> && !std::is_same_v<C, ParticleEmitter>) {
        show_add_component_entry<C>(entity);
    }

    if constexpr (sizeof...(Cs) != 0) {
        show_add_component_list<Cs...>(entity);
    }
}

void show_entity_actions_popup(Scene& scene,
                               SceneObject*& entity,
                               SceneObject*& selected_entity) {
    using namespace Components;
    if (ImGui::BeginPopup("Entity actions")) {
        if (ImGui::Selectable("Delete entity")) {
            log::log(fmt::format("Deleting entity: {}",
                                 entity->get_component<Name>().name));
            scene.destroy_object(selected_entity);
            selected_entity = nullptr;
            entity = nullptr;
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::BeginMenu("Add Component")) {
            impl::show_add_component_list<COMPONENT_LIST>(entity);
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
void EntityTree::reset_selected_entity() { selected_entity = nullptr; }

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

        if (ImGui::IsItemClicked()) { selected_entity = *entity; }
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
                // next section of our tree, so break out of the loop We
                // first decrement cur because it will be incremented in the
                // next iteration of the loop
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
    if (ImGui::Button("Entity actions ...", ImVec2(150, 0))) {
        ImGui::OpenPopup("Entity actions");
    }
    impl::show_entity_actions_popup(scene, entity, selected_entity);
    if (entity) { impl::display_components<COMPONENT_LIST>(entity); }
}

} // namespace Saturn::Editor

#endif
