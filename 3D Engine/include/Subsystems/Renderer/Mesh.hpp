#ifndef MVG_MESH_HPP_
#define MVG_MESH_HPP_

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

    // Only allow const access since we don't want the user to be able to
    // call .assign()
    VertexArray& get_vertices();

private:
    VertexArray vertices;
};

} // namespace Saturn

#endif
