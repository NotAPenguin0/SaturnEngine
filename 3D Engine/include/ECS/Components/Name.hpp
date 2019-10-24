#ifndef SATURN_COMPONENT_NAME_HPP_
#define SATURN_COMPONENT_NAME_HPP_

#include "ComponentBase.hpp"
#include <string>

namespace Saturn::Components {
// clang-format off
struct [[saturn::component, saturn::default_serialize, saturn::hide_in_editor]] Name
    : ComponentBase {
    // clang-format on
    std::string name;
};

} // namespace Saturn::Components

#endif
