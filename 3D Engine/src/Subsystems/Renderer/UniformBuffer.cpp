#include "Subsystems/Renderer/UniformBuffer.hpp"

#include "Subsystems/Renderer/OpenGL.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace Saturn {

UniformBuffer::UniformBuffer() : ubo(0) {}

UniformBuffer::UniformBuffer(CreateInfo const& create_info) {
    glGenBuffers(1, &ubo);

    bind(*this);
    glBufferData(GL_UNIFORM_BUFFER, create_info.size_in_bytes, nullptr,
                 create_info.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    unbind();

    // Link our buffer to the requested binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, create_info.binding_point, ubo);
}

UniformBuffer::~UniformBuffer() {
    if (ubo != 0) glDeleteBuffers(1, &ubo);
}

void UniformBuffer::bind(UniformBuffer& buf) {
    glBindBuffer(GL_UNIFORM_BUFFER, buf.ubo);
}

void UniformBuffer::unbind() { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

void UniformBuffer::assign(CreateInfo const& create_info) {
    if (ubo != 0) glDeleteBuffers(1, &ubo);

    glGenBuffers(1, &ubo);

    bind(*this);
    glBufferData(GL_UNIFORM_BUFFER, create_info.size_in_bytes, nullptr,
                 create_info.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    unbind();

    // Link our buffer to the requested binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, create_info.binding_point, ubo);
}

void UniformBuffer::set_int(int value, std::size_t byte_offset) {
    glBufferSubData(GL_UNIFORM_BUFFER, byte_offset, sizeof(int), &value);
}

void UniformBuffer::set_float(float value, std::size_t byte_offset) {
    glBufferSubData(GL_UNIFORM_BUFFER, byte_offset, sizeof(float), &value);
}

void UniformBuffer::set_vec3(glm::vec3 const& value, std::size_t byte_offset) {
    glBufferSubData(GL_UNIFORM_BUFFER, byte_offset, sizeof(glm::vec3),
                    glm::value_ptr(value));
}

void UniformBuffer::set_mat4(glm::mat4 const& value, std::size_t byte_offset) {
    glBufferSubData(GL_UNIFORM_BUFFER, byte_offset, sizeof(glm::mat4),
                    glm::value_ptr(value));
}

} // namespace Saturn
