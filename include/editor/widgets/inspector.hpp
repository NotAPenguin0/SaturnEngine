#ifndef SATURN_ENTITY_INSPECTOR_HPP_
#define SATURN_ENTITY_INSPECTOR_HPP_

#include <saturn/core/frame_context.hpp>

namespace editor {

class Inspector {
public:
    void show(saturn::ecs::registry& ecs, saturn::ecs::entity_t entity);

    bool* get_shown_pointer() { return &shown; }
    bool is_shown() const { return shown; }

private:
    bool shown = true;
};

}

#endif