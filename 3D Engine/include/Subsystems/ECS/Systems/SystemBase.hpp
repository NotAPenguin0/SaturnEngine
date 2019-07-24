#ifndef SATURN_SYSTEM_BASE_HPP_
#define SATURN_SYSTEM_BASE_HPP_

namespace Saturn {
class Scene;
}

namespace Saturn {
namespace Systems {

class SystemBase {
public:
    virtual ~SystemBase() = 0;

    virtual void on_start(Scene& scene);
    virtual void on_update(Scene& scene) = 0;
    // late_update is when the physics system updates and applies all forces
    virtual void on_late_update(Scene& scene);
};

} // namespace Systems
} // namespace Saturn

#endif
