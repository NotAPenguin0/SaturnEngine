#ifndef SATURN_PARTICLE_MODULE_HPP_
#define SATURN_PARTICLE_MODULE_HPP_

#include "RenderModule.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class ParticleModule : public RenderModule {
public:
    ParticleModule();

    std::string_view str_id() const override { return "ParticleModule"; }
    std::string_view description() const override {
        return "Category: Render module\n\n"
               "This module renders all particles in the scene. Removing this "
               "will disable any particle rendering.";
    }

    void init() override;
    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

private:
    Resource<Shader> particle_shader;
    Resource<Texture> default_texture;
};

} // namespace Saturn::RenderModules

#endif
