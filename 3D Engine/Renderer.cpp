#include "Renderer.hpp"

#include "Application.hpp"
#include "Exceptions.hpp"

#include "bind_guard.hpp"

#include "CameraTransform.hpp"
#include "LinearTransform.hpp"

#include "Trig.hpp"

#include <sstream>

namespace Saturn {

static std::vector<float> screen_vertices = {
    // Vertices	        Texture coords
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // BR
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TR
};

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

    // Setup the framebuffer
    Framebuffer::CreateInfo framebuffer_create_info;
    framebuffer_create_info.size = screen_size;
    framebuf.assign(framebuffer_create_info);

    // Create default viewport
    add_viewport(Viewport(0, 0, screen_size.x, screen_size.y));
    // Set it as the active viewport
    Viewport::set_active(get_viewport(0));

    std::vector<VertexAttribute> screen_attributes;
    screen_attributes.push_back({0, 3}); // Position is a vec3
    screen_attributes.push_back({1, 2}); // TexCoords is a vec2

    screen.assign({screen_attributes, screen_vertices, {0, 1, 2, 0, 3, 2}});

    // Setup shaders
    Shader::CreateInfo default_shader_create_info;
    default_shader_create_info.vtx_path =
        "resources/shaders/postprocessing/default_v.glsl";
    default_shader_create_info.frag_path =
        "resources/shaders/postprocessing/default_f.glsl";
    default_shader.assign(default_shader_create_info);
    LogSystem::write(LogSystem::Severity::Info, "Renderer created.");
    LogSystem::write(LogSystem::Severity::Warning,
                     "No postprocessing shader system in place. Default shader "
                     "will be used");
}

Renderer::~Renderer() {}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {

    bind_guard<Framebuffer> framebuf_guard(framebuf);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(flags);
}

void Renderer::render_scene_graph(SceneGraph const& scene) {
    // Temporary

    bind_guard<Framebuffer> framebuf_guard(framebuf);
    // All temporary obviously
    for (std::size_t i = 0; i < scene.vtx_arrays.size(); ++i) {
        auto& shader = *scene.shader;
        auto& vtx_array = *scene.vtx_arrays[i];
        auto& transform = *scene.transforms[i];

        auto projection = Math::Transform::perspective(
            Math::radians(45.0f), (float)screen_size.x / (float)screen_size.y,
            0.1f, 100.0f);
        float radius = 10.0f;
        float camX = sin((float)glfwGetTime()) * radius;
        float camZ = cos((float)glfwGetTime()) * radius;
        Math::Matrix4x4<float> view;
        view = Math::Transform::look_at(Math::Vec3<float>(camX, 0.0, camZ),
                                        Math::Vec3<float>(0.0f, 0.0f, 0.0f));

        auto model = Math::Matrix4x4<float>::identity();
        // Apply transformations
        Math::Transform::add_translation(model, transform.position);
        Math::Transform::add_rotation(model, transform.rotation.axis,
                                      transform.rotation.angle_in_radians);
        Math::Transform::add_scale(model, transform.scale);

        bind_guard<Shader> shader_guard(shader);
        bind_guard<VertexArray> vao_guard(vtx_array);

        shader.set_mat4("model", model);
        shader.set_mat4("view", view);
        shader.set_mat4("projection", projection);

        glDrawElements(GL_TRIANGLES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
    }
}

void Renderer::update_screen() {
    bind_guard<Framebuffer> framebuf_guard(screen_framebuf);

    // Bind VAO
    bind_guard<VertexArray> screen_guard(screen);
    bind_guard<Ebo> ebo_guard(screen.ebo);

    // Temporarily disable some GL functionality
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Set (postprocessing) shader
    bind_guard<Shader> shader_guard(default_shader);

    // Render framebuffer texture to the screen
    glBindTexture(GL_TEXTURE_2D, framebuf.texture);
    glDrawElements(GL_TRIANGLES, screen.index_size(), GL_UNSIGNED_INT, nullptr);
    //	  Re enable functionality
    glEnable(GL_DEPTH_TEST);
    //    glEnable(GL_CULL_FACE); //#Optimize Enable later when 3D works
}

void Renderer::debug_log_viewport() const {
    Viewport vp = Viewport::current();
    LogSystem::write(LogSystem::Severity::Info, "Viewport info: ");
    std::ostringstream oss;
    oss << "x: " << vp.position().x << " y: " << vp.position().y << "\n";
    oss << "w: " << vp.dimensions().x << " h: " << vp.dimensions().y << "\n";
    LogSystem::write(LogSystem::Severity::Info, oss.str());
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
