#ifndef SATURN_EDITOR_WIDGET_HPP_
#define SATURN_EDITOR_WIDGET_HPP_

namespace Saturn::Editor {

class EditorWidget {
public:
    bool is_shown() const;
    void set_shown(bool s);

    bool* get_shown_pointer();

protected:
    bool shown = false;
};

} // namespace Saturn::Editor

#endif