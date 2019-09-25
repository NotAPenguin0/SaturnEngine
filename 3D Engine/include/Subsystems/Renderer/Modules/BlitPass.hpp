#ifndef SATURN_BLIT_PASS_HPP_
#define SATURN_BLIT_PASS_HPP_

#include "PostRenderStage.hpp"

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"
#include "Subsystems/Renderer/VertexArray.hpp"

namespace Saturn::RenderModules {

// The BlitPass render stage defaults the target framebuffer to be the screen
// framebuffer
class BlitPass : public PostRenderStage {
public:
    BlitPass();
    void init() override;
    void process(Scene& scene, Framebuffer& source) override;

private:
    VertexArray screen_vao;
    Resource<Shader> blit_shader;
};

} // namespace Saturn::RenderModules

#endif
