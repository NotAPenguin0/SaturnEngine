#include "SystemBase.hpp"

namespace Saturn::Systems {

class RotatorSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

}