#ifndef SATURN_META_FOR_EACH_COMPONENT_HPP_
#define SATURN_META_FOR_EACH_COMPONENT_HPP_

#include <saturn/components/blueprint.hpp>
#include <saturn/components/blueprint_instance.hpp>
#include <saturn/components/camera.hpp>
#include <saturn/components/mesh_renderer.hpp>
#include <saturn/components/name.hpp>
#include <saturn/components/point_light.hpp>
#include <saturn/components/static_mesh.hpp>
#include <saturn/components/transform.hpp>
#include <samples/components/rotator.hpp>
#include <editor/components/editor_camera.hpp>

#include <stl/utility.hpp>

namespace saturn::meta {

#define component_list ::saturn::components::Blueprint, ::saturn::components::BlueprintInstance, ::saturn::components::Camera, ::saturn::components::MeshRenderer, ::saturn::components::Name, ::saturn::components::PointLight, ::saturn::components::StaticMesh, ::saturn::components::Transform, ::saturn::components::Rotator, ::saturn::components::EditorCamera 

namespace detail {

template<template<typename> typename, typename...>
struct for_each_component_impl;

template<template<typename> typename F, typename CFirst>
struct for_each_component_impl<F, CFirst> {
    template<typename... Args>
    void operator()(Args&&... args) {
        F<CFirst>{}(stl::forward<Args>(args) ...);
    }
};

template<template<typename> typename F, typename CFirst, typename CNext, typename... CRest>
struct for_each_component_impl<F, CFirst, CNext, CRest ...> {
    template<typename... Args>
    void operator()(Args&&... args) {
        F<CFirst>{}(stl::forward<Args>(args) ...);
        for_each_component_impl<F, CNext, CRest...>{}(stl::forward<Args>(args) ...);
    }
};

}


template<template<typename> typename F, typename... Args>
void for_each_component(Args&&... args) {
    detail::for_each_component_impl<F, component_list>{}(stl::forward<Args>(args) ...);
}

#undef component_list

}

#endif