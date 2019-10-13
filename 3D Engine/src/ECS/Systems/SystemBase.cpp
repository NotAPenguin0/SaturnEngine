#include "ECS/Systems/SystemBase.hpp"

#include "Scene/Scene.hpp"

namespace Saturn::Systems {

SystemBase::~SystemBase() {}

void SystemBase::on_start(Scene&) {}
void SystemBase::on_late_update(Scene&) {}

} // namespace Saturn::Systems
