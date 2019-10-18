#ifndef SATURN_POST_RENDER_STAGE_HPP_
#define SATURN_POST_RENDER_STAGE_HPP_

#include "RenderStage.hpp"

#include "../Framebuffer.hpp"
#include "Scene/Scene.hpp"

namespace Saturn::RenderModules {

class PostRenderStage : public RenderStage {
public:
    PostRenderStage() = default;
    PostRenderStage(size_t ordering) : RenderStage(ordering) {}
    // Called when the module is added to the renderer
    virtual void init() {}
    // Called every frame. The framebuffer parameter here is the source
    // framebuffer. A post renderstage should write to the framebuffer it stores
    // itself.
    virtual void process(Scene& scene, Framebuffer& source) = 0;

	virtual void clear(glm::vec4 color, GLenum flags) {
		Framebuffer::bind(target);
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(flags);
	}

    Framebuffer& get_framebuffer() { return target; }

    virtual ~PostRenderStage() = default;

protected:
    Framebuffer target;
};

} // namespace Saturn::RenderModules

#endif
