#include "Editor/PreviewRendering.hpp"

#include "Subsystems/Renderer/Framebuffer.hpp"
#include "Subsystems/Renderer/OpenGL.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn::Editor {

namespace impl {

using texture_t = unsigned int;

glm::mat4 get_pvm_matrix() {
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    glm::vec3 cam_pos = glm::vec3(0.0f, 1.2f, 5.0f);
    glm::vec3 cam_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cam_dir = glm::normalize(cam_pos - cam_target);
    glm::vec3 world_up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 cam_right = glm::normalize(glm::cross(world_up, cam_dir));
    glm::vec3 cam_up = glm::cross(cam_dir, cam_right);
    glm::mat4 view = glm::lookAt(cam_pos, cam_target, cam_up);

    glm::mat4 model = glm::mat4(1.0f);
	 
    return projection * view * model;
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
        pvm_matrix = get_pvm_matrix();
    }
    Framebuffer framebuf;
    Resource<Shader> shader;
    glm::mat4 pvm_matrix;
};

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, max_preview_size, max_preview_size,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenerateMipmap(GL_TEXTURE_2D);
    return handle;
}

void render_mesh(render_info& info, AssetManager<Mesh>::Asset& asset) {
    if (!info.shader.is_loaded()) { return; }

    // TODO: Make sure this only has to happen once for a performance boost
    Framebuffer::bind(info.framebuf);
    Shader::bind(*info.shader);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, max_preview_size, max_preview_size);
    info.shader->set_mat4(0, info.pvm_matrix);
    auto& vertices = asset.ptr->get_vertices();
    VertexArray::bind(vertices);
    glDrawElements(GL_TRIANGLES, vertices.index_size(), GL_UNSIGNED_INT,
                   nullptr);
}

} // namespace impl

impl::texture_t render_mesh_preview(AssetManager<Mesh>::Asset& asset) {
    static impl::render_info info;

    // Setup render target
    impl::texture_t texture = impl::create_texture();
    info.framebuf.assign_texture(texture);

    // Render mesh to target
    impl::render_mesh(info, asset);

    return texture;
}

} // namespace Saturn::Editor
