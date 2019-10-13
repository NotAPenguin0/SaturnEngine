#include "Renderer/Modules/DepthMapPass.hpp"

#include "AssetManager/AssetManager.hpp"
#include "Renderer/RenderUtils.hpp"
#include "Renderer/Viewport.hpp"

using namespace Saturn::Components;

namespace Saturn::RenderModules {

DepthMap* DepthMapPass::last_depthmap = nullptr;

// The depth map pass is the first stage
DepthMapPass::DepthMapPass() : PreRenderStage(0) {}

void DepthMapPass::init() {
    // Create depth map
    DepthMap::CreateInfo info;
    info.dimensions = {Precision, Precision};
    depth_map.assign(info);
    // Create depth shader
    depth_shader =
        AssetManager<Shader>::get_resource("config/resources/shaders/depth_map.sh", true);
    last_depthmap = &depth_map;
}

void DepthMapPass::process(Scene& scene) {
    setup_for_render();

    // Obtain lightspace matrix
    glm::mat4 lightspace = get_lightspace_matrix(scene);

    for (auto [transform, mesh] :
         scene.get_ecs().select<Transform, StaticMesh>()) {

        if (!mesh.mesh.is_loaded()) { continue; }

        // Send model matrix
        send_model_matrix(depth_shader.get(), transform);

        // Do the rendering
        auto& vtx_array = mesh.mesh->get_vertices();
        VertexArray::bind(vtx_array);
        Shader::bind(depth_shader.get());
        depth_shader->set_mat4(Shader::Uniforms::LightSpaceMatrix, lightspace);
        glDrawElements(GL_TRIANGLES, vtx_array.index_size(), GL_UNSIGNED_INT,
                       nullptr);
    }

    reset();
}

DepthMapPass::~DepthMapPass() { last_depthmap = nullptr; }

void DepthMapPass::setup_for_render() {
    // Bind the target framebuffer and texture
    DepthMap::bind_framebuffer(depth_map);
    // GL_TEXTURE2 is reserved for the depth map by Saturn
    glActiveTexture(GL_TEXTURE2);
    DepthMap::bind_texture(depth_map);

    // Clear before beginning
    glClear(GL_DEPTH_BUFFER_BIT);
    // Create a viewport for the depth map and set it to be the active one
    Viewport vp = Viewport(0, 0, Precision, Precision);
    Viewport::set_active(vp);
    // Set face culling mode to front
    glCullFace(GL_FRONT);
}

void DepthMapPass::reset() {
    // Reset cull face
    glCullFace(GL_BACK);

    // Unbind texture
    DepthMap::unbind_texture();
    glActiveTexture(GL_TEXTURE0);

    // The active framebuffer will be taken care of in the main renderer calling
    // all the modules
}

} // namespace Saturn::RenderModules
