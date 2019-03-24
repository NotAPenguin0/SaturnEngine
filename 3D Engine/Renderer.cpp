#include "Renderer.hpp"

#include "Application.hpp"

namespace Saturn {

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(flags);
}

} // namespace Saturn
