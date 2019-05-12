#ifndef MVG_CAMERA_ZOOM_CONTROLLER_SYSTEM_HPP_
#define MVG_CAMERA_ZOOM_CONTROLLER_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Systems {

class CameraZoomControllerSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif
