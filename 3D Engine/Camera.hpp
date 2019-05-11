#ifndef MVG_COMPONENT_CAMERA_HPP_
#define MVG_COMPONENT_CAMERA_HPP_

#include "ComponentBase.hpp"
#include "Vector.hpp"

namespace Saturn::Components {

struct Camera : public ComponentBase {
    Math::Vec3<float> front;
    Math::Vec3<float> up;
};

} // namespace Saturn::Components

#endif
