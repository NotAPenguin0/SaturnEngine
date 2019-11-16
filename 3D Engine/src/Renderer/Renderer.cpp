#include "Renderer/Renderer.hpp"

#include "Core/Application.hpp"
#include "ECS/Components.hpp"
#include "Logging/LogSystem.hpp"
#include "Math/Math.hpp"
#include "Renderer/PostProcessing.hpp"
#include "Renderer/RenderUtils.hpp"
#include "Scene/Scene.hpp"
#include "Utility/Exceptions.hpp"
#include "Utility/Utility.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn {

void Renderer::setup_framebuffer(CreateInfo const& create_info) {
    /*
        Framebuffer::CreateInfo framebuf_info;
        framebuf_info.size = {800, 600};
        framebuf.assign(framebuf_info);

        screen_tex = nullptr;*/
}

void Renderer::create_default_viewport(CreateInfo const& create_info) {}

void Renderer::initialize_postprocessing() {
    PostProcessing::get_instance().load_shaders(
        "config/resources/shaders/postprocessing/postprocessing_effects.ppe");
    PostProcessing::get_instance().set_active("none");
}

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

    //    setup_framebuffer(create_info);
    //    create_default_viewport(create_info);
    initialize_postprocessing();
	viewport_targets.reserve(16);
    glDepthFunc(GL_LEQUAL);
}

Renderer::~Renderer() {}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {

    for (size_t i = 0; i < viewports.size(); ++i) {
        Framebuffer::bind(viewport_targets[i]);
        glClearColor(clear_color.r, clear_color.g, clear_color.b,
                     clear_color.a);
        glClear(flags);

        blit_pass.clear(
            {clear_color.r, clear_color.g, clear_color.b, clear_color.a},
            flags);
    }
}

void Renderer::render_scene(Scene& scene) {
    // Render every viewport
    size_t vp_id = 0;
    for (auto& vp : viewports) {
        if (!vp.has_camera()) {
            ++vp_id;
            continue;
        }
        // First, execute all pre-render stages
        for (auto& stage : pre_render_stages) { stage->process(scene); }

        Framebuffer::bind(viewport_targets[vp_id]);

        render_viewport(scene, vp, vp_id);

        ++vp_id;
    }
}

void Renderer::blit_to_screen(Scene& scene) {
    blit_pass.process(scene, viewport_targets[0]);
}

void Renderer::render_viewport(Scene& scene, Viewport& vp, size_t idx) {
    Viewport::set_active(vp);

    // Process render modules
    for (auto& mod : render_modules) {
        mod->process(scene, vp, viewport_targets[idx]);
    }
}

static unsigned int create_texture(glm::vec2 size) {
    unsigned int handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Fill with empty data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);

    return handle;
}

static void resize_if_needed(Framebuffer& target, glm::vec2 size) {
    if (auto const& framebuf_size = target.get_size();
        framebuf_size.x != (size_t)size.x ||
        framebuf_size.y != (size_t)size.y) {

        unsigned int new_texture = create_texture(size);
        target.delete_texture();
        target.set_size({(size_t)size.x, (size_t)size.y});
        target.assign_texture(new_texture);
    }
}

void Renderer::resize(unsigned int x, unsigned int y) {
    //    resize_if_needed(framebuf, {x, y});
    size_t idx = 0;
    for (auto& vp : viewports) {
        vp = Viewport(vp.get_camera(), 0, 0, x, y);
        resize_if_needed(viewport_targets[idx], {x, y});
        ++idx;
    }
}

Viewport& Renderer::get_viewport(std::size_t index) {

    return viewports[viewport_index.at(index)];
}

std::size_t Renderer::add_viewport(Viewport vp) {
    viewports.push_back(vp);
    Framebuffer::CreateInfo framebuf_info;
    framebuf_info.size = {vp.dimensions().x, vp.dimensions().y};
	
    viewport_targets.emplace_back();
    auto& vp_buf = viewport_targets[viewports.size() - 1];
    vp_buf.assign(framebuf_info);
    auto id = IDGenerator<Viewport>::next();
    viewport_index[id] = viewports.size() - 1;
    viewports.back().id = id;
    return id;
}

void Renderer::remove_viewport(size_t id) {
    auto idx = viewport_index[id];
    // Algorithm for erasing and updating indices:
    /*
    1. Swap element to erase with last element
    2. Update index for swapped elements
    3. Remove erased id from index map
    4. Erase
    */

    // Setup for 1 and 2
    auto id_to_update = viewports.back().id;
    auto erased_idx = viewport_index.at(id);
    std::swap(viewports[erased_idx], viewports.back());
	std::swap(viewport_targets[erased_idx], viewport_targets.back());
    viewport_index[id_to_update] = erased_idx;
    viewport_index.erase(id);
    viewports.erase(viewports.end() - 1);
	viewport_targets.erase(viewport_targets.end() - 1);
}

void Renderer::add_pre_render_stage(
    std::unique_ptr<RenderModules::PreRenderStage> stage) {

    pre_render_stages.push_back(std::move(stage));
    // Initialize the render stage
    pre_render_stages.back()->init();
    // re-sort the render stages
    std::sort(pre_render_stages.begin(), pre_render_stages.end(),
              [](auto const& lhs, auto const& rhs) { return *lhs < *rhs; });
}

void Renderer::add_render_module(
    std::unique_ptr<RenderModules::RenderModule> module) {
    render_modules.push_back(std::move(module));
    // Initialize the render stage
    render_modules.back()->init();
    // re-sort the render stages
    std::sort(render_modules.begin(), render_modules.end(),
              [](auto const& lhs, auto const& rhs) { return *lhs < *rhs; });
}

} // namespace Saturn
