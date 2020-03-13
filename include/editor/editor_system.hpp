#ifndef SATURN_EDITOR_SYSTEM_HPP_
#define SATURN_EDITOR_SYSTEM_HPP_

#include <saturn/systems/system.hpp>
#include <saturn/core/frame_context.hpp>

#include <editor/log_window.hpp>

struct ImFont;

class EditorSystem : public saturn::systems::System {
public:
    EditorSystem(LogWindow* log_window);

    void startup(ph::VulkanContext& ctx) override;
    void update(saturn::FrameContext& ctx) override;
private:
    LogWindow* log_window;
    ImFont* editor_font;
};

#endif