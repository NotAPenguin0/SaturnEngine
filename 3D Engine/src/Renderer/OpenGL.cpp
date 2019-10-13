#include "Renderer/OpenGL.hpp"

namespace Saturn {

Vao::Vao() { glGenVertexArrays(1, &id); }

Vao::~Vao() {
    if (id != 0) { glDeleteVertexArrays(1, &id); }
}

void Vao::bind(Vao& vao) { glBindVertexArray(vao.id); }

void Vao::unbind() { glBindVertexArray(0); }

} // namespace Saturn
