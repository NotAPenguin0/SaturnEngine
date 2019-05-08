#ifndef MVG_RENDERER_HPP_
#define MVG_RENDERER_HPP_

#include "Framebuffer.hpp"
#include "SceneGraph.hpp"
#include "Shader.hpp"
#include "Utility.hpp"
#include "VertexArray.hpp"
#include "Viewport.hpp"
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

    void render_scene_graph(SceneGraph const& scene);

    void update_screen();

    void debug_log_viewport() const;

    // \brief Returns a reference to the viewport with specified index.
    // \param index: The index of the viewport to return. Viewport 0 is
    // initialized to be the full window
    Viewport& get_viewport(std::size_t index);

    // \brief Adds a viewport to the renderer
    // \param vp: The viewport to add
    // \return The index of the newly added viewport
    std::size_t add_viewport(Viewport vp);

private:
    std::reference_wrapper<Application> app;
    WindowDim screen_size;
    Framebuffer framebuf;
    ///< default constructed framebuffer means screen
    Framebuffer screen_framebuf;

    VertexArray screen;

    Shader default_shader;
    std::vector<Viewport> viewports;
};

} // namespace Saturn

#endif
