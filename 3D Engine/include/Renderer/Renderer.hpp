#ifndef SATURN_RENDERER_HPP_
#define SATURN_RENDERER_HPP_

#include "AssetManager/AssetManager.hpp"
#include "ECS/Components.hpp"
#include "Scene/Scene.hpp"

#include "DepthMap.hpp"
#include "Framebuffer.hpp"
#include "UniformBuffer.hpp"
#include "Utility/Utility.hpp"
#include "VertexArray.hpp"
#include "Viewport.hpp"

#include "Modules/BlitPass.hpp"
#include "Modules/PostRenderStage.hpp"
#include "Modules/PreRenderStage.hpp"
#include "Modules/RenderModule.hpp"

#include "glad/glad.h"

#include <functional>

namespace Saturn {

class Application;

class Renderer {
public:
    struct CreateInfo {
        WindowDim screen_size;
        Application& app;
    };

    Renderer(CreateInfo create_info);

    Renderer(Renderer const&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer const& other) = delete;
    Renderer& operator=(Renderer&& other) = delete;

    ~Renderer();

    void clear(Color clear_color,
               GLenum flags = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    void render_scene(Scene& scene);

    // /brief Returns a reference to the viewport with specified index.
    // /param index: The index of the viewport to return. Viewport 0 is
    // initialized to be the full window
    Viewport& get_viewport(std::size_t index);

    // /brief Adds a viewport to the renderer
    // /param vp: The viewport to add
    // /return The index of the newly added viewport
    std::size_t add_viewport(Viewport vp);

	void remove_viewport(size_t index);

    // Adds a pre-render stage to the renderer. Takes ownership of the
    // unique_ptr passed in
    void
    add_pre_render_stage(std::unique_ptr<RenderModules::PreRenderStage> stage);
    void add_render_module(std::unique_ptr<RenderModules::RenderModule> module);

    auto& get_pre_render_stages() { return pre_render_stages; }
    auto& get_render_modules() { return render_modules; }

    Framebuffer* get_viewport_framebuf(size_t idx) { return &viewport_targets[viewport_index[idx]]; }

    void resize(unsigned int x, unsigned int y);

    void blit_to_screen(Scene& scene);

private:
    // Initialization
    void setup_framebuffer(CreateInfo const& create_info);
    void create_default_viewport(CreateInfo const& create_info);
    void initialize_postprocessing();

    // Rendering functions
    void render_viewport(Scene& scene, Viewport& vp, size_t idx);

    // Member variables
    std::reference_wrapper<Application> app;
    WindowDim screen_size;
    std::vector<Viewport> viewports;
    std::vector<Framebuffer> viewport_targets;
	std::unordered_map<size_t, size_t> viewport_index;

    std::vector<std::unique_ptr<RenderModules::PreRenderStage>>
        pre_render_stages;
    std::vector<std::unique_ptr<RenderModules::RenderModule>> render_modules;

    RenderModules::BlitPass blit_pass;
};

} // namespace Saturn

#endif
