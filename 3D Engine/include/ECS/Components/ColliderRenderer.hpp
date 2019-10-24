#ifndef SATURN_COLLIDER_RENDERER_HPP_
#define SATURN_COLLIDER_RENDERER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct [[component,
         default_serialize,
         hide_in_editor]] ColliderRenderer : ComponentBase{};

} // namespace Saturn::Components

#endif
