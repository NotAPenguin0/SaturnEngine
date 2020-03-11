#ifndef SATURN_EDITOR_SYSTEM_HPP_
#define SATURN_EDITOR_SYSTEM_HPP_

#include <saturn/systems/system.hpp>
#include <saturn/core/frame_context.hpp>

class EditorSystem : public saturn::systems::System {
public:
    void update(saturn::FrameContext& ctx) override;
};

#endif