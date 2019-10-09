#include "Editor/PreviewRendering.hpp"

#include "Subsystems/Renderer/Framebuffer.hpp"
#include "Subsystems/Renderer/OpenGL.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"

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
    glm::vec3 cam_pos =
        glm::vec3(3, -3, 5); // I fucked up y pos somewhere and it's flipped, so
                             // here's a terrible hack

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

void render_mesh(render_info& info, AssetManager<Mesh>::Asset& asset) {
    if (!info.shader.is_loaded()) { return; }

    // TODO: Make sure this only has to happen once for a performance boost
    Framebuffer::bind(info.framebuf);
    Shader::bind(*info.shader);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, max_preview_size, max_preview_size);
    send_light_data(info);
    info.pvm_matrix = get_pv_matrix(info.cam_pos) * info.model_matrix;
    info.shader->set_mat4(0, info.pvm_matrix);
    info.shader->set_mat4(1, info.model_matrix);
    info.shader->set_vec3(7, info.cam_pos);
    auto& vertices = asset.ptr->get_vertices();
    VertexArray::bind(vertices);
    glDrawElements(GL_TRIANGLES, vertices.index_size(), GL_UNSIGNED_INT,
                   nullptr);
    glBindTexture(GL_TEXTURE_2D, info.framebuf.get_texture());
	glGenerateMipmap(GL_TEXTURE_2D);
	
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

    // Render mesh to target
    impl::render_mesh(info, asset);

    // Add to cache
    impl::cache.try_emplace(asset.path.generic_string(), texture);

    return texture;
}

void clear_cache() {
	impl::cache.clear();
}


} // namespace Saturn::Editor::previews
