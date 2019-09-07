#ifndef SATURN_FREELOOK_CONTROLLER_SYSTEM_HPP_
#define SATURN_FREELOOK_CONTROLLER_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Systems {

class SYSTEM FreeLookControllerSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif