#include "Subsystems/Renderer/VertexArray.hpp"

#include "Subsystems/Logging/LogSystem.hpp"
#include "Subsystems/Renderer/OpenGL.hpp"
#include "Utility/bind_guard.hpp"

#include <cassert>
#include <numeric>
#include <vector>

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
    if (vao.id != 0 && !buffers.empty() && ebo.id != 0) {
        glDeleteVertexArrays(1, &vao.id);
        glDeleteBuffers(1, &buffers[0]->id);
        glDeleteBuffers(1, &ebo.id);
    }

    buffers.clear();

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
    buffers.emplace_back();
    buffers[0] = std::make_unique<Vbo<BufferTarget::ArrayBuffer>>();
    glGenVertexArrays(1, &vao.id);
    glGenBuffers(1, &buffers[0]->id);
    glGenBuffers(1, &ebo.id);

    if (vao.id == 0) {
        LogSystem::write(LogSystem::Severity::Error, "Failed to create VAO");
    }
    if (buffers.empty()) {
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
    bind_guard vbo_guard(*buffers[0]);
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

std::size_t VertexArray::add_buffer(BufferInfo const& info) {
    buffers.emplace_back();
    buffers.back() = std::make_unique<Vbo<BufferTarget::ArrayBuffer>>();
    glGenBuffers(1, &buffers.back()->id);

    GLint mode;
    switch (info.mode) {
        case BufferMode::Static: mode = GL_STATIC_DRAW; break;
        case BufferMode::Dynamic: mode = GL_DYNAMIC_DRAW; break;
        case BufferMode::DataStream: mode = GL_DYNAMIC_DRAW; break;
    }

    bind_guard vao_guard(vao);
    bind_guard vbo_guard(*buffers.back());

    glBufferData(GL_ARRAY_BUFFER, info.data.size() * sizeof(float),
                 info.data.data(), mode);

    std::size_t vertex_size_in_bytes =
        get_vertex_size_in_bytes(info.attributes);
    std::size_t offset = 0;
    for (auto& attr : info.attributes) {
        assert(attr.num_components >= 1 && attr.num_components <= 4);
        glVertexAttribPointer(attr.location_in_shader, attr.num_components,
                              GL_FLOAT, GL_FALSE, vertex_size_in_bytes,
                              (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(attr.location_in_shader);
        offset += attr.num_components;
    }

    return buffers.size() - 1;
}

} // namespace Saturn
