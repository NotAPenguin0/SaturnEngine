#ifndef SATURN_RENDER_PIPELINE_WIDGET_HPP_
#define SATURN_RENDER_PIPELINE_WIDGET_HPP_

#ifdef WITH_EDITOR

#    include "EditorWidget.hpp"

namespace Saturn {
class Application;

namespace RenderModules {
class RenderStage;
}

} // namespace Saturn

namespace Saturn::Editor {

class RenderPipelineWidget : public EditorWidget {
public:
    void show(Application& app);

private:
    void list_render_stages(Application& app);
    void show_add_stage_popup(Application& app);
    void display_stage_info(Application& app);

    RenderModules::RenderStage* selected = nullptr;
};

} // namespace Saturn::Editor

#endif

#endif
