#ifndef SATURN_EDITOR_HPP_
#define SATURN_EDITOR_HPP_

#ifdef WITH_EDITOR

#include <cstddef>

namespace Saturn {

class Application;

namespace Editor {

class Editor {
public:
    Editor(Application& app);
	
	void setup_viewports();

	// Must be called at the start of the frame
	void render();
    void frame_end();

private:
    Application* app;
    std::size_t scene_view_viewport_id = 0;
};

} // namespace Editor
} // namespace Saturn

#endif
#endif