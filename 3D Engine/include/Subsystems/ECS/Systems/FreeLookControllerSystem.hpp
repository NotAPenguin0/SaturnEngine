#ifndef MVG_FREELOOK_CONTROLLER_SYSTEM_HPP_
#define MVG_FREELOOK_CONTROLLER_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Systems {

class FreeLookControllerSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems

#endif