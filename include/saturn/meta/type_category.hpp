#ifndef SATURN_TYPE_CATEGORY_HPP_
#define SATURN_TYPE_CATEGORY_HPP_

#include <saturn/stl/types.hpp>
#include <saturn/stl/bit_flag.hpp>

#include <saturn/components/material.hpp>
#include <saturn/components/point_light.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/transform.hpp>


namespace saturn::meta {

struct TypeCategory : stl::bit_flag<stl::uint32_t> {
    using bit_flag::bit_flag;
    
    static constexpr stl::uint32_t Component = 1;
    static constexpr stl::uint32_t System = 2;
    static constexpr stl::uint32_t Unknown = 4;
};

template<typename T>
TypeCategory type_category() {
    return TypeCategory::Unknown;
}

template<>
TypeCategory type_category<components::Material>() {
    return TypeCategory::Component;
}

template<>
TypeCategory type_category<components::PointLight>() {
    return TypeCategory::Component;
}

template<>
TypeCategory type_category<components::StaticMesh>() {
    return TypeCategory::Component;
}

template<>
TypeCategory type_category<components::Transform>() {
    return TypeCategory::Component;
}


} // namespace saturn::meta

#endif