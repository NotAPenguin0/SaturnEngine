#ifndef SATURN_OUTLINE_RENDERER_COMPONENT_HPP_
#define SATURN_OUTLINE_RENDERER_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE HIDE_IN_EDITOR OutlineRenderer : public ComponentBase {
    color3 color;
};

} // namespace Saturn::Components

#endif