#ifndef SATURN_PROJECT_MENU_WIDGET_HPP_
#define SATURN_PROJECT_MENU_WIDGET_HPP_

#include "Editor/EditorWidget.hpp"
#include "Editor/RenderPipelineWidget.hpp"

namespace Saturn {
class Scene;
}

namespace Saturn::Editor {

class Editor;

class ProjectMenuWidget : public EditorWidget {
public:
    enum class SettingCategory { RenderPipeline, Scenes, Info };

    void show(Application& app, Editor* editor, Scene& scene);

    RenderPipelineWidget pipeline_settings;

private:
    void show_category_pane();
    void show_details_pane(Application& app, Editor* editor, Scene& scene);

    SettingCategory cat = SettingCategory::Info;
};

} // namespace Saturn::Editor

#endif // ifndef SATURN_PROJECT_MENU_WIDGET_HPP_
