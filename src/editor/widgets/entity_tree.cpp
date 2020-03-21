#include <editor/widgets/entity_tree.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <unordered_map>

#include <stl/tuple.hpp>

#include <saturn/components/name.hpp>

using namespace saturn::components;

namespace editor {

EntityTree::EntityTree(const char* name, saturn::ecs::registry& ecs) : name(name), ecs(&ecs) {

}

using expanded_map_t = std::unordered_map<saturn::ecs::entity_t, bool>;

static stl::tuple<saturn::ecs::registry*, expanded_map_t*, saturn::ecs::entity_t*, EntityDragDropData*> display_tree(
    saturn::ecs::entity_t entity, stl::tree<saturn::ecs::entity_t>::const_traverse_info info, 
    saturn::ecs::registry* ecs, expanded_map_t* expanded, saturn::ecs::entity_t* selected_entity, EntityDragDropData* payload) {

    // Skip root entity
    if (entity == 0) { return stl::make_tuple(ecs, expanded, selected_entity, payload); }

    // If the parent was not expanded, skip this entity
    if (info.parent.leaf() != nullptr && (*expanded)[info.parent->data] == false) {
        return stl::make_tuple(ecs, expanded, selected_entity, payload);
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

    if (ImGui::BeginDragDropSource()) {
        payload->entity = entity;
        payload->registry = ecs;
        ImGui::SetDragDropPayload("payload_entity", payload, sizeof(EntityDragDropData));
        ImGui::Text("drag drop in progress");
        ImGui::EndDragDropSource();
    }


    // Continue iterating with the same parameters
    return stl::make_tuple(ecs, expanded, selected_entity, payload);
}

static void tree_post_callback(saturn::ecs::entity_t entity, stl::tree<saturn::ecs::entity_t>::const_traverse_info,
    saturn::ecs::registry*, expanded_map_t* expanded, saturn::ecs::entity_t*, EntityDragDropData*) {
    if (entity != 0 && (*expanded)[entity]) {
        ImGui::TreePop();
    }
}

void EntityTree::show(saturn::FrameContext& ctx) {
    if (ImGui::Begin(name.c_str(), get_shown_pointer())) {

        // Child to enable drag drop on full window
        std::string drag_drop_name = name + "drag_drop";
        ImGui::BeginChild(drag_drop_name.c_str());
    
        auto scene_root = ecs->get_entities().root();
        expanded_map_t expanded_entities;
        // Set root node to expanded state
        expanded_entities[0] = true;
        auto const& color = ImGui::GetStyleColorVec4(ImGuiCol_HeaderHovered);
        ImGui::PushStyleColor(ImGuiCol_Header, color);
        // Display all entities
        ecs->get_entities().traverse(display_tree, tree_post_callback, ecs, &expanded_entities, &selected_entity, &current_payload);
        ImGui::PopStyleColor();

        ImGui::EndChild();


        // Only let this scene view be a drag drop target if the current payload is our payload
        bool accept_payload = true;
        if (ImGuiPayload const* payload = ImGui::GetDragDropPayload()) {
            EntityDragDropData const* payload_data = reinterpret_cast<EntityDragDropData*>(payload->Data);
            if (payload_data->registry == ecs) { accept_payload = false; }
        } 

        if (accept_payload && ImGui::BeginDragDropTarget()) {
            if (ImGuiPayload const* payload = ImGui::AcceptDragDropPayload("payload_entity")) {
                STL_ASSERT(payload->DataSize == sizeof(EntityDragDropData), "data size does not match");
                
                EntityDragDropData* payload_data = reinterpret_cast<EntityDragDropData*>(payload->Data);
                // Only if the payload comes from a different ecs
                if (payload_data->registry != ecs) {
                    ecs->import_blueprint(*payload_data->registry, payload_data->entity);
                }

                payload_data->registry = nullptr;
                payload_data->entity = 0;
            }

            ImGui::EndDragDropTarget();
        }
    }

    ImGui::End();
}

}