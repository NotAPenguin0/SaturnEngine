#ifndef MVG_VERTEX_ARRAY_HPP_
#define MVG_VERTEX_ARRAY_HPP_

#include <cstddef>
#include <vector>

#include "OpenGL.hpp"

namespace Saturn {

struct VertexAttribute {
    std::size_t location_in_shader;
    // The amount of components one element has. Must be 1, 2, 3 or 4
    std::size_t num_components;
};

class VertexArray {
public:
    struct CreateInfo {
        std::vector<VertexAttribute> attributes;
        std::vector<float> vertices;
        std::vector<GLuint> indices; ///< Index buffer. Leave empty to auto-generate
        bool dynamic = false; ///< Indicates whether the buffer data will change
    };

    VertexArray() = default;
    VertexArray(CreateInfo const& create_info);

    VertexArray(VertexArray const&) = delete;
    VertexArray(VertexArray&& rhs) = delete;

    VertexArray& operator=(VertexArray const&) = delete;
    VertexArray& operator=(VertexArray&& rhs) = delete;

    void assign(CreateInfo const& create_info);

    ~VertexArray();

    static void bind(VertexArray& buf);
    static void unbind();

	// Returns the amount of vertices
	std::size_t size() const;

	// Returns the amount of indices in the index buffer
	std::size_t index_size() const;

private:
	friend class Renderer;

    void do_create(CreateInfo const& create_info);

    Vao vao;
    Vbo<BufferTarget::ArrayBuffer> vbo;
    Vbo<BufferTarget::ElementArrayBuffer> ebo;

	std::size_t vertex_count;
	std::size_t indices_size;

	std::vector<GLuint> indices;
};

} // namespace Saturn

#endif
