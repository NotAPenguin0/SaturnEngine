#ifndef SATURN_EDITOR_CAMERA_SYSTEM_HPP_
#define SATURN_EDITOR_CAMERA_SYSTEM_HPP_

#include <saturn/systems/system.hpp>

namespace editor {

class CameraSystem : public saturn::systems::System {
public:
    void startup(ph::VulkanContext&, saturn::Scene& scene) override;
    void update(saturn::FrameContext& ctx) override;
};

}

#endif