#include <saturn/scene/scene.hpp>

#include <saturn/components/point_light.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/transform.hpp>
#include <saturn/components/material.hpp>

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

    load_from_file("data/ecs.bin");

    Handle<assets::Model> model_handle = assets::load_model(context, "data/models/bunny.obj");
    
    assets::Model* model = assets::get_model(model_handle);
    // Add dummy material component. Default material will be used anyway (TODO)
    blueprints.add_component<Material>(model->blueprint);
    blueprints.add_component<Transform>(model->blueprint, 
        glm::vec3(0, 0, 0), glm::vec3(0, 80.0f, 0), glm::vec3(0.6, 0.6, 0.6));
    ecs.import_entity(blueprints, model->blueprint);
}  

void Scene::build_render_graph(ph::FrameInfo& frame, ph::RenderGraph& graph) {
    using namespace components;
    
    for (auto const&[material] : ecs.view<Material>()) {
        if (material.texture.id == -1) continue;
        graph.materials.push_back(ph::Material{assets::get_texture(material.texture)});
    }

    auto& color_attachment = frame.present_manager->get_attachment("color1");
    glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
        (float)color_attachment.get_width() / (float)color_attachment.get_height(), 0.1f, 100.0f);
    projection[1][1] *= -1;

    graph.view = view;
    graph.projection = projection;
    graph.camera_pos = glm::vec3(2, 2, 0);

    for (auto const&[transform, light] : ecs.view<Transform, PointLight>()) {
        ph::PointLight pt_light;
        pt_light.position = transform.position;
        pt_light.ambient = light.ambient;
        pt_light.diffuse = light.diffuse;
        pt_light.specular = light.specular;

        graph.point_lights.push_back(pt_light);
    }

    for (auto const&[transform, mesh, material] 
        : ecs.view<Transform, StaticMesh, Material>()) {
        
        ph::RenderGraph::DrawCommand draw_cmd;
        // TODO: Fix when we get better material component
        draw_cmd.material_index = 0;
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

void Scene::save_to_file(fs::path const& path) {
    nlohmann::json ecs_json;
    ecs_json = ecs;
    std::ofstream out("data/ecs.bin", std::ios::binary);
    out << ecs_json;
}

void Scene::load_from_file(fs::path const& path) {
    std::ifstream file(path);
    nlohmann::json j;
    file >> j;
    ecs = j;
}

} // namespace saturn