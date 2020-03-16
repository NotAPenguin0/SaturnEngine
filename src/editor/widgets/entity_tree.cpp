#include <editor/widgets/entity_tree.hpp>

#include <imgui/imgui.h>
#include <unordered_map>

#include <saturn/components/name.hpp>

using namespace saturn::components;

namespace editor {

EntityTree::EntityTree(const char* name, saturn::ecs::registry& ecs) : name(name), ecs(&ecs) {

}

using expanded_map_t = std::unordered_map<saturn::ecs::entity_t, bool>;

static std::tuple<saturn::ecs::registry*, expanded_map_t*> display_tree(
    saturn::ecs::entity_t entity, stl::tree<saturn::ecs::entity_t>::const_traverse_info info, 
    saturn::ecs::registry* ecs, expanded_map_t* expanded) {

    // Skip root entity
    if (entity == 0) { return std::make_tuple(ecs, expanded); }

    // If the parent was not expanded, skip this entity
    if (info.parent.leaf() != nullptr && (*expanded)[info.parent->data] == false) {
        return std::make_tuple(ecs, expanded);
    }

    // display this entity
    std::string name = "no_name";
    if (ecs->has_component<Name>(entity)) {
        name = ecs->get_component<Name>(entity).name;
    }

    // If the entity has no children, display it as a leaf
    ImGuiTreeNodeFlags flags;
    if (info.it->children.empty()) {
        flags |= ImGuiTreeNodeFlags_Leaf;
    }

    if (ImGui::TreeNodeEx(name.c_str(), flags)) {
        (*expanded)[entity] = true;
        // Insert other display code here (probably not that much)
    }

    // Continue iterating with the same parameters
    return std::make_tuple(ecs, expanded);
}

static void tree_post_callback(saturn::ecs::entity_t entity, stl::tree<saturn::ecs::entity_t>::const_traverse_info,
    saturn::ecs::registry*, expanded_map_t* expanded) {
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
        ecs->get_entities().traverse(display_tree, tree_post_callback, ecs, &expanded_entities);
    }

    ImGui::End();
}

}