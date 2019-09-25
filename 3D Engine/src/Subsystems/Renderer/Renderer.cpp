#include "Subsystems/Renderer/Renderer.hpp"

#include "Core/Application.hpp"
#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Renderer/PostProcessing.hpp"
#include "Subsystems/Renderer/RenderUtils.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Utility/Exceptions.hpp"
#include "Utility/Utility.hpp"
#include "Utility/bind_guard.hpp"

// Temp, testing
#include "Subsystems/Renderer/Modules/DepthMapPass.hpp"

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
        sizeof(int) +                           // point_light_count
        sizeof(int) +                           // directional_light_count
        sizeof(int) +                           // spot_light_count
        LightSizesBytes::PaddingAfterSizeVars + // padding
        MaxLightsPerType * (LightSizesBytes::PointLightGLSL +
                            LightSizesBytes::DirectionalLightGLSL +
                            LightSizesBytes::SpotLightGLSL);
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
    collider_shader =
        AssetManager<Shader>::get_resource("resources/shaders/collider.sh");
    axis_shader =
        AssetManager<Shader>::get_resource("resources/shaders/axis.sh");
    outline_shader =
        AssetManager<Shader>::get_resource("resources/shaders/outline.sh");
}

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

    setup_framebuffer(create_info);
    create_default_viewport(create_info);
    initialize_postprocessing();
    create_uniform_buffers();
    load_default_shaders();

    // #Temp, #RefactorTesting
    add_pre_render_stage(std::make_unique<RenderModules::DepthMapPass>());

    box_collider_mesh =
        AssetManager<Mesh>::get_resource("resources/meshes/box_collider.mesh");
    line_mesh = AssetManager<Mesh>::get_resource("resources/meshes/line.mesh");
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
    // Render every viewport
    for (auto& vp : viewports) {
        if (!vp.has_camera()) continue;

        // First, execute all pre-render stages
        for (auto& stage : pre_render_stages) {
            Framebuffer::bind(framebuf);
            stage->process(scene);
        }

        Framebuffer::bind(framebuf);

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
        // clang-format off
        const auto point_light_offset =
            sizeof(int) + // point_light_count
			sizeof(int) + // directional_light_count
			sizeof(int) + // spot_light_count
			LightSizesBytes::PaddingAfterSizeVars +
            i * (LightSizesBytes::PointLightGLSL); // point_lights array
        // clang-format on
        lights_buffer.set_vec3(point_lights[i]->ambient, point_light_offset);
        lights_buffer.set_vec3(point_lights[i]->diffuse,
                               point_light_offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(point_lights[i]->specular,
                               point_light_offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(lightpos,
                               point_light_offset + 3 * sizeof(glm::vec4));
        lights_buffer.set_float(point_lights[i]->intensity,
                                point_light_offset + 3 * sizeof(glm::vec4) +
                                    sizeof(glm::vec3));
    }

    constexpr std::size_t PointLightBaseOffset =
        (sizeof(int) + sizeof(int) + sizeof(int) +
         LightSizesBytes::PaddingAfterSizeVars +
         MaxLightsPerType * (LightSizesBytes::PointLightGLSL));

    auto directional_lights = collect_directional_lights(scene);
    lights_buffer.set_int(directional_lights.size(), sizeof(int));
    for (std::size_t i = 0; i < directional_lights.size(); ++i) {
        const auto offset =
            PointLightBaseOffset + i * (LightSizesBytes::DirectionalLightGLSL);
        lights_buffer.set_vec3(directional_lights[i]->ambient, offset);
        lights_buffer.set_vec3(directional_lights[i]->diffuse,
                               offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(directional_lights[i]->specular,
                               offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(directional_lights[i]->direction,
                               offset + 3 * sizeof(glm::vec4));
    }

    constexpr std::size_t DirectionalLightBaseOffset =
        MaxLightsPerType * LightSizesBytes::DirectionalLightGLSL;
    constexpr std::size_t OffsetBeforeSpotLights =
        PointLightBaseOffset + DirectionalLightBaseOffset;

    auto spot_lights = collect_spot_lights(scene);
    lights_buffer.set_int(spot_lights.size(), 2 * sizeof(int));
    for (std::size_t i = 0; i < spot_lights.size(); ++i) {
        const auto offset =
            OffsetBeforeSpotLights + i * LightSizesBytes::SpotLightGLSL;
        auto lightpos = spot_lights[i]
                            ->entity->get_component<Components::Transform>()
                            .position;
        lights_buffer.set_vec3(spot_lights[i]->ambient, offset);
        lights_buffer.set_vec3(spot_lights[i]->diffuse,
                               offset + sizeof(glm::vec4));
        lights_buffer.set_vec3(spot_lights[i]->specular,
                               offset + 2 * sizeof(glm::vec4));
        lights_buffer.set_vec3(lightpos, offset + 3 * sizeof(glm::vec4));
        lights_buffer.set_vec3(spot_lights[i]->direction,
                               offset + 4 * sizeof(glm::vec4));
        lights_buffer.set_float(
            spot_lights[i]->intensity,
            offset + 4 * sizeof(glm::vec4) +
                sizeof(glm::vec3)); // the first float is packed against the
                                    // previous vec3
        lights_buffer.set_float(
            glm::cos(glm::radians(spot_lights[i]->inner_angle)),
            offset + 5 * sizeof(glm::vec4));
        lights_buffer.set_float(
            glm::cos(glm::radians(spot_lights[i]->outer_angle)),
            offset + 5 * sizeof(glm::vec4) + sizeof(float));
    }
}


void Renderer::send_material_data(Shader& shader,
                                  Components::Material& material) {

    Shader::bind(shader);
    if (material.lit) {
        if (material.diffuse_map.is_loaded()) {
            Texture::bind(material.diffuse_map.get());
            shader.set_int(Shader::Uniforms::Material::DiffuseMap,
                           material.diffuse_map->unit() - GL_TEXTURE0);
        }
        if (material.specular_map.is_loaded()) {
            Texture::bind(material.specular_map.get());
            shader.set_int(Shader::Uniforms::Material::SpecularMap,
                           material.specular_map->unit() - GL_TEXTURE0);
        }
        shader.set_float(Shader::Uniforms::Material::Shininess,
                         material.shininess);
    }
    Shader::unbind();
}

void Renderer::unbind_textures(Components::Material& material) {
    if (material.lit) {
        if (material.diffuse_map.is_loaded()) {
            Texture::unbind(material.diffuse_map.get());
        }
        if (material.specular_map.is_loaded()) {
            Texture::unbind(material.specular_map.get());
        }
    }
}



void Renderer::debug_render_colliders(Scene& scene) {
    using namespace Components;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (auto [rel_trans, collider, _] :
         scene.ecs.select<Transform, BoxCollider, ColliderRenderer>()) {
        auto& shader = collider_shader.get();
        auto copy = rel_trans;
        copy.position += collider.center;
        copy.scale = collider.half_widths;
        copy.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
        send_model_matrix(shader, copy);
        Shader::bind(shader);
        auto& vtx_array = box_collider_mesh->get_vertices();
        bind_guard<VertexArray> vao(vtx_array);
        glDrawElements(GL_LINES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        Shader::unbind();
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::render_outlines(Scene& scene) {
    using namespace Components;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (auto [rel_trans, mesh, outline] :
         scene.ecs.select<Transform, StaticMesh, OutlineRenderer>()) {

        if (!mesh.mesh.is_loaded()) { continue; }

        auto& shader = outline_shader.get();
        send_model_matrix(shader, rel_trans);
        Shader::bind(shader);
        shader.set_vec3(Shader::Uniforms::Color, outline.color);
        auto& vtx_array = mesh.mesh->get_vertices();
        bind_guard<VertexArray> vao(vtx_array);
        glEnable(GL_LINE_SMOOTH);
        glDrawElements(GL_LINES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        glDisable(GL_LINE_SMOOTH);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::render_axes() {
    glm::vec3 rotations[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),  // y axis
        glm::vec3(90.0f, 0.0f, 0.0f), // z axis
        glm::vec3(0.0f, 0.0f, 90.0f)  // x axis
    };
    glm::vec3 colors[] = {
        glm::vec3(0.0f, 1.0f, 0.0f), // y axis
        glm::vec3(0.0f, 0.0f, 1.0f), // z axis
        glm::vec3(1.0f, 0.0f, 0.0f)  // x axis
    };
    auto& shader = axis_shader.get();
    auto& vertices = line_mesh->get_vertices();
    Components::Transform axis_transform;
    axis_transform.scale = glm::vec3(1.0, 100.0f, 1.0f);
    axis_transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    // There are 3 axes
    for (int i = 0; i < 3; ++i) {
        axis_transform.rotation = rotations[i];
        auto model = glm::mat4(1.0f);
        // Apply transformations
        model = glm::translate(model, axis_transform.position);
        model = glm::rotate(model, {glm::radians(axis_transform.rotation.x),
                                    glm::radians(axis_transform.rotation.y),
                                    glm::radians(axis_transform.rotation.z)});
        model = glm::scale(model, axis_transform.scale);
        // Send to shader
        Shader::bind(shader);
        shader.set_mat4(Shader::Uniforms::Model, model);
        bind_guard<VertexArray> vao(vertices);
        auto const& color = colors[i];
        shader.set_vec3(Shader::Uniforms::Color, color);
        glDrawElements(GL_LINES, vertices.index_size(), GL_UNSIGNED_INT,
                       nullptr);
    }
}

void Renderer::render_viewport(Scene& scene, Viewport& vp) {
    Viewport::set_active(vp);

    auto cam_id = vp.get_camera();
    auto& cam = scene.ecs.get_with_id<Components::Camera>(cam_id);
    send_camera_matrices(scene, vp, cam);

    send_lighting_data(scene);

    render_particles(scene);
    debug_render_colliders(scene);
    render_outlines(scene);
    render_axes();

    for (auto [relative_transform, mesh, material] :
         scene.ecs.select<Components::Transform, Components::StaticMesh,
                          Components::Material>()) {

        if (!mesh.mesh.is_loaded()) { continue; }

        auto& shader = material.shader.is_loaded() ? material.shader.get()
                                                   : no_shader_error.get();

        // Send data to shader
        send_model_matrix(shader, relative_transform);
        send_material_data(shader, material);

        // Set lightspace matrix in shader
        Shader::bind(shader);
        if (material.lit) {
            auto lightspace = get_lightspace_matrix(scene);
            shader.set_mat4(Shader::Uniforms::LightSpaceMatrix, lightspace);
            if (RenderModules::DepthMapPass::last_depthmap) {
                // Set shadow map in shader
                glActiveTexture(GL_TEXTURE2);
                DepthMap::bind_texture(    
                    *RenderModules::DepthMapPass::last_depthmap);
                shader.set_int(Shader::Uniforms::DepthMap, 2);
            }
        }

        // Do the actual rendering (maybe put this in another function
        // render_mesh() or something)
        auto& vtx_array = mesh.mesh->get_vertices();

        bind_guard<VertexArray> vao_guard(vtx_array);
        if (!mesh.face_cull) { glDisable(GL_CULL_FACE); }
        glDrawElements(GL_TRIANGLES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        if (!mesh.face_cull) { glEnable(GL_CULL_FACE); }
        // Cleanup
        unbind_textures(material);
        glActiveTexture(GL_TEXTURE2);
        DepthMap::unbind_texture();
    }
}

void Renderer::render_particles(Scene& scene) {
    using namespace Components;
    Shader::bind(particle_shader.get());

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

std::vector<Components::DirectionalLight*>
Renderer::collect_directional_lights(Scene& scene) {
    std::vector<Components::DirectionalLight*> result;
    for (auto [light] : scene.ecs.select<Components::DirectionalLight>()) {
        result.push_back(&light);
    }
    return result;
}

std::vector<Components::SpotLight*>
Renderer::collect_spot_lights(Scene& scene) {
    std::vector<Components::SpotLight*> result;
    for (auto [light] : scene.ecs.select<Components::SpotLight>()) {
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

    // Enable gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Set (postprocessing) shader
    Shader::bind(PostProcessing::get_instance().get_active().get());

    // Render framebuffer texture to the screen
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuf.texture);

    PostProcessing::get_instance().get_active()->set_int(
        Shader::Uniforms::Texture, 0);
    glDrawElements(GL_TRIANGLES, screen.index_size(), GL_UNSIGNED_INT, nullptr);

    // Re enable functionality
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Disable gamma correction
    glDisable(GL_FRAMEBUFFER_SRGB);
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

void Renderer::add_pre_render_stage(
    std::unique_ptr<RenderModules::PreRenderStage> stage) {

    pre_render_stages.push_back(std::move(stage));
    // Initialize the render stage
    pre_render_stages.back()->init();
    // re-sort the render stages
    std::sort(pre_render_stages.begin(), pre_render_stages.end(),
              [](auto const& lhs, auto const& rhs) { return *lhs < *rhs; });
}

} // namespace Saturn
