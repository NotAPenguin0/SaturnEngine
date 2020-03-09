#include "Editor/PreviewRendering.hpp"

#include "Renderer/Font.hpp"
#include "Renderer/Framebuffer.hpp"
#include "Renderer/Model.hpp"
#include "Renderer/OpenGL.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn::Editor::previews {

namespace impl {

using texture_t = unsigned int;
using texture_cache = std::unordered_map<std::string, texture_t>;

glm::mat4 get_pv_matrix(glm::vec3 const& cam_pos) {
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::vec3 cam_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cam_dir = glm::normalize(cam_pos - cam_target);
    glm::vec3 world_up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 cam_right = glm::normalize(glm::cross(world_up, cam_dir));
    glm::vec3 cam_up = glm::cross(cam_dir, cam_right);
    glm::mat4 view = glm::lookAt(cam_pos, cam_target, cam_up);

    return projection * view;
}

struct render_info {
    render_info() {
        Framebuffer::CreateInfo info;
        info.size = {max_preview_size, max_preview_size};
        framebuf.assign(info);
        // Remove owned texture as we will supply our own later
        framebuf.delete_texture();

        // Load the preview shader
        shader = AssetManager<Shader>::get_resource(
            "config/resources/shaders/mesh_preview.sh", true);
        pvm_matrix = get_pv_matrix(cam_pos) * model_matrix;

        light_data.direction = glm::vec3(1, -1, -1);
        light_data.ambient = glm::vec3(0.3, 0.3, 0.3);
        light_data.diffuse = glm::vec3(0.8, 0.8, 0.8);
        light_data.specular = glm::vec3(1.0, 1.0, 1.0);

        obj_color = glm::vec3(1.0, 1.0, 1.0f);
    }

    Framebuffer framebuf;
    Resource<Shader> shader;
    glm::mat4 pvm_matrix;
    glm::mat4 model_matrix = glm::mat4(1.0f);
    glm::vec3 cam_pos = glm::vec3(3, 10, 5);

    struct light {
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    } light_data;

    glm::vec3 obj_color;
};

static texture_cache cache;

texture_t create_texture() {
    texture_t handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Fill with empty data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, max_preview_size, max_preview_size,
                 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    return handle;
}

void send_light_data(render_info& info) {
    info.shader->set_vec3(2, info.obj_color);
    info.shader->set_vec3(3, info.light_data.direction);
    info.shader->set_vec3(4, info.light_data.ambient);
    info.shader->set_vec3(5, info.light_data.diffuse);
    info.shader->set_vec3(6, info.light_data.specular);
}

static void render_mesh(render_info& info, Mesh& mesh) {
    if (!info.shader.is_loaded()) { return; }

    auto& vertices = mesh.get_vertices();
    VertexArray::bind(vertices);
    glDrawElements(GL_TRIANGLES, vertices.index_size(), GL_UNSIGNED_INT,
                   nullptr);
    glBindTexture(GL_TEXTURE_2D, info.framebuf.get_texture());
    // Don't forget to generate mipmap to allow resizing
    glGenerateMipmap(GL_TEXTURE_2D);
}

static void render_char(Font::glyph_data const& data,
                        Framebuffer& target,
                        VertexArray& quad,
                        Shader& shader,
                        glm::vec2 position,
                        glm::vec2 size,
                        glm::vec2 offset) {

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, data.texture);
    shader.set_int(Shader::Uniforms::Texture, 0);
    glm::vec2 img_dim = glm::vec2(data.size.x, data.size.y);
    glm::vec2 target_dim = glm::vec2(target.get_size().x, target.get_size().y);
    shader.set_vec2(0, glm::vec2(img_dim.x / target_dim.x * size.x,
                                 img_dim.y / target_dim.y * size.y));
    shader.set_vec2(1, position);
    shader.set_vec2(2, offset);

    glDrawElements(GL_TRIANGLES, quad.index_size(), GL_UNSIGNED_INT, nullptr);
}

static void render_font(Framebuffer& target,
                        Font& font,
                        VertexArray& quad,
                        Shader& shader) {
    Framebuffer::bind(target);
    glViewport(0, 0, target.get_size().x, target.get_size().y);
    Shader::bind(shader);
    VertexArray::bind(quad);

    shader.set_vec3(3, {1, 1, 1});

    glm::vec2 offset = glm::vec2(0, 0);
    static const std::string text = "Example";
    for (auto const& c : text) {
        if (c == 0) { break; };
        Font::glyph_data const& data = font.glyphs[c];
        glm::vec2 offset_pos =
            offset +
            glm::vec2(data.bearing.x, data.pixel_size - data.bearing.y);
        glm::vec2 relative_offset =
            glm::vec2(offset_pos.x / target.get_size().x,
                      offset_pos.y / target.get_size().y);
        render_char(data, target, quad, shader, {glm::vec2(0.4, 0.4)},
                    glm::vec2(0.1, 0.1), relative_offset);
        // shift by 6 to get the offset in pixels
        offset.x += (data.advance >> 6);
    }
}

} // namespace impl

impl::texture_t render_mesh_preview(AssetManager<Mesh>::Asset& asset) {

    // Optimizations for this:
    // - Send PVM matrix only once
    // - Send lighting data only once

    static impl::render_info info;

    // If the asset is already in the cache, return it
    if (auto tex = impl::cache.find(asset.path.generic_string());
        tex != impl::cache.end()) {
        return tex->second;
    }

    // Texture is not in cache, render it and add to the cache

    // Setup render target
    impl::texture_t texture = impl::create_texture();
    info.framebuf.assign_texture(texture);

    // TODO: Make sure this only has to happen once for a performance boost
    Framebuffer::bind(info.framebuf);
    Shader::bind(*info.shader);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, max_preview_size, max_preview_size);
    send_light_data(info);
    info.pvm_matrix = impl::get_pv_matrix(info.cam_pos) * info.model_matrix;
    info.shader->set_mat4(0, info.pvm_matrix);
    info.shader->set_mat4(1, info.model_matrix);
    info.shader->set_vec3(7, info.cam_pos);

    // Render mesh to target
    impl::render_mesh(info, *asset.ptr);

    // Add to cache
    impl::cache.try_emplace(asset.path.generic_string(), texture);

    return texture;
}

impl::texture_t render_model_preview(AssetManager<Model>::Asset& asset) {
    static impl::render_info info;

    // If the asset is already in the cache, return it
    if (auto tex = impl::cache.find(asset.path.generic_string());
        tex != impl::cache.end()) {
        return tex->second;
    }

    // Texture is not in cache, render it and add to the cache

    // Setup render target
    impl::texture_t texture = impl::create_texture();
    info.framebuf.assign_texture(texture);

    // TODO: Make sure this only has to happen once for a performance boost
    Framebuffer::bind(info.framebuf);
    Shader::bind(*info.shader);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, max_preview_size, max_preview_size);
    send_light_data(info);
    info.pvm_matrix = impl::get_pv_matrix(info.cam_pos) * info.model_matrix;
    info.shader->set_mat4(0, info.pvm_matrix);
    info.shader->set_mat4(1, info.model_matrix);
    info.shader->set_vec3(7, info.cam_pos);

    // Render model to target
    for (auto& mesh : asset.ptr->meshes) { impl::render_mesh(info, mesh); }
    // Add to cache
    impl::cache.try_emplace(asset.path.generic_string(), texture);

    return texture;
}

void clear_cache() { impl::cache.clear(); }

} // namespace Saturn::Editor::previews