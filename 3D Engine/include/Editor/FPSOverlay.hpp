#ifndef SATURN_EDITOR_FPS_OVERLAY_HPP_
#define SATURN_EDITOR_FPS_OVERLAY_HPP_

#ifdef WITH_EDITOR

#    include "EditorWidget.hpp"

#    include <vector>

namespace Saturn::Editor {
class FPSOverlay : public EditorWidget {
public:
    void show();
private:
	std::vector<float> frame_times;
	float sum = 0.0f;
	size_t num_frames = 300;
};
} // namespace Saturn::Editor

#endif

#endif
