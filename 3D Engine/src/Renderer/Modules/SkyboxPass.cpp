#include "Renderer/Modules/SkyboxPass.hpp"

#include "AssetManager/AssetManager.hpp"
#include "ECS/Components/Camera.hpp"

namespace Saturn::RenderModules {

namespace impl {
std::vector<float> skybox_vertices = {
    // positions only
    -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
    -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

    1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
    1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};
}

SkyboxPass::SkyboxPass() : RenderModule(100) {}

void SkyboxPass::init() {
    VertexArray::CreateInfo info;
    info.vertices = impl::skybox_vertices;
    info.dynamic = false;
    info.attributes.push_back({0, 3}); // Only positions
	skybox_vao.assign(info);

    skybox_shader = AssetManager<Shader>::get_resource(
        "config/resources/shaders/skybox.sh", true);
}

void SkyboxPass::process(Scene& scene,
                         Viewport& viewport,
                         Framebuffer& target) {
    using namespace Components;

    glDepthMask(GL_FALSE);
    Framebuffer::bind(target);
    Shader::bind(*skybox_shader);
    VertexArray::bind(skybox_vao);

    auto& cam = scene.get_ecs().get_with_id<Camera>(viewport.get_camera());

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (cam.skybox.is_loaded()) {
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cam.skybox->handle);
		skybox_shader->set_int(0, 0);
		glDrawElements(GL_TRIANGLES, skybox_vao.index_size(), GL_UNSIGNED_INT, nullptr);
	}
	
	glDisable(GL_BLEND);

    glDepthMask(GL_TRUE);
}

} // namespace Saturn::RenderModules
