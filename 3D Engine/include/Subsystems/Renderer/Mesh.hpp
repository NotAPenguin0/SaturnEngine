#ifndef SATURN_MESH_HPP_
#define SATURN_MESH_HPP_

#include "VertexArray.hpp"

namespace Saturn {

class Mesh {
public:
    struct CreateInfo {
        VertexArray::CreateInfo vertices;
    };

    Mesh() = default;
    Mesh(CreateInfo const& create_info);
    Mesh(Mesh const&) = delete;
    Mesh(Mesh&&) = delete;

    Mesh& operator=(Mesh const&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    void assign(CreateInfo const& create_info);

    VertexArray& get_vertices();
    VertexArray const& get_vertices() const { return vertices; }

private:
    VertexArray vertices;
};

} // namespace Saturn

#endif
