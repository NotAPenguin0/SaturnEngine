#ifndef SATURN_COLLIDER_RENDERER_HPP_
#define SATURN_COLLIDER_RENDERER_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct [[saturn::component,
         saturn::default_serialize,
         saturn::hide_in_editor]] ColliderRenderer : ComponentBase{};

} // namespace Saturn::Components

#endif
