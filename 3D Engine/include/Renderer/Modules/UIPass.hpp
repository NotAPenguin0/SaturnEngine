#ifndef SATURN_UI_PASS_MODULE_HPP_
#define SATURN_UI_PASS_MODULE_HPP_

#include "Renderer/Modules/PostRenderStage.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/VertexArray.hpp"

#include "AssetManager/Resource.hpp"

namespace Saturn::RenderModules {

class UIPass : public PostRenderStage {
public:
    UIPass();

    std::string_view str_id() const override { return "UIPass"; }
    std::string_view description() const override {
        return "Category: Post-render stage.\n"
               "This stage renders all game UI.";
    }

    void init() override;
    void process(Scene& scene, Framebuffer& source) override;

private:
    VertexArray quad;
    Resource<Shader> ui_shader;
    Resource<Shader> blit_shader;
	Resource<Shader> img_shader;
	Resource<Shader> text_shader;
    Framebuffer ui_buffer;
};

} // namespace Saturn::RenderModules

#endif // ifndef SATURN_UI_PASS_MODULE_HPP_
