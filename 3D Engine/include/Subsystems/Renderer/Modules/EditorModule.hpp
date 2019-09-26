#ifndef SATURN_EDITOR_MODULE_HPP_
#define SATURN_EDITOR_MODULE_HPP_

#include "RenderModule.hpp"

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class EditorModule : public RenderModule {
public:
    EditorModule();

	std::string_view str_id() const override { return "EditorModule"; }

    void init() override;

    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

private:

	void render_colliders(Scene& scene);
    void render_axes();
    void render_outlines(Scene& scene);

    Resource<Shader> collider_shader;
    Resource<Shader> axis_shader;
    Resource<Shader> outline_shader;

	Resource<Mesh> box_collider_mesh;
	Resource<Mesh> line_mesh;
};

} // namespace Saturn::RenderModules

#endif
