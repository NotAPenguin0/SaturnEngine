#ifndef SATURN_MESH_HPP_
#define SATURN_MESH_HPP_

#include "VertexArray.hpp"
#include "Material.hpp"
#include "AssetManager/Resource.hpp"

namespace Saturn {

class Mesh {
public:
    struct CreateInfo {
        VertexArray::CreateInfo vertices;
    };

    Mesh() = default;
    Mesh(CreateInfo const& create_info);
    Mesh(Mesh const&) = delete;
    Mesh(Mesh&&) = default;

    Mesh& operator=(Mesh const&) = delete;
    Mesh& operator=(Mesh&&) = default;

    void assign(CreateInfo const& create_info);

    VertexArray& get_vertices();
    VertexArray const& get_vertices() const { return vertices; }

    void swap(Mesh& other) { vertices.swap(other.vertices); }

	Resource<Material> material;

private:
    VertexArray vertices;
};

} // namespace Saturn

#endif
