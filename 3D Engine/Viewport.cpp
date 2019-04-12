#include "Viewport.hpp"

#include "OpenGL.hpp"

namespace Saturn {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow-field-in-constructor"

Viewport::Viewport(unsigned int x,
                   unsigned int y,
                   unsigned int w,
                   unsigned int h) :
    x(x),
    y(y), w(w), h(h) {}

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

Viewport Viewport::current() {
    int viewport[4];
	// Query OpenGL for current viewport information
    glGetIntegerv(GL_VIEWPORT, viewport);
    return Viewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

#pragma clang diagnostic pop

} // namespace Saturn