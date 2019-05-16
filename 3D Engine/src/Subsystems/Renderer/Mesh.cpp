#include "Subsystems/Renderer/Mesh.hpp"

namespace Saturn {

Mesh::Mesh(CreateInfo const& create_info) : vertices(create_info.vertices) {}

void Mesh::assign(CreateInfo const& create_info) {
	vertices.assign(create_info.vertices);
}

VertexArray& Mesh::get_vertices() { return vertices; }

} // namespace Saturn