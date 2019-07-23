#include "Subsystems/ECS/Systems/SystemBase.hpp"

#include "Subsystems/Scene/Scene.hpp"

namespace Saturn::Systems {

SystemBase::~SystemBase() {}

void SystemBase::on_start(Scene&) {}
void SystemBase::on_late_update(Scene&) {}

} // namespace Saturn::Systems
