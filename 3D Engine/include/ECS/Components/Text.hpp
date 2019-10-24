#ifndef SATURN_TEXT_COMPONENT_HPP_
#define SATURN_TEXT_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Font.hpp"
#include "Utility/Color.hpp"
#include "Utility/UIAnchor.hpp"

#include <glm/glm.hpp>
#include <string>

namespace Saturn::Components {
// clang-format off
struct [[saturn::component, saturn::default_serialize]] Text : ComponentBase {
	[[saturn::tooltip("The text string to display.")]]
    std::string text;

	[[saturn::tooltip("Position on the UI canvas, relative to it's size.")]]
    glm::vec2 position;

	[[saturn::tooltip("Size of the text.")]]
    glm::vec2 size;

	[[saturn::tooltip("Text color.")]]
    color3 color;

	[[saturn::tooltip("Font resource to render the text with.")]]
    Resource<Font> font;

	[[saturn::tooltip("UI anchor to align the text with.")]]
    ui_anchors::anchor_t anchor;
};
// clang-format on

} // namespace Saturn::Components

#endif // ifndef SATURN_TEXT_COMPONENT_HPP_
