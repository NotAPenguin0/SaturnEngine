#ifndef SATURN_BUTTON_COMPNENT_HPP_
#define SATURN_BUTTON_COMPNENT_HPP_

#include "ComponentBase.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Texture.hpp"

#include "Utility/Color.hpp"
#include "Utility/UIAnchor.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize]] Button
    : ComponentBase {

	enum class Event {
		None,
		Press,
		Hold,
		Release
	};

	[[do_not_serialize, hide_in_editor]]
	Event last_event;

	[[tooltip("Position on the UI canvas, relative to it's size.")]]
    glm::vec2 position;

	[[tooltip("Size of the button")]]
    glm::vec2 size;

	[[tooltip("Background image to display.")]]
    Resource<Texture> image;

	[[tooltip("Color to multiply with the image")]]
	color3 color;

	[[tooltip("UI anchor to align the button with.")]]
    ui_anchors::anchor_t anchor;

};
// clang-format on
} // namespace Saturn::Components

#endif // ifndef SATURN_BUTTON_COMPNENT_HPP_
