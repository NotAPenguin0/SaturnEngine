#ifndef SATURN_SCENE_HPP_
#define SATURN_SCENE_HPP_

#include <saturn/ecs/registry.hpp>

#include <phobos/renderer/render_graph.hpp>
#include <phobos/present/frame_info.hpp>

#include <saturn/utility/context.hpp>

#include <filesystem>
namespace fs = std::filesystem;

namespace saturn {

struct ModelToLoad {
    ecs::entity_t root;
    fs::path path;
};

class Scene {
public:
    ecs::registry ecs;
    // Stores entity blueprints
    ecs::registry blueprints;

    ecs::entity_t main_camera;

    stl::vector<ModelToLoad> models_to_load;

    void init_demo_scene(ph::VulkanContext* ctx);
    void build_render_graph(ph::FrameInfo& frame, ph::RenderGraph& graph);

    void save_to_file(ecs::registry const& registry, fs::path const& path);
    void load_from_file(ecs::registry& registry, fs::path const& path);

    void resolve_blueprint_references();
    void instantiate_blueprint(ecs::entity_t entity, ecs::entity_t blueprint);
    void find_main_camera();
    void load_assets(Context& ctx);
};

} // namespace saturn

#endif