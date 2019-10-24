#ifndef SATURN_BUTTON_COMPNENT_HPP_
#define SATURN_BUTTON_COMPNENT_HPP_

#include "ComponentBase.hpp"

namespace Saturn::Components {

// clang-format off
struct [[saturn::component, saturn::default_serialize]] Button
    : ComponentBase {

	enum class LastEvent {
		None,
		Press,
		Hold,
		Release
	};

	[[saturn::do_not_serialize, saturn::hide_in_editor]]
	LastEvent last_event;



};
// clang-format on
} // namespace Saturn::Components

#endif // ifndef SATURN_BUTTON_COMPNENT_HPP_
