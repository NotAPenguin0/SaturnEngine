#ifndef SATURN_SCENE_HPP_
#define SATURN_SCENE_HPP_

#include <saturn/ecs/registry.hpp>

#include <phobos/renderer/render_graph.hpp>

namespace saturn {

class Scene {
public:
    ecs::registry ecs;

    void init_demo_scene(ph::VulkanContext* ctx, ph::AssetManager* asset_manager);
    void build_render_graph(ph::RenderGraph& graph);
private:
    // TODO: Fix this to be more customizable
    ph::Material default_material;
};

} // namespace saturn

#endif