#ifndef MVG_SCENE_GRAPH_HPP_
#define MVG_SCENE_GRAPH_HPP_

#include "Shader.hpp"
#include "VertexArray.hpp"

#include <vector>

namespace Saturn {

struct SceneGraph {
	// temporary
    std::vector<VertexArray*> vtx_arrays;
	Shader* shader;

};

} // namespace Saturn

#endif