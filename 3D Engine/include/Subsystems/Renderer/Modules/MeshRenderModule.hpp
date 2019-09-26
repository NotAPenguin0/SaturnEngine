#ifndef SATURN_MESH_RENDER_MODULE_HPP_
#define SATURN_MESH_RENDER_MODULE_HPP_

#include "RenderModule.hpp"

#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class MeshRenderModule : public RenderModule {
public:
    MeshRenderModule();

	std::string_view str_id() const override { return "MeshRenderModule"; }

    void init() override;

    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

private:
	void unbind_textures(Components::Material& material);
    void send_material_data(Shader& shader, Components::Material& material);
    
	Resource<Shader> no_shader_error;
};

} // namespace Saturn::RenderModules

#endif
