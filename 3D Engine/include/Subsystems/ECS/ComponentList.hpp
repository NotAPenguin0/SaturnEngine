#ifndef MVG_COMPONENT_LIST_HPP_
#define MVG_COMPONENT_LIST_HPP_

#include "Components.hpp"

#define COMPONENT_LIST                                                         \
    ::Saturn::Components::Transform, ::Saturn::Components::Camera,             \
        ::Saturn::Components::FPSCameraController,                             \
        ::Saturn::Components::FreeLookController,                              \
        ::Saturn::Components::CameraZoomController,                            \
        ::Saturn::Components::StaticMesh, ::Saturn::Components::Material,      \
        ::Saturn::Components::ParticleEmitter, ::Saturn::Components::Rotator,  \
        ::Saturn::Components::PointLight

#endif
