#ifndef SATURN_EDITOR_WIDGET_HPP_
#define SATURN_EDITOR_WIDGET_HPP_

#include <saturn/core/frame_context.hpp>

namespace editor {

class Widget {
public:
    virtual void show(saturn::FrameContext& ctx) = 0;
    
    bool* get_shown_pointer() { return &shown; }
    bool is_shown() const { return shown; }

private:
    bool shown = true;
};


}

#endif