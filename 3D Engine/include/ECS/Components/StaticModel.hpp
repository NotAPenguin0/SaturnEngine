#ifndef SATURN_STATIC_MODEL_COMPONENT_HPP_
#define SATURN_STATIC_MODEL_COMPONENT_HPP_



#    include "AssetManager/Resource.hpp"
#    include "ComponentBase.hpp"
#    include "Renderer/Material.hpp"
#    include "Renderer/Model.hpp"

namespace Saturn::Components {

// clang-format off
struct [[component, default_serialize, category("Rendering")]] StaticModel
    : public ComponentBase {

	[[tooltip("The model resource that will be rendered")]]
    Resource<Model> model;

	[[tooltip("Whether to do face culling on the model. Usually you want this, except for planes.")]]
    bool face_cull = true;
};
// clang-format on

} // namespace Saturn::Components

#endif //ifndef SATURN_STATIC_MODEL_COMPONENT_HPP_
