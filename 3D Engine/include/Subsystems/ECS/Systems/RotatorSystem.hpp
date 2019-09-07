#include "SystemBase.hpp"

namespace Saturn::Systems {

// Temporary, run this system in the editor for now to test it
class SYSTEM RotatorSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
    SYSTEM_RUN_IN_EDITOR
};

} // namespace Saturn::Systems
