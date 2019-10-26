#ifndef SATURN_SKYBOX_PASS_HPP_
#define SATURN_SKYBOX_PASS_HPP_

#include "RenderModule.hpp"

#include "Renderer/VertexArray.hpp"
#include "Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class SkyboxPass : public RenderModule {
public:
    SkyboxPass();

    std::string_view str_id() const override { return "SkyboxPass"; }
    std::string_view description() const override {
        return "Category: Render module\n\n"
               "This module renders the skybox. You can safely "
               "remove this to disable skybox rendering.";
    }

    void init() override;

    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

private:
	VertexArray skybox_vao;
	Resource<Shader> skybox_shader;
};

} // namespace Saturn::RenderModules

#endif // ifndef SATURN_SKYBOX_PASS_HPP_
