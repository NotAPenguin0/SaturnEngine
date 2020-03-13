#ifndef SATURN_EDITOR_SYSTEM_HPP_
#define SATURN_EDITOR_SYSTEM_HPP_

#include <saturn/systems/system.hpp>
#include <saturn/core/frame_context.hpp>

#include <editor/log_window.hpp>

class EditorSystem : public saturn::systems::System {
public:
    EditorSystem(LogWindow* log_window);

    void update(saturn::FrameContext& ctx) override;
private:
    LogWindow* log_window;
};

#endif