#ifndef SATURN_ECS_COMPONENT_STORAGE_BASE_HPP_
#define SATURN_ECS_COMPONENT_STORAGE_BASE_HPP_

#include <saturn/stl/sparse_set.hpp>
#include <saturn/ecs/entity.hpp>

namespace saturn::ecs {

class component_storage_base : public stl::sparse_set<entity_t> {
public:
    using sparse_set::sparse_set;
};

}

#endif