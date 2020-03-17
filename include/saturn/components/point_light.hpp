#ifndef SATURN_COMPONENTS_POINT_LIGHT_HPP_
#define SATURN_COMPONENTS_POINT_LIGHT_HPP_

#include <saturn/utility/color.hpp>

namespace saturn::components {

struct [[component]] PointLight {
    color3 ambient;
    color3 diffuse;
    color3 specular;
};

}

#endif