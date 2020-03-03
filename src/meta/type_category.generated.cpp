#include <saturn/meta/type_category.hpp>

#include <saturn/components/transform.hpp>

namespace saturn::meta {

using namespace components;

template<>
TypeCategory type_category<Transform>() {
    return TypeCategory::Component;
}

}