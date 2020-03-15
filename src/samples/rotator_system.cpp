#include <samples/rotator_system.hpp>

#include <samples/components/rotator.hpp>
#include <saturn/components/transform.hpp>

using namespace saturn::components;

namespace samples {

void RotatorSystem::update(saturn::FrameContext& ctx) {
    for (auto[transform, rotator] : ctx.ecs.view<Transform, Rotator>()) {
        transform.rotation += rotator.axes * rotator.speed;
    }
}

}