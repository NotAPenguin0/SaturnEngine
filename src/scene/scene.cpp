#include <saturn/scene/scene.hpp>

#include <saturn/components/point_light.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/transform.hpp>
#include <saturn/components/camera.hpp>
#include <saturn/components/mesh_renderer.hpp>
#include <saturn/components/name.hpp>

#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include <phobos/present/present_manager.hpp>

#include <saturn/assets/assets.hpp>
#include <saturn/serialization/default_serializers.hpp>
#include <saturn/serialization/component_serializers.hpp>

#include <saturn/utility/math.hpp>

#include <fstream>
#include <numeric>

namespace saturn {

void Scene::init_demo_scene(ph::VulkanContext* ctx) {
    using namespace components;

    static Context context { ctx, this };
    set_serialize_context(&context);

    load_from_file(ecs, "data/ecs.bin");
    load_from_file(blueprints, "data/blueprints.bin");

    // Create camera entity
    main_camera = ecs.create_entity();
    ecs.add_component<Name>(main_camera, "Main Camera");
    ecs.add_component<Camera>(main_camera);
    ecs.add_component<Transform>(main_camera, glm::vec3(2, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

    Handle<assets::Model> model_handle = assets::load_model(context, "data/models/crytek-sponza/sponza.obj");
    
    assets::Model* model = assets::get_model(model_handle);
    ecs.import_entity(blueprints, model->blueprint);
}  

void Scene::build_render_graph(ph::FrameInfo& frame, ph::RenderGraph& graph) {
    using namespace components;
    
    // Gather materials
    for (auto material : assets::get_all_materials()) {
        graph.materials.push_back(*material);
    }

    auto& color_attachment = frame.present_manager->get_attachment("color1");

    // Setup camera data
    for (auto [transform, camera] : ecs.view<Transform, Camera>()) {
        graph.camera_pos = transform.position;
        graph.view = glm::lookAt(transform.position, transform.position + camera.front, camera.up);
        graph.projection = glm::perspective(camera.fov, 
            (float)color_attachment.get_width() / (float)color_attachment.get_height(), 0.1f, 5000.0f);
        // Flip projection because vulkan
        graph.projection[1][1] *= -1;
        // Only a single camera entity is supported atm
        break;
    }

    // Setup light data
    for (auto const&[transform, light] : ecs.view<Transform, PointLight>()) {
        ph::PointLight pt_light;
        pt_light.position = transform.position;
        pt_light.ambient = light.ambient;
        pt_light.diffuse = light.diffuse;
        pt_light.specular = light.specular;

        graph.point_lights.push_back(pt_light);
    }

    // Meshes to render
    for (auto const&[transform, mesh, mesh_renderer] 
        : ecs.view<Transform, StaticMesh, MeshRenderer>()) {
        
        ph::RenderGraph::DrawCommand draw_cmd;
        if (mesh_renderer.material.id >= 0) {
            draw_cmd.material_index = mesh_renderer.material.id;
        } else {
            draw_cmd.material_index = 0;
        }
        draw_cmd.mesh = assets::get_mesh(mesh.mesh);

        ph::RenderGraph::Instance instance;
        glm::mat4 model = glm::mat4(1.0);

        model = glm::translate(model, transform.position);
        model = glm::rotate(model, {glm::radians(transform.rotation.x),
                                glm::radians(transform.rotation.y),
                                glm::radians(transform.rotation.z)});
        model = glm::scale(model, transform.scale);

        instance.transform = model;

        draw_cmd.instances.push_back(instance);
        graph.draw_commands.push_back(draw_cmd);
    }
}

void Scene::save_to_file(ecs::registry const& registry, fs::path const& path) {
    nlohmann::json ecs_json;
    ecs_json = registry;
    std::ofstream out(path, std::ios::binary);
    out << ecs_json;
}

void Scene::load_from_file(ecs::registry& registry, fs::path const& path) {
    std::ifstream file(path);
    nlohmann::json j;
    file >> j;
    registry = j;
}

} // namespace saturn