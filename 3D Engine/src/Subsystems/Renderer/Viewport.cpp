#include "Subsystems/Renderer/Viewport.hpp"

#include "Subsystems/Renderer/OpenGL.hpp"

#include <cstddef>

namespace Saturn {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow-field-in-constructor"

Viewport::Viewport(unsigned int x,
                   unsigned int y,
                   unsigned int w,
                   unsigned int h) :
    x(x),
    y(y), w(w), h(h) {}

Viewport::Viewport(std::size_t cam,
                   unsigned int x,
                   unsigned int y,
                   unsigned int w,
                   unsigned int h) :

    x(x),
    y(y), w(w), h(h), camera(cam) {}

#pragma clang diagnostic pop

WindowDim Viewport::dimensions() const { return {w, h}; }
WindowDim Viewport::position() const { return {x, y}; }

void Viewport::resize(unsigned int new_w, unsigned int new_h) {
    w = new_w;
    h = new_h;
}

void Viewport::move(unsigned int new_x, unsigned int new_y) {
    x = new_x;
    y = new_y;
}

void Viewport::set_active(Viewport const& viewport) {
    glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wc++11-narrowing"

std::size_t Viewport::get_camera() const { return camera; }
void Viewport::set_camera(std::size_t cam_id) { camera = cam_id; }

bool Viewport::has_camera() const {
    return camera != static_cast<std::size_t>(-1);
}

#pragma clang diagnostic pop

} // namespace Saturn
