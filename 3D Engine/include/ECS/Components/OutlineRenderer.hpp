#ifndef SATURN_OUTLINE_RENDERER_COMPONENT_HPP_
#define SATURN_OUTLINE_RENDERER_COMPONENT_HPP_

#include "ComponentBase.hpp"
#include "Utility/Color.hpp"

namespace Saturn::Components {

struct [[component,
         default_serialize,
         hide_in_editor]] OutlineRenderer : public ComponentBase {
    color3 color;
};

} // namespace Saturn::Components

#endif
