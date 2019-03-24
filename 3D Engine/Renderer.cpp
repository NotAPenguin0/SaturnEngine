#include "Renderer.hpp"

#include "Application.hpp"

namespace Saturn {

Renderer::Renderer(CreateInfo create_info) :
    app(create_info.app), screen_size(create_info.screen_size) {}

} // namespace Saturn

void Renderer::clear(Color clear_color) {

}