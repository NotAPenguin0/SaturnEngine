#include <editor/widgets/entity_tree.hpp>

#include <imgui/imgui.h>
#include <unordered_map>

#include <saturn/components/name.hpp>

using namespace saturn::components;

namespace editor {

EntityTree::EntityTree(const char* name, saturn::ecs::registry& ecs) : name(name), ecs(&ecs) {

}

using expanded_map_t = std::unordered_map<saturn::ecs::entity_t, bool>;

static std::tuple<saturn::ecs::registry*, expanded_map_t*, saturn::ecs::entity_t*> display_tree(
    saturn::ecs::entity_t entity, stl::tree<saturn::ecs::entity_t>::const_traverse_info info, 
    saturn::ecs::registry* ecs, expanded_map_t* expanded, saturn::ecs::entity_t* selected_entity) {

    // Skip root entity
    if (entity == 0) { return std::make_tuple(ecs, expanded, selected_entity); }

    // If the parent was not expanded, skip this entity
    if (info.parent.leaf() != nullptr && (*expanded)[info.parent->data] == false) {
        return std::make_tuple(ecs, expanded, selected_entity);
    }

    // display this entity
    std::string name = "no_name";
    if (ecs->has_component<Name>(entity)) {
        name = ecs->get_component<Name>(entity).name;
    }

    // If the entity has no children, display it as a leaf
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (info.it->children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    // If the entity ID matches the selected entity ID, highlight it
    if (entity != 0 && entity == *selected_entity) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    if (ImGui::TreeNodeEx(name.c_str(), flags)) {
        (*expanded)[entity] = true;
        
        if (ImGui::IsItemClicked()) {
            *selected_entity = entity;
        }
    }

    // Continue iterating with the same parameters
    return std::make_tuple(ecs, expanded, selected_entity);
}

static void tree_post_callback(saturn::ecs::entity_t entity, stl::tree<saturn::ecs::entity_t>::const_traverse_info,
    saturn::ecs::registry*, expanded_map_t* expanded, saturn::ecs::entity_t*) {
    if (entity != 0 && (*expanded)[entity]) {
        ImGui::TreePop();
    }
}

void EntityTree::show(saturn::FrameContext& ctx) {
    if (ImGui::Begin(name.c_str(), get_shown_pointer())) {
        auto scene_root = ecs->get_entities().root();
        expanded_map_t expanded_entities;
        // Set root node to expanded state
        expanded_entities[0] = true;
        auto const& color = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
        ImGui::PushStyleColor(ImGuiCol_Header, color);
        ecs->get_entities().traverse(display_tree, tree_post_callback, ecs, &expanded_entities, &selected_entity);
        ImGui::PopStyleColor();
    }

    ImGui::End();
}

}