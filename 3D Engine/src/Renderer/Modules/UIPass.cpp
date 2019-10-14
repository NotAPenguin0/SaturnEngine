#include "Renderer/Modules/UIPass.hpp"

#include "AssetManager/AssetManager.hpp"
#include "ECS/Components/Canvas.hpp"

using namespace Saturn::Components;

namespace Saturn::RenderModules {

namespace impl {

static unsigned int create_texture(glm::vec2 size) {
    unsigned int handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Fill with empty data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

    return handle;
}

static void resize_if_needed(Framebuffer& target, glm::vec2 size) {
    if (auto const& framebuf_size = target.get_size();
        framebuf_size.x != (size_t)size.x ||
        framebuf_size.y != (size_t)size.y) {

        unsigned int new_texture = create_texture(size);
        target.delete_texture();
        target.set_size({(size_t)size.x, (size_t)size.y});
        target.assign_texture(new_texture);
    }
}

static void blit_framebuffer(Framebuffer& source,
                             Framebuffer& target,
                             Shader& shader,
                             VertexArray& quad) {
    Framebuffer::bind(target);
    glViewport(0, 0, target.get_size().x, target.get_size().y);

    VertexArray::bind(quad);

    Shader::bind(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.get_texture());

    shader.set_int(Shader::Uniforms::Texture, 0);
    shader.set_vec2(0, glm::vec2(1, 1));
	shader.set_vec2(1, glm::vec2(0, 0));
    glDrawElements(GL_TRIANGLES, quad.index_size(), GL_UNSIGNED_INT, nullptr);
}

static void blit_ui(Framebuffer& source,
                    Framebuffer& target,
                    Shader& shader,
                    VertexArray& quad,
                    glm::vec2 ui_rel_size, glm::vec2 ui_pos) {
    Framebuffer::bind(target);
    glViewport(0, 0, target.get_size().x, target.get_size().y);

    VertexArray::bind(quad);

    Shader::bind(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.get_texture());

    shader.set_int(Shader::Uniforms::Texture, 0);
    shader.set_vec2(0, ui_rel_size);
	shader.set_vec2(1, ui_pos);
    glDrawElements(GL_TRIANGLES, quad.index_size(), GL_UNSIGNED_INT, nullptr);
}

static std::vector<float> quad_vertices = {
    // Vertices	        Texture coords
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // BR
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TR
};

} // namespace impl

UIPass::UIPass() : PostRenderStage(10) {}

void UIPass::init() {
    Framebuffer::CreateInfo info;
    // Default, will be resized to the scene view size later
    info.size = {1024, 1024};
    target.assign(info);
    // Default, will be resized to the canvas size later
    ui_buffer.assign(info);

    // Create quad VAO
    std::vector<VertexAttribute> quad_attributes;
    quad_attributes.push_back({0, 3}); // Position is a vec2 here
    quad_attributes.push_back({1, 2}); // TexCoords is a vec2
    quad.assign({quad_attributes, impl::quad_vertices, {0, 1, 2, 0, 3, 2}});

    ui_shader = AssetManager<Shader>::get_resource(
        "config/resources/shaders/ui/ui_main.sh", true);
    blit_shader = AssetManager<Shader>::get_resource(
        "config/resources/shaders/ui/blit.sh", true);
}

void UIPass::process(Scene& scene, Framebuffer& source) {
    auto canvas_list = scene.get_ecs().select<Canvas>();

    for (auto [canvas] : canvas_list) {

        // Resize framebuffers if needed
        impl::resize_if_needed(
            target, glm::vec2(source.get_size().x, source.get_size().y));
        glm::vec2 ui_size_pixels =
            glm::vec2(canvas.size.x * source.get_size().x,
                      canvas.size.y * source.get_size().y);
        impl::resize_if_needed(ui_buffer, ui_size_pixels);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        // First, blit the scene view to our target buffer, then render the UI
        // to the UI buffer and blit both on the target buffer

        // Blit source buffer to target buffer
        impl::blit_framebuffer(source, target, *blit_shader, quad);

        // Render UI

        Framebuffer::bind(ui_buffer);
        glViewport(0, 0, ui_buffer.get_size().x, ui_buffer.get_size().y);

        VertexArray::bind(quad);

        Shader::bind(ui_shader.get());

        glDrawElements(GL_TRIANGLES, quad.index_size(), GL_UNSIGNED_INT,
                       nullptr);

        // Blit UI to target buffer
        impl::blit_ui(ui_buffer, target, *blit_shader, quad, canvas.size, canvas.position);

        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        // Only process the first canvas
        break;
    }
}

} // namespace Saturn::RenderModules
