#ifndef SATURN_RENDER_MODULE_HPP_
#define SATURN_RENDER_MODULE_HPP_

#include "RenderStage.hpp"

#include "../Framebuffer.hpp"
#include "../Viewport.hpp"
#include "Subsystems/Scene/Scene.hpp"

namespace Saturn::RenderModules {

class RenderModule : public RenderStage {
public:
    RenderModule() = default;
    RenderModule(size_t ordering) : RenderStage(ordering) {}
    // Called when the render module is added to the renderer
    virtual void init() {}
    // Called every frame, in the correct order
    virtual void process(Scene& scene, Viewport& viewport, Framebuffer& target) = 0;
    // Called at the end of each frame, again in the rendering order
    virtual void end_frame(Scene& scene, Framebuffer& target) {}

    virtual ~RenderModule() = default;
};

} // namespace Saturn::RenderModules

#endif
