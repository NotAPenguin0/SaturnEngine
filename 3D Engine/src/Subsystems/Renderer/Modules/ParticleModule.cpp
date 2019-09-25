#include "Subsystems/Renderer/Modules/ParticleModule.hpp"

#include "Subsystems/AssetManager/AssetManager.hpp"
#include "Subsystems/Renderer/Texture.hpp"

namespace Saturn::RenderModules {

// After the transfer module has been processed, we can process the particle
// module
ParticleModule::ParticleModule() : RenderModule(1) {}

void ParticleModule::init() {
    particle_shader =
        AssetManager<Shader>::get_resource("resources/shaders/particle.sh");
    default_texture =
        AssetManager<Texture>::get_resource("resources/textures/white.tex");
}

void ParticleModule::process(Scene& scene,
                             Viewport& viewport,
                             Framebuffer& target) {
    using namespace Components;
    Shader::bind(particle_shader.get());

    glDisable(GL_CULL_FACE);
    for (auto [emitter] : scene.get_ecs().select<ParticleEmitter>()) {
        if (emitter.additive) { glBlendFunc(GL_SRC_ALPHA, GL_ONE); }
        // Bind VAO
        VertexArray::bind(emitter.particle_vao.get());

        auto& texture = emitter.texture.is_loaded() ? emitter.texture.get()
                                                    : default_texture.get();

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

} // namespace Saturn::RenderModules
