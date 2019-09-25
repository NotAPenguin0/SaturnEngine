#include "Subsystems/Renderer/Modules/BlitPass.hpp"

#include "Core/Application.hpp"
#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/Renderer/Viewport.hpp"

#include <vector>

namespace Saturn::RenderModules {

static std::vector<float> screen_vertices = {
    // Vertices	        Texture coords
    -1.0f, 1.0f,  0.0f, 0.0f, 1.0f, // TL
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // BL
    1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // BR
    1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // TR
};

// The blit pass is always the last render pass
BlitPass::BlitPass() : PostRenderStage(size_t(-1)) {}

void BlitPass::init() {
    std::vector<VertexAttribute> screen_attributes;
    screen_attributes.push_back({0, 3}); // Position is a vec3
    screen_attributes.push_back({1, 2}); // TexCoords is a vec2

    screen_vao.assign({screen_attributes, screen_vertices, {0, 1, 2, 0, 3, 2}});

    blit_shader =
        AssetManager<Shader>::get_resource("resources/shaders/blit.sh");
}

void BlitPass::process(Scene& scene, Framebuffer& source) {
    Framebuffer::bind(target);

    Viewport::set_active(scene.get_app()->get_renderer()->get_viewport(0));

    VertexArray::bind(screen_vao);

    // Temporarily disable some GL functionality
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Enable gamma correction
    glEnable(GL_FRAMEBUFFER_SRGB);

    // Set (postprocessing) shader
    Shader::bind(blit_shader.get());

    // Render framebuffer texture to the screen
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, source.get_texture());

    blit_shader->set_int(Shader::Uniforms::Texture, 0);
    glDrawElements(GL_TRIANGLES, screen_vao.index_size(), GL_UNSIGNED_INT,
                   nullptr);

    // Re enable functionality
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Disable gamma correction
    glDisable(GL_FRAMEBUFFER_SRGB);

    VertexArray::unbind();
}

} // namespace Saturn::RenderModules
