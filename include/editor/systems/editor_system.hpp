#ifndef SATURN_EDITOR_SYSTEM_HPP_
#define SATURN_EDITOR_SYSTEM_HPP_

#include <saturn/systems/system.hpp>

#include <editor/widgets/widget.hpp>
#include <editor/widgets/entity_tree.hpp>
#include <editor/widgets/log_window.hpp>
#include <editor/widgets/inspector.hpp>

#include <stl/vector.hpp>
#include <stl/unique_ptr.hpp>

struct ImFont;

namespace editor {

class EditorSystem : public saturn::systems::System {
public:
    EditorSystem(LogWindow* log_window);

    void startup(ph::VulkanContext& ctx, saturn::Scene& scene) override;
    void update(saturn::FrameContext& ctx) override;
private:
    LogWindow* log_window;
    Inspector inspector;
    stl::unique_ptr<EntityTree> scene_tree;
    stl::unique_ptr<EntityTree> blueprints_tree;
    ImFont* editor_font;

    stl::vector<stl::unique_ptr<Widget>> widgets;
};

}

#endif