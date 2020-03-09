#include "Renderer/Modules/ParticleModule.hpp"

#include "AssetManager/AssetManager.hpp"
#include "Renderer/Texture.hpp"

namespace Saturn::RenderModules {

// Particles should be rendered after the skybox to get correct blending
ParticleModule::ParticleModule() : RenderModule(200) {}

void ParticleModule::init() {
    particle_shader = AssetManager<Shader>::get_resource(
        "config/resources/shaders/particle.sh", true);
    default_texture = AssetManager<Texture>::get_resource(
        "config/resources/textures/white.tex", true);
}

void ParticleModule::process(Scene& scene,
                             Viewport& viewport,
                             Framebuffer& target) {
    using namespace Components;
    Shader::bind(particle_shader.get());

    glDisable(GL_CULL_FACE);
    for (auto [emitter] : scene.get_ecs().select<ParticleEmitter>()) {
		glEnable(GL_BLEND);
        if (emitter.additive) {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        }
        // Bind VAO
        VertexArray::bind(emitter.particle_vao.get());

        auto& texture = emitter.texture.is_loaded() ? emitter.texture.get()
                                                    : default_texture.get();

        Texture::bind(texture);
        particle_shader->set_int(Shader::Uniforms::Texture,
                                 texture.unit() - GL_TEXTURE0);

        glDrawElementsInstanced(GL_TRIANGLES, emitter.particle_vao->index_size(),
                                GL_UNSIGNED_INT, nullptr,
                                emitter.particles.size());

        Texture::unbind(texture);

        if (emitter.additive) {
            // reset blend function to old one
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
    }
    glEnable(GL_CULL_FACE);
}

} // namespace Saturn::RenderModules