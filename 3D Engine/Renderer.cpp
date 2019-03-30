#include "Renderer.hpp"

#include "Application.hpp"

namespace Saturn {

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {

    Framebuffer::CreateInfo framebuffer_create_info;
    framebuffer_create_info.size = screen_size;
    framebuf = Framebuffer(framebuffer_create_info);
}

void Renderer::clear(
    Color clear_color,
    GLenum flags /*= GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT*/) {
    Framebuffer::bind(framebuf);
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(flags);
    Framebuffer::unbind(framebuf);
}

void Renderer::render_to_screen() {
	throw std::runtime_error("Renderer::render_to_screen(): Function is not implemented yet");
}

} // namespace Saturn
