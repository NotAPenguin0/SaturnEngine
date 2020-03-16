#ifndef SATURN_EDITOR_NAME_COMPONENT_HPP_
#define SATURN_EDITOR_NAME_COMPONENT_HPP_

#include <string>

namespace saturn::components {

struct [[component]] Name {
    std::string name;
};

}

#endif