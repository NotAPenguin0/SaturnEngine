#ifndef SATURN_EDITOR_CAMERA_SYSTEM_HPP_
#define SATURN_EDITOR_CAMERA_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Components {
struct Transform;
struct Camera;
struct EditorCameraController;
} // namespace Saturn::Components

namespace Saturn::Systems {

class SYSTEM EditorCameraSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;

    SYSTEM_RUN_IN_EDITOR
    SYSTEM_EDITOR_ONLY
private:
    void freelook(Components::Transform& trans,
                  Components::Camera& cam,
                  Components::EditorCameraController& controller);
    void movement(Components::Transform& trans,
                  Components::Camera& cam,
                  Components::EditorCameraController& controller);
	void zoom(Components::Camera& cam, Components::EditorCameraController& controller);
};

} // namespace Saturn::Systems

#endif
