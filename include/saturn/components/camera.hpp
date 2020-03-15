#ifndef SATURN_CAMERA_COMPONENT_HPP_
#define SATURN_CAMERA_COMPONENT_HPP_

#include <glm/vec3.hpp>

namespace saturn::components {

struct [[component]] Camera {
public:
    glm::vec3 front = glm::vec3(1, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);

    float fov = 0.785398f; // == radians(45)
};

}

#endif