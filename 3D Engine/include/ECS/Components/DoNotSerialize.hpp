#ifndef SATURN_DO_NOT_SERIALIZE_COMPONENT_HPP_
#define SATURN_DO_NOT_SERIALIZE_COMPONENT_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

struct [[saturn::component, saturn::hide_in_editor]] DoNotSerialize : public ComponentBase {};

} // namespace Saturn::Components

#endif
