#ifndef SATURN_FLASHLIGHT_SYSTEM_HPP_
#define SATURN_FLASHLIGHT_SYSTEM_HPP_

#include "SystemBase.hpp"

namespace Saturn::Systems {

class FlashlightSystem : public SystemBase {
public:
	void on_update(Scene& scene) override;
};

}

#endif