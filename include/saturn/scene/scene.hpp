#ifndef SATURN_SCENE_HPP_
#define SATURN_SCENE_HPP_

#include <saturn/ecs/registry.hpp>

#include <phobos/renderer/render_graph.hpp>
#include <phobos/present/frame_info.hpp>

#include <filesystem>
namespace fs = std::filesystem;

namespace saturn {

class Scene {
public:
    ecs::registry ecs;
    // Stores entity blueprints
    ecs::registry blueprints;

    ecs::entity_t main_camera;

    void init_demo_scene(ph::VulkanContext* ctx);
    void build_render_graph(ph::FrameInfo& frame, ph::RenderGraph& graph);

    void save_to_file(fs::path const& path);
    void load_from_file(fs::path const& path);
};

} // namespace saturn

#endif