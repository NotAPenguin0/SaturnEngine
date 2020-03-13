#include <saturn/scene/scene.hpp>

#include <saturn/components/point_light.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/transform.hpp>
#include <saturn/components/material.hpp>

#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

#include <phobos/present/present_manager.hpp>

#include <numeric>

namespace saturn {

void Scene::init_demo_scene(ph::VulkanContext* ctx, ph::AssetManager* asset_manager) {
    using namespace components;

    ecs::entity_t light = ecs.create_entity();
    ecs::entity_t box = ecs.create_entity();

    ecs.add_component<Transform>(light, glm::vec3(0, 2, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    ecs.add_component<PointLight>(light, glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.75, 0.75, 0.75), glm::vec3(1, 1, 1));

    ecs.add_component<Transform>(box, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5));
    ph::Mesh::CreateInfo box_info;

    float vertices[] = {
        -1, -1, -1, 0, 0, -1, 1, 1,    1, 1, -1, 0, 0, -1, 0, 0,     1,  -1, -1, 0, 0, -1, 0, 1,
        1,  1, -1, 0, 0, -1, 0, 0,    -1, -1, -1, 0, 0, -1, 1, 1,   -1, 1, -1, 0, 0, -1, 1, 0,

        -1, -1, 1, 0, 0, 1, 0, 1,    1, -1, 1, 0, 0, 1, 1, 1,     1, 1, 1, 0, 0, 1, 1, 0,
        1,  1,  1, 0, 0, 1, 1, 0,    -1, 1, 1, 0, 0, 1, 0, 0,     -1, -1, 1, 0, 0, 1, 0, 1,

        -1, 1, -1, -1, 0, 0, 0, 0,    -1, -1, -1, -1, 0, 0, 0, 1,   -1, 1, 1, -1, 0, 0, 1, 0,
        -1, -1, -1, -1, 0, 0, 0, 1,   -1, -1, 1, -1, 0, 0, 1, 1,    -1, 1, 1, -1, 0, 0, 1, 0,

        1, 1, 1, 1, 0, 0, 0, 0,      1, -1, -1, 1, 0, 0, 1, 1,    1, 1, -1, 1, 0, 0, 1, 0,
        1, -1, -1, 1, 0, 0, 1, 1,    1,  1, 1, 1, 0, 0, 0, 0,     1, -1, 1, 1, 0, 0, 0, 1,

        -1, -1, -1, 0, -1, 0, 0, 1,   1, -1, -1, 0, -1, 0, 1, 1,    1, -1, 1, 0, -1, 0, 1, 0,
        1, -1, 1, 0, -1, 0, 1, 0,     -1, -1, 1, 0, -1, 0, 0, 0,    -1, -1, -1, 0, -1, 0, 0, 1,

        -1, 1, -1, 0, 1, 0, 0, 0,    1, 1, 1, 0, 1, 0, 1, 1,      1, 1, -1, 0, 1, 0, 1, 0,
        1,  1, 1, 0, 1, 0, 1, 1,    -1, 1, -1, 0, 1, 0, 0, 0,    -1, 1, 1, 0, 1, 0, 0, 1
    };
  
    uint32_t indices[36];
    std::iota(indices, indices + 36, 0);
    box_info.ctx = ctx;
    box_info.vertex_count = 36;
    box_info.vertices = vertices;
    box_info.vertex_size = 8;
    box_info.index_count = 36;
    box_info.indices = indices;

    ph::Handle<ph::Mesh> box_mesh = asset_manager->add_mesh(box_info); 

    int w, h, channels;
    uint8_t* img = stbi_load("data/textures/blank.png", &w, &h, &channels, STBI_rgb_alpha);
    ph::Texture::CreateInfo tex_info;
    tex_info.ctx = ctx;
    tex_info.channels = channels;
    tex_info.format = vk::Format::eR8G8B8A8Srgb;
    tex_info.width = w;
    tex_info.height = h;
    tex_info.data = img;
    ph::Handle<ph::Texture> texture = asset_manager->add_texture(tex_info);

    ecs.add_component<StaticMesh>(box, box_mesh);
    ecs.add_component<Material>(box, texture);

    default_material.texture = texture;
}  

void Scene::build_render_graph(ph::FrameInfo& frame, ph::RenderGraph& graph) {
    using namespace components;

    graph.materials.push_back(&default_material);

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
        draw_cmd.material_index = 0;
        draw_cmd.mesh = mesh.mesh;

        ph::RenderGraph::Instance instance;
        glm::mat4 model = glm::mat4(1.0);
        // TODO: Rotations
        model = glm::scale(model, transform.scale);
        model = glm::translate(model, transform.position);
       
        instance.transform = model;

        draw_cmd.instances.push_back(instance);
        graph.draw_commands.push_back(draw_cmd);
    }
}

} // namespace saturn