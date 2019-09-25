#ifndef SATURN_DEBUG_MODULE_HPP_
#define SATURN_DEBUG_MODULE_HPP_

#include "RenderModule.hpp"

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class DebugModule : public RenderModule {
public:
    DebugModule();

    void init() override;
    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;
};

} // namespace Saturn::RenderModules

#endif
