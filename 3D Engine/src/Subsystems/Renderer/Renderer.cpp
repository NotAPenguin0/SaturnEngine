#include "Subsystems/Renderer/Renderer.hpp"

#include "Core/Application.hpp"
#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Renderer/PostProcessing.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Utility/Exceptions.hpp"
#include "Utility/Utility.hpp"
#include "Utility/bind_guard.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn {

static std::vector<float> screen_vertices = {
    // Vertices	        Texture coords
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // BR
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TR
};

void Renderer::setup_framebuffer(CreateInfo const& create_info) {
    Framebuffer::CreateInfo framebuf_info;
    framebuf_info.size = screen_size;
    framebuf.assign(framebuf_info);

    std::vector<VertexAttribute> screen_attributes;
    screen_attributes.push_back({0, 3}); // Position is a vec3
    screen_attributes.push_back({1, 2}); // TexCoords is a vec2

    screen.assign({screen_attributes, screen_vertices, {0, 1, 2, 0, 3, 2}});
}

void Renderer::create_default_viewport(CreateInfo const& create_info) {
    add_viewport(
        Viewport(0, 0, create_info.screen_size.x, create_info.screen_size.y));
    // Set it as the active viewport
    Viewport::set_active(get_viewport(0));
}

void Renderer::initialize_postprocessing() {
    PostProcessing::get_instance().load_shaders(
        "resources/shaders/postprocessing/postprocessing_effects.ppe");
    PostProcessing::get_instance().set_active("none");
}

void Renderer::create_uniform_buffers() {
    // View + Projection matrix buffer
    UniformBuffer::CreateInfo matrix_info;
    matrix_info.binding_point = 0;
    matrix_info.dynamic = true; // View/Projection matrices can change
    matrix_info.size_in_bytes = 2 * sizeof(glm::mat4);
    matrix_buffer.assign(matrix_info);

    // Lights buffer
    UniformBuffer::CreateInfo lights_info;
    lights_info.binding_point = 1;
    lights_info.dynamic =
        false; // for now, we assume lights are mostly static #CHECK
    lights_info.size_in_bytes =
        sizeof(int) + 12 +
        MaxLightsPerType * (4 * sizeof(glm::vec4)); //#UpdateMe When we
                                                    // add a new light type
    lights_buffer.assign(lights_info);

    // Camera data buffer
    UniformBuffer::CreateInfo camera_info;
    camera_info.binding_point = 2;
    camera_info.dynamic = true; // Camera data changes
    camera_info.size_in_bytes =
        sizeof(glm::vec4); // vec3 padded to the size of a vec4 because of
                           // std140 layout
    camera_buffer.assign(camera_info);
}

void Renderer::load_default_shaders() {

    no_shader_error =
        AssetManager<Shader>::get_resource("resources/shaders/default.sh");
    particle_shader =
        AssetManager<Shader>::get_resource("resources/shaders/particle.sh");
}

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

    setup_framebuffer(create_info);
    create_default_viewport(create_info);
    initialize_postprocessing();
    create_uniform_buffers();
    load_default_shaders();
}

Renderer::~Renderer() {}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {

    bind_guard<Framebuffer> framebuf_guard(framebuf);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(flags);
}

void Renderer::render_scene(Scene& scene) {
    bind_guard<Framebuffer> framebuf_guard(framebuf);

    // Render every viewport
    for (auto& vp : viewports) {
        if (!vp.has_camera()) continue;
        render_viewport(scene, vp);
    }
}

void Renderer::send_camera_matrices(Scene& scene,
                                    Viewport& vp,
                                    Components::Camera& camera) {
    auto& cam_trans = camera.entity->get_component<Components::Transform>();
    auto projection = glm::perspective(
        glm::radians(camera.fov),
        (float)vp.dimensions().x / (float)vp.dimensions().y, 0.1f, 100.0f);

    auto view = glm::lookAt(cam_trans.position,
                            cam_trans.position + camera.front, camera.up);

    bind_guard<UniformBuffer> matrix_guard(matrix_buffer);
    matrix_buffer.set_mat4(projection, 0);
    matrix_buffer.set_mat4(view, sizeof(glm::mat4));

    bind_guard<UniformBuffer> cam_guard(camera_buffer);
    camera_buffer.set_vec3(cam_trans.position, 0);
}

void Renderer::send_lighting_data(Scene& scene) {
    bind_guard<UniformBuffer> lights_guard(lights_buffer);
    auto point_lights = collect_point_lights(scene);
    lights_buffer.set_int(point_lights.size(), 0);
    for (std::size_t i = 0; i < point_lights.size(); ++i) {
        auto lightpos = point_lights[i]
                            ->entity->get_component<Components::Transform>()
                            .position;
        auto offset =
            sizeof(int) /*the size variable*/ +
            12 /*Padding after the size variable*/ +
            i * (4 * sizeof(glm::vec4)); /*Add offsets for
                                      every point light in the array. vec4
                                      because of std140 layout padding*/
        lights_buffer.set_vec3(point_lights[i]->ambient, offset);
        lights_buffer.set_vec3(point_lights[i]->diffuse,
                               offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(point_lights[i]->specular,
                               offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(lightpos, offset + 3 * sizeof(glm::vec4));
    }
}

void Renderer::send_model_matrix(
    Shader& shader, Components::Transform const& relative_transform) {
    // Make sure to get absolute transform
    auto transform = make_absolute_transform(relative_transform);

    auto model = glm::mat4(1.0f);
    // Apply transformations
    model = glm::translate(model, transform.position);
    model = glm::rotate(model, {glm::radians(transform.rotation.x),
                                glm::radians(transform.rotation.y),
                                glm::radians(transform.rotation.z)});
    model = glm::scale(model, transform.scale);
    // Send to shader
    bind_guard<Shader> guard(shader);
    shader.set_mat4(Shader::Uniforms::Model, model);
}

void Renderer::send_material_data(Shader& shader,
                                  Components::Material& material) {

    bind_guard<Shader> guard(shader);

    if (material.lit) {
        Texture::bind(material.diffuse_map.get());
        shader.set_int(Shader::Uniforms::Material::DiffuseMap,
                       material.diffuse_map->unit() - GL_TEXTURE0);
        Texture::bind(material.specular_map.get());
        shader.set_int(Shader::Uniforms::Material::SpecularMap,
                       material.specular_map->unit() - GL_TEXTURE0);
        shader.set_float(Shader::Uniforms::Material::Shininess,
                         material.shininess);
    } else if (material.texture.is_loaded() && material.shader.is_loaded()) {
        // If there is a texture
        Texture::bind(material.texture.get());

        shader.set_int(Shader::Uniforms::Texture,
                       material.texture->unit() - GL_TEXTURE0);
    }
}

void Renderer::unbind_textures(Components::Material& material) {
    if (material.texture.is_loaded() && material.shader.is_loaded()) {
        Texture::unbind(material.texture.get());
    }
    if (material.lit) {
        Texture::unbind(material.diffuse_map.get());
        Texture::unbind(material.specular_map.get());
    }
}

void Renderer::render_viewport(Scene& scene, Viewport& vp) {
    Viewport::set_active(vp);

    auto cam_id = vp.get_camera();
    auto& cam = scene.ecs.get_with_id<Components::Camera>(cam_id);
    send_camera_matrices(scene, vp, cam);

    send_lighting_data(scene);

    render_particles(scene); // #TODO: Check if it makes any difference
                             // if we render particles before or after
                             // the scene + figure out best option

    for (auto [relative_transform, mesh, material] :
         scene.ecs.select<Components::Transform, Components::StaticMesh,
                          Components::Material>()) {
        auto& shader = material.shader.is_loaded() ? material.shader.get()
                                                   : no_shader_error.get();

		// Send data to shader
        send_model_matrix(shader, relative_transform);
        send_material_data(shader, material);

		// Do the actual rendering
        auto& vtx_array = mesh.mesh->get_vertices();
        bind_guard<Shader> guard(shader);
        bind_guard<VertexArray> vao_guard(vtx_array);
        glDrawElements(GL_TRIANGLES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
		// Cleanup
		unbind_textures(material);
        
    }
}

//#MaybeTODO: Render particles with GL_POINTS if they're not textured?
void Renderer::render_particles(Scene& scene) {
    using namespace Components;
    bind_guard<Shader> shader_guard(particle_shader.get());

    Resource<Texture> default_texture =
        AssetManager<Texture>::get_resource("resources/textures/white.tex");

    glDisable(GL_CULL_FACE);
    for (auto [emitter] : scene.ecs.select<ParticleEmitter>()) {
        if (emitter.additive) { glBlendFunc(GL_SRC_ALPHA, GL_ONE); }
        // Bind VAO
        bind_guard<VertexArray> vao_guard(emitter.particle_vao.get());

        //            particle_shader->set_vec3(Shader::Uniforms::Position,
        //                                      particle.position);
        //            particle_shader->set_vec3(
        //                Shader::Uniforms::Scale,
        //                glm::vec3(particle.size.x, particle.size.y, 0.0f));
        auto& texture = emitter.texture.is_loaded() ? emitter.texture.get()
                                                    : default_texture.get();
        //            particle_shader->set_vec4(Shader::Uniforms::Color,
        //            particle.color);
        Texture::bind(texture);
        particle_shader->set_int(Shader::Uniforms::Texture,
                                 texture.unit() - GL_TEXTURE0);

        glDrawElementsInstanced(
            GL_TRIANGLES, emitter.particle_vao->index_size(), GL_UNSIGNED_INT,
            nullptr, emitter.particles.size());

        Texture::unbind(texture);

        if (emitter.additive) {
            // reset blend function to old one
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
    glEnable(GL_CULL_FACE);
}

std::vector<Components::PointLight*>
Renderer::collect_point_lights(Scene& scene) {
    std::vector<Components::PointLight*> result;
    for (auto [light] : scene.ecs.select<Components::PointLight>()) {
        result.push_back(&light);
    }
    return result;
}

void Renderer::update_screen() {
    bind_guard<Framebuffer> framebuf_guard(screen_framebuf);

    Viewport::set_active(get_viewport(0));

    // Bind VAO
    bind_guard<VertexArray> screen_guard(screen);
    bind_guard<Ebo> ebo_guard(screen.ebo);

    // Temporarily disable some GL functionality
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Set (postprocessing) shader
    bind_guard<Shader> shader_guard(
        PostProcessing::get_instance().get_active().get());

    // Render framebuffer texture to the screen
    glBindTexture(GL_TEXTURE_2D, framebuf.texture);
    PostProcessing::get_instance().get_active()->set_int(
        Shader::Uniforms::Texture, 0);
    glDrawElements(GL_TRIANGLES, screen.index_size(), GL_UNSIGNED_INT, nullptr);

    // Re enable functionality
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

Viewport& Renderer::get_viewport(std::size_t index) {
    if (index >= viewports.size()) {
        throw InvalidViewportException("Invalid viewport with index " +
                                       std::to_string(index) + " requested.");
    }
    return viewports[index];
}

std::size_t Renderer::add_viewport(Viewport vp) {
    viewports.push_back(vp);
    return viewports.size() - 1;
}

} // namespace Saturn
