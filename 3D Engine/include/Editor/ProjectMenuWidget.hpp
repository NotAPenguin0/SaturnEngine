#ifndef SATURN_PROJECT_MENU_WIDGET_HPP_
#define SATURN_PROJECT_MENU_WIDGET_HPP_

#include "Editor/EditorWidget.hpp"

namespace Saturn::Editor {

class ProjectMenuWidget : public EditorWidget {
public:
	enum class SettingCategory {
		RenderPipeline,
		Scenes,
		Info
	};

    void show();
private:
    void show_category_pane();
    void show_details_pane();

    SettingCategory cat = SettingCategory::Info;
};

} // namespace Saturn::Editor

#endif // ifndef SATURN_PROJECT_MENU_WIDGET_HPP_
