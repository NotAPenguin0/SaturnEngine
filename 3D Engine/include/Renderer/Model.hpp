#ifndef SATURN_MODEL_HPP_
#define SATURN_MODEL_HPP_

#include "Mesh.hpp"

#include <vector>

namespace Saturn {

class Model {
public:
    Model() = default;
    Model(Model const&) = delete;
	Model(Model&&) = default;
    Model& operator=(Model const&) = delete;
	Model& operator=(Model&&) = default;

    void swap(Model& other) { std::swap(meshes, other.meshes); }

    std::vector<Mesh> meshes;
};

} // namespace Saturn

#endif // ifndef SATURN_MODEL_HPP_
