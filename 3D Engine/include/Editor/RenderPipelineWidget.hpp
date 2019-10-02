#ifndef SATURN_RENDER_PIPELINE_WIDGET_HPP_
#define SATURN_RENDER_PIPELINE_WIDGET_HPP_

#ifdef WITH_EDITOR

#    include "EditorWidget.hpp"

#include <string_view>

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

	void add_stage(Application& app, std::string_view type, std::string_view stage);

private:
    void list_render_stages(Application& app);
    void show_add_stage_popup(Application& app);
    void display_stage_info(Application& app);

    RenderModules::RenderStage* selected = nullptr;
};

} // namespace Saturn::Editor

#endif

#endif
