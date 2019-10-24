#ifndef SATURN_IMAGE_COMPONENT_HPP_
#define SATURN_IMAGE_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Texture.hpp"

#include "Utility/UIAnchor.hpp"

namespace Saturn::Components {
// clang-format off
struct [[saturn::component, saturn::default_serialize]] Image : ComponentBase {

	[[saturn::tooltip("Position on the UI canvas, relative to it's size.")]]
    glm::vec2 position;

	[[saturn::tooltip("Size of the image.")]]
    glm::vec2 size;

	[[saturn::tooltip("The image to display.")]]
    Resource<Texture> image;

	[[saturn::tooltip("UI anchor to align the image with.")]]
    ui_anchors::anchor_t anchor;
};
// clang-format on

} // namespace Saturn::Components

#endif // ifndef SATURN_IMAGE_COMPONENT_HPP_
