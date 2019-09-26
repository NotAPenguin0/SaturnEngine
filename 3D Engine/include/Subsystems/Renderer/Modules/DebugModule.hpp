#ifndef SATURN_DEBUG_MODULE_HPP_
#define SATURN_DEBUG_MODULE_HPP_

#include "RenderModule.hpp"

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class DebugModule : public RenderModule {
public:
    DebugModule();

    std::string_view str_id() const override { return "DebugModule"; }
    std::string_view description() const override {
        return "Category: Render module\n\n"
               "This module takes care of potential debug rendering. You "
               "should probably disable this module in release builds";
    }

    void init() override;
    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;
};

} // namespace Saturn::RenderModules

#endif
