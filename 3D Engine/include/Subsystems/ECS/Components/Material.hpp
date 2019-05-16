#ifndef MVG_MATERIAL_HPP_
#define MVG_MATERIAL_HPP_

#include "ComponentBase.hpp"
#include "Subsystems/AssetManager/Resource.hpp"
#include "Subsystems/Renderer/Shader.hpp"

namespace Saturn::Components {

struct Material : public ComponentBase {
    Resource<Shader> shader;
};

} // namespace Saturn::Components

#endif
