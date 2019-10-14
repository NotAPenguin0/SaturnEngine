#ifndef SATURN_IMAGE_COMPONENT_HPP_
#define SATURN_IMAGE_COMPONENT_HPP_

#include "ComponentBase.hpp"

#include "AssetManager/Resource.hpp"
#include "Renderer/Texture.hpp"

namespace Saturn::Components {

struct COMPONENT DEFAULT_SERIALIZE Image : ComponentBase {
    glm::vec2 position;
	glm::vec2 size;
    Resource<Texture> image;
};

} // namespace Saturn::Components

#endif // ifndef SATURN_IMAGE_COMPONENT_HPP_
