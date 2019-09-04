#include "Editor/EditorWidget.hpp"

namespace Saturn::Editor {


bool EditorWidget::is_shown() const { return shown; }

void EditorWidget::set_shown(bool s) { shown = s; }

bool* EditorWidget::get_shown_pointer() { return &shown; }

}