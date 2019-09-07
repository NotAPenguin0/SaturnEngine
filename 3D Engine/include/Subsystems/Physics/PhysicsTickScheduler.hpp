#ifndef SATURN_PHYSICS_TICK_SCHEDULER_HPP_
#define SATURN_PHYSICS_TICK_SCHEDULER_HPP_

namespace Saturn {

class Physics;
class Scene;

enum class SystemUpdateMode;

class PhysicsTickScheduler {
public:
    PhysicsTickScheduler() = default;
	PhysicsTickScheduler(Physics& physics_system);

	PhysicsTickScheduler(PhysicsTickScheduler&& rhs);
	PhysicsTickScheduler& operator=(PhysicsTickScheduler&& rhs);

	void set_physics_system(Physics& physics_system);

	void update(SystemUpdateMode mode, Scene& scene);

private:
	Physics* physics = nullptr;
    float timer;
};

} // namespace Saturn

#endif