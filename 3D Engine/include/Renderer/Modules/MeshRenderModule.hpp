#ifndef SATURN_MESH_RENDER_MODULE_HPP_
#define SATURN_MESH_RENDER_MODULE_HPP_

#include "RenderModule.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Shader.hpp"

namespace Saturn::RenderModules {

class MeshRenderModule : public RenderModule {
public:
    MeshRenderModule();

    std::string_view str_id() const override { return "MeshRenderModule"; }
    std::string_view description() const override {
        return "Category: Render module\n\n"
               "This module renders all meshes in the scene. You can safely "
               "remove this to disable mesh rendering.";
    }

    void init() override;

    void
    process(Scene& scene, Viewport& viewport, Framebuffer& target) override;

private:
    void render_mesh(Scene& scene,
                     Components::Transform& relative_transform,
                     Mesh& mesh,
                     Material& material,
                     bool face_cull);

    void unbind_textures(Material& material);
    void send_material_data(Scene& scene, Shader& shader, Material& material);

    Resource<Shader> no_shader_error;
};

} // namespace Saturn::RenderModules

#endif
