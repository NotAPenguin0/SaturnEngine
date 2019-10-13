#ifndef SATURN_PRE_RENDER_STAGE_HPP_
#define SATURN_PRE_RENDER_STAGE_HPP_

#include "RenderStage.hpp"

#include "Scene/Scene.hpp"

namespace Saturn::RenderModules {

class PreRenderStage : public RenderStage {
public:
	PreRenderStage() = default;
	PreRenderStage(size_t ordering) : RenderStage(ordering) {}
    // Called every time the render stage is re-added to the renderer
    virtual void init() {}
    // Called every frame before the main rendering stages start
    virtual void process(Scene& scene) = 0;
    // Called at the end of a frame
    virtual void end_frame(Scene& scene) {}
    // Cleanup work goes into the destructor obviously
    virtual ~PreRenderStage() = default;
};

} // namespace Saturn::RenderModules

#endif
