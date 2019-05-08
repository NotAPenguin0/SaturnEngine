#include "VertexArray.hpp"

#include "LogSystem.hpp"
#include "OpenGL.hpp"
#include "bind_guard.hpp"

#include <cassert>
#include <numeric>

namespace Saturn {

static std::size_t
get_vertex_size_in_bytes(std::vector<VertexAttribute> const& attributes) {
    std::size_t result = 0;
    for (auto& attr : attributes) {
        result += attr.num_components * sizeof(float);
    }
    return result;
}

VertexArray::VertexArray(CreateInfo const& create_info) {
    do_create(create_info);
}

VertexArray::~VertexArray() {}

void VertexArray::assign(CreateInfo const& create_info) {
    if (vao.id != 0 && vbo.id != 0 && ebo.id != 0) {
        glDeleteVertexArrays(1, &vao.id);
        glDeleteBuffers(1, &vbo.id);
        glDeleteBuffers(1, &ebo.id);
    }

    glGenVertexArrays(1, &vao.id);
    glGenBuffers(1, &vbo.id);
    glGenBuffers(1, &ebo.id);

    do_create(create_info);
}

std::size_t VertexArray::size() const { return vertex_count; }
std::size_t VertexArray::index_size() const { return indices_size; }

void VertexArray::bind(VertexArray& buf) {
    glBindVertexArray(buf.vao.id);
    Ebo::bind(buf.ebo);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
    Ebo::unbind();
}

void VertexArray::do_create(CreateInfo const& create_info) {

    if (vao.id == 0) {
        LogSystem::write(LogSystem::Severity::Error, "Failed to create VAO");
    }
    if (vbo.id == 0) {
        LogSystem::write(LogSystem::Severity::Error, "Failed to create VBO");
    }

    //#MaybeOptimize
    indices = create_info.indices;

    // If the EBO is empty, generate a default one (indices from 0 ... to
    // vertex_count)

    std::size_t vertex_size_in_bytes =
        get_vertex_size_in_bytes(create_info.attributes);

    // The amount of vertices is equal to the total size in bytes of the array
    // divided by the vertex size in bytes

    vertex_count =
        create_info.vertices.size() * sizeof(float) / vertex_size_in_bytes;

    if (indices.empty()) {
        // Make space for N elements (N is vertex count), to fill buffer in next
        // call
        indices.resize(vertex_count);
        // Fill with values ranging 0 ... N where N is the vertex count
        std::iota(indices.begin(), indices.end(), 0);
    }

    indices_size = indices.size();

    bind_guard vao_guard(vao);
    bind_guard vbo_guard(vbo);
    bind_guard ebo_guard(ebo);

    // Fill VBO
    glBufferData(GL_ARRAY_BUFFER, create_info.vertices.size() * sizeof(float),
                 create_info.vertices.data(),
                 create_info.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    // Fill EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 indices.data(),
                 create_info.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    std::size_t offset = 0;
    for (auto& attr : create_info.attributes) {
        assert(attr.num_components >= 1 && attr.num_components <= 4);

        glVertexAttribPointer(attr.location_in_shader, attr.num_components,
                              GL_FLOAT, GL_FALSE, vertex_size_in_bytes,
                              (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attr.location_in_shader);
        offset += attr.num_components; // move the offset pointer for the next
                                       // iteration
    }
}

} // namespace Saturn
