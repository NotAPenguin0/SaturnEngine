#include "Renderer/Modules/MeshRenderModule.hpp"

#include "Renderer/Modules/DepthMapPass.hpp"

#include "AssetManager/AssetManager.hpp"
#include "Renderer/RenderUtils.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn::RenderModules {

using namespace Components;

MeshRenderModule::MeshRenderModule() : RenderModule(10) {}

void MeshRenderModule::init() {

    no_shader_error = AssetManager<Shader>::get_resource(
        "config/resources/shaders/default.sh", true);
}

void MeshRenderModule::process(Scene& scene,
                               Viewport& viewport,
                               Framebuffer& target) {

    for (auto [relative_transform, mesh, material] :
         scene.get_ecs().select<Transform, StaticMesh, Material>()) {

        if (!mesh.mesh.is_loaded()) { continue; }

        auto& shader = material.shader.is_loaded() ? material.shader.get()
                                                   : no_shader_error.get();

        // Send data to shader
        send_model_matrix(shader, relative_transform);
        send_material_data(scene, shader, material);

        // Set lightspace matrix in shader
        Shader::bind(shader);
        if (material.lit) {
            auto lightspace = get_lightspace_matrix(scene);
            shader.set_mat4(Shader::Uniforms::LightSpaceMatrix, lightspace);
            if (RenderModules::DepthMapPass::last_depthmap) {
                // Set shadow map in shader
                glActiveTexture(GL_TEXTURE2);
                DepthMap::bind_texture(
                    *RenderModules::DepthMapPass::last_depthmap);
                shader.set_int(Shader::Uniforms::DepthMap, 2);
            }
        }

        // Do the actual rendering (maybe put this in another function
        // render_mesh() or something)
        auto& vtx_array = mesh.mesh->get_vertices();

        VertexArray::bind(vtx_array);
        if (!mesh.face_cull) { glDisable(GL_CULL_FACE); }
        glDrawElements(GL_TRIANGLES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
        if (!mesh.face_cull) { glEnable(GL_CULL_FACE); }
        // Cleanup
        unbind_textures(material);
        glActiveTexture(GL_TEXTURE2);
        DepthMap::unbind_texture();
    }
}

void MeshRenderModule::unbind_textures(Material& material) {
    if (material.lit) {
        if (material.diffuse_map.is_loaded()) {
            Texture::unbind(*material.diffuse_map);
        }
        if (material.specular_map.is_loaded()) {
            Texture::unbind(*material.specular_map);
        }
        if (material.normal_map.is_loaded()) {
            Texture::unbind(*material.normal_map);
        }
    }
}

void MeshRenderModule::send_material_data(Scene& scene,
                                          Shader& shader,
                                          Material& material) {
    Shader::bind(shader);
    if (material.lit) {
        if (material.diffuse_map.is_loaded()) {
            Texture::bind(*material.diffuse_map);
            shader.set_int(Shader::Uniforms::Material::DiffuseMap,
                           material.diffuse_map->unit() - GL_TEXTURE0);
        }
        if (material.specular_map.is_loaded()) {
            Texture::bind(*material.specular_map);
            shader.set_int(Shader::Uniforms::Material::SpecularMap,
                           material.specular_map->unit() - GL_TEXTURE0);
        }
        if (material.normal_map.is_loaded()) {
            Texture::bind(*material.normal_map);
            shader.set_int(Shader::Uniforms::Material::NormalMap,
                           material.normal_map->unit() - GL_TEXTURE0);
        }
        shader.set_float(Shader::Uniforms::Material::Shininess,
                         material.shininess);
    }

    if (material.reflective) {
        // Get the skybox texture
        CubeMap* skybox = nullptr;
        for (auto [cam] : scene.get_ecs().select<Camera>()) {
			// Grab the first skybox we can get and exit
            if (cam.skybox.is_loaded()) {
                skybox = &cam.skybox.get();
                break;
            }
        }
        // Skybox has texture unit 4
        if (skybox) { 
			glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->handle);
			shader.set_int(Shader::Uniforms::Material::Skybox, 4); 
		}
    }

    Shader::unbind();
}

} // namespace Saturn::RenderModules
