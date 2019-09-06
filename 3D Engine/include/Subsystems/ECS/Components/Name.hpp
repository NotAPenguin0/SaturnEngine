#ifndef SATURN_COMPONENT_NAME_HPP_
#define SATURN_COMPONENT_NAME_HPP_

#include "ComponentBase.hpp"
#include <string>

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE HIDE_IN_EDITOR Name : ComponentBase {
    std::string name;
};

} // namespace Saturn::Components

#endif
