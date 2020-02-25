#ifndef SATURN_ECS_COMPONENT_STORAGE_HPP_
#define SATURN_ECS_COMPONENT_STORAGE_HPP_

#include <saturn/stl/sparse_set.hpp>

#include <saturn/ecs/component_storage_base.hpp>
#include <saturn/ecs/entity.hpp>

namespace saturn::ecs {

// T is the component type
template<typename T>
class component_storage : public component_storage_base {
public:
    using underlying_storage = stl::sparse_set<entity_t>;

    class iterator {
    public:
        using value_type = T;

        iterator(std::vector<T>* components_ref, size_t index) :
            components_ref(components_ref), index(index) {
        }

        iterator(iterator const& rhs) = default;

        iterator& operator=(iterator const& rhs) = default;

        iterator& operator++() {
            ++index;
            return *this;
        }

        iterator& operator++(int) {
            iterator old = *this;
            ++index;
            return old;
        }

        iterator& operator--() {
            --index;
            return *this;
        }

        iterator& operator--(int) {
            iterator old = *this;
            --index;
            return *this;
        }

        bool operator==(iterator other) const {
            return components_ref == other.components_ref && index == other.index;
        }

        bool operator!=(iterator other) const {
            return !(*this == other);
        }

        bool operator<(iterator other) const {
            SATURN_ASSERT(components_ref == other.components_ref, "Cannot compare incompatible iterators");
            return index < other.index;
        }

        bool operator<=(iterator other) const {
            SATURN_ASSERT(components_ref == other.direcomponents_refct_ref, "Cannot compare incompatible iterators");
            return index <= other.index;
        }

        bool operator>(iterator other) const {
            SATURN_ASSERT(components_ref == other.components_ref, "Cannot compare incompatible iterators");
            return index > other.index;
        }

        bool operator>=(iterator other) const {
            SATURN_ASSERT(components_ref == other.components_ref, "Cannot compare incompatible iterators");
            return index >= other.index;
        }

        T& operator*() {
            return components_ref->at(index);
        }

        T const& operator*() const {
            return components_ref->at(index);
        }

        auto operator->() {
            return this;
        }

    private:
        friend class sparse_set;

        stl::vector<T>* components_ref;
        size_t index;
    };

    component_storage() = default;
    component_storage(component_storage const&) = default;
    component_storage(component_storage&&) = default;

    component_storage& operator=(component_storage const&) = default;
    component_storage& operator=(component_storage&&) = default;

    ~component_storage() = default;

    iterator begin() {
        return iterator(&components, 0);
    }

    iterator end() {
        return iterator(&components, components.size());
    }

    iterator insert(entity_t entity, T const& value) {
        // By inserting at the end, this component will be at the same index as the value in our
        // direct list in the sparse set.
        components.push_back(value);
        underlying_storage::insert(entity);

        return iterator(&components, components.size() - 1);
    }

    template<typename... Args>
    iterator construct(entity_t entity, Args&&... args) {
        components.push_back(T{std::forward<Args>(args) ...});
        underlying_storage::insert(entity);

        return iterator(&components, components.size() - 1);
    }

    iterator find(entity_t entity) {
        underlying_storage::iterator it = underlying_storage::find(entity);
        if (it == underlying_storage::end()) {
            end();
        }

        return iterator(&components, it.get_index());
    }

    T& get(entity_t entity) {
        auto it = find(entity);
        SATURN_ASSERT(it != end(), "Entity not in storage");
        return *it;
    }

    size_t size() const {
        return components.size();
    }

private:
    stl::vector<T> components;
};

}

#endif