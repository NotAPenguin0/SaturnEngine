#ifndef SATURN_EDITOR_STYLE_HPP_
#define SATURN_EDITOR_STYLE_HPP_

namespace Saturn::Editor {

enum class Style { ImGuiDark = 0, Grey = 1, None};

void set_editor_style(Style style);

void init_font();
void clear_font();
void push_font();

}

#endif // ifndef SATURN_EDITOR_STYLE_HPP_
