#include "Subsystems/Renderer/Renderer.hpp"

#include "Core/Application.hpp"
#include "Subsystems/ECS/Components.hpp"
#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Math/Math.hpp"
#include "Subsystems/Renderer/PostProcessing.hpp"
#include "Subsystems/Renderer/RenderUtils.hpp"
#include "Subsystems/Scene/Scene.hpp"
#include "Utility/Exceptions.hpp"
#include "Utility/Utility.hpp"

// #Temp, testing
#include "Subsystems/Renderer/Modules/BlitPass.hpp"
#include "Subsystems/Renderer/Modules/DebugModule.hpp"
#include "Subsystems/Renderer/Modules/DepthMapPass.hpp"
#include "Subsystems/Renderer/Modules/EditorModule.hpp"
#include "Subsystems/Renderer/Modules/MeshRenderModule.hpp"
#include "Subsystems/Renderer/Modules/ParticleModule.hpp"
#include "Subsystems/Renderer/Modules/TransferModule.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Saturn {

void Renderer::setup_framebuffer(CreateInfo const& create_info) {
    Framebuffer::CreateInfo framebuf_info;
    framebuf_info.size = screen_size;
    framebuf.assign(framebuf_info);
}

void Renderer::create_default_viewport(CreateInfo const& create_info) {
    add_viewport(
        Viewport(0, 0, create_info.screen_size.x, create_info.screen_size.y));
    // Set it as the active viewport
    Viewport::set_active(get_viewport(0));
}

void Renderer::initialize_postprocessing() {
    PostProcessing::get_instance().load_shaders(
        "resources/shaders/postprocessing/postprocessing_effects.ppe");
    PostProcessing::get_instance().set_active("none");
}

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

    setup_framebuffer(create_info);
    create_default_viewport(create_info);
    initialize_postprocessing();

    // #Temp, #RefactorTesting
    add_pre_render_stage(std::make_unique<RenderModules::DepthMapPass>());
    add_render_module(std::make_unique<RenderModules::TransferModule>());
    add_render_module(std::make_unique<RenderModules::ParticleModule>());
    add_render_module(std::make_unique<RenderModules::DebugModule>());
    add_render_module(std::make_unique<RenderModules::EditorModule>());
//    add_render_module(std::make_unique<RenderModules::MeshRenderModule>());
    add_post_render_stage(std::make_unique<RenderModules::BlitPass>());
}

Renderer::~Renderer() {}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {

    Framebuffer::bind(framebuf);

    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(flags);
}

void Renderer::render_scene(Scene& scene) {
    // Render every viewport
    for (auto& vp : viewports) {
        if (!vp.has_camera()) continue;

        // First, execute all pre-render stages
        for (auto& stage : pre_render_stages) {
            Framebuffer::bind(framebuf);
            stage->process(scene);
        }

        Framebuffer::bind(framebuf);

        render_viewport(scene, vp);

        Framebuffer* next_source = &framebuf;
        for (auto& stage : post_render_stages) {
            stage->process(scene, *next_source);
            // Chain post render stages together by having each of them operate
            // on the result of the previous one
            next_source = &stage->get_framebuffer();
        }
    }
}

void Renderer::render_viewport(Scene& scene, Viewport& vp) {
    Viewport::set_active(vp);

    // Process render modules
    for (auto& mod : render_modules) { mod->process(scene, vp, framebuf); }
}

Viewport& Renderer::get_viewport(std::size_t index) {
    if (index >= viewports.size()) {
        throw InvalidViewportException("Invalid viewport with index " +
                                       std::to_string(index) + " requested.");
    }
    return viewports[index];
}

std::size_t Renderer::add_viewport(Viewport vp) {
    viewports.push_back(vp);
    return viewports.size() - 1;
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

void Renderer::add_post_render_stage(
    std::unique_ptr<RenderModules::PostRenderStage> stage) {
    post_render_stages.push_back(std::move(stage));
    // Initialize the render stage
    post_render_stages.back()->init();
    // re-sort the render stages
    std::sort(post_render_stages.begin(), post_render_stages.end(),
              [](auto const& lhs, auto const& rhs) { return *lhs < *rhs; });
}

} // namespace Saturn
