#ifndef MVG_COMPONENTS_TRANSFORM_HPP_
#define MVG_COMPONENTS_TRANSFORM_HPP_

#include "ComponentBase.hpp"
#include "Vec3.hpp"

namespace Saturn {

namespace Components {

struct Transform : public ComponentBase {
    Math::Vec3<float> position = Math::Vec3<float>(0.0f, 0.0f, 0.0f);

    struct Rotation {
        Math::Vec3<float> axis = Math::Vec3<float>(0.0f, 0.0f, 0.0f);
        float angle_in_radians = 0.0f;
    } rotation;

    Math::Vec3<float> scale = Math::Vec3<float>(1.0f, 1.0f, 1.0f);
};

} // namespace Components

} // namespace Saturn

#endif
