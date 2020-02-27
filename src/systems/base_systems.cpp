#include <saturn/systems/base_systems.hpp>

#include <saturn/components/transform.hpp>
#include <saturn/components/static_mesh.hpp>

namespace saturn::systems { 

void register_base_systems(saturn::ecs::system_manager& mngr) {
    mngr.add_system(&test_scale_system::update);
}

namespace test_scale_system {
    void update(saturn::ecs::registry& ecs) {
        using namespace saturn::components;
        for (auto const&[transform, mesh] : ecs.view<Transform, StaticMesh>()) {
//            transform.scale = transform.scale * 0.999f;
        }
    }
}

}