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
struct [[component, default_serialize, category("UI")]] Text : ComponentBase {
	[[tooltip("The text string to display.")]]
    std::string text;

	[[tooltip("Position on the UI canvas, relative to it's size.")]]
    glm::vec2 position;

	[[tooltip("Size of the text.")]]
    glm::vec2 size;

	[[tooltip("Text color.")]]
    color3 color;

	[[tooltip("Font resource to render the text with.")]]
    Resource<Font> font;

	[[tooltip("UI anchor to align the text with.")]]
    ui_anchors::anchor_t anchor;
};
// clang-format on

} // namespace Saturn::Components

#endif // ifndef SATURN_TEXT_COMPONENT_HPP_
