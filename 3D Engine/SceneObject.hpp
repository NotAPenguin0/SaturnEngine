#ifndef MVG_SCENE_OBJECT_HPP_
#define MVG_SCENE_OBJECT_HPP_

#include "VertexArray.hpp" // Temporary
#include "Shader.hpp"

namespace Saturn {

class Scene;

class SceneObject {
public:
    friend class Scene;

    SceneObject();

    struct Debug {
        VertexArray cube;
		Shader shader;
    } debug;

private:
};

} // namespace Saturn

#endif
