#ifndef SATURN_BLIT_PASS_HPP_
#define SATURN_BLIT_PASS_HPP_

#include "PostRenderStage.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

namespace Saturn::RenderModules {

// The BlitPass render stage defaults the target framebuffer to be the screen
// framebuffer
class BlitPass : public PostRenderStage {
public:
    BlitPass();

    std::string_view str_id() const override { return "BlitPass"; }
    std::string_view description() const override {
        return "Category: Post-render stage.\n"
               "\n"
               "This is the final stage in the whole rendering pipeline, and "
               "it will write the "
               "final framebuffer after rendering and postprocessing to the "
               "screen. Note that "
               "removing this render stage will remove ALL rendering, "
               "including the editor rendering. "
               "do NOT remove this render stage.";
    }

    void init() override;
    void process(Scene& scene, Framebuffer& source) override;

    ~BlitPass() = default;

private:
    VertexArray screen_vao;
    Resource<Shader> blit_shader;
};

} // namespace Saturn::RenderModules

#endif
