#include "Subsystems/ECS/Systems/SystemBase.hpp"

#include "Subsystems/Scene/Scene.hpp"

namespace Saturn::Systems {

SystemBase::~SystemBase() {}

void SystemBase::on_start(Scene&) {}

} // namespace Saturn::Systems
