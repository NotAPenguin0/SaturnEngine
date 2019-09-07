#include "SystemBase.hpp"

namespace Saturn::Systems {

// Temporary, run this system in the editor for now to test it
class SYSTEM RotatorSystem : public SystemBase {
public:
    void on_update(Scene& scene) override;
};

} // namespace Saturn::Systems
