#include <editor/widgets/inspector.hpp>

#include <saturn/meta/for_each_component.hpp>
#include <saturn/meta/reflect.hpp>

#include <imgui/imgui.h>

using namespace saturn;

namespace editor {

using namespace components;

namespace {

template<typename C>
struct display_entity_component {
    void operator()(ecs::registry& ecs, ecs::entity_t entity) {
        // Don't display name component
        if constexpr (std::is_same_v<C, Name>) { return; }

        if (ecs.has_component<C>(entity)) {
            meta::Info info = meta::reflect<C>();
            if (ImGui::CollapsingHeader(info.name.data())) {

            }
        }
    }
};

}

void Inspector::show(ecs::registry& ecs, ecs::entity_t entity) {
    if (ImGui::Begin("Inspector")) {

        // If no entity is selected, display an empty window
        if (entity == 0) { ImGui::End(); return; }

        auto& name = ecs.get_component<Name>(entity);
        name.name.resize(64);
        ImGui::InputText("Name##entity_name_input", name.name.data(), name.name.size());

        // Display components
        meta::for_each_component<display_entity_component>(ecs, entity);

        ImGui::End();
    }
}

}