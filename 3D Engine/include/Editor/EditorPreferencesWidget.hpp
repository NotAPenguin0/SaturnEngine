#ifndef SATURN_EDITOR_PREFERENCES_WIDGET_HPP_
#define SATURN_EDITOR_PREFERENCES_WIDGET_HPP_

#ifdef WITH_EDITOR

#    include "Editor/EditorWidget.hpp"

namespace Saturn::Editor {

class EditorPreferencesWidget : public EditorWidget {
public:
    struct Preferences {
        struct Camera {
            float sensitivity = 0.1f;
            float speed = 4.0f;
            float zoom_speed = 100.0f;
            float fov = 45.0f;
		} camera;
    };

	Preferences const& get_preferences() const;

    void show();

private:
	Preferences prefs;
};

} // namespace Saturn::Editor

#endif

#endif
