#ifndef MVG_PHYSICS_SYSTEM_HPP_
#define MVG_PHYSICS_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Systems {

class PhysicsSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

}

#endif