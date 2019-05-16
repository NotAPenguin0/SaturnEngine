#ifndef MVG_COMPONENT_VIEW_HPP_
#define MVG_COMPONENT_VIEW_HPP_

#include "Subsystems/Scene/SceneObject.hpp"
#include "component_index.hpp"

#include <tuple>

namespace Saturn {

template<typename... Cs>
class component_view {
public:
    using entity_iterator = std::vector<std::unique_ptr<SceneObject>>::iterator;

    // Warning: pass view of ENTITIES here!
    component_view(entity_iterator e_first, entity_iterator e_last) :
        first(e_first), last(e_last) {}

    class iterator {
    public:
        iterator() = default;
        iterator(entity_iterator i, entity_iterator end) : it(i), last(end) {
            if (it == last) stop = true;
            ensure_valid_iterator();
        }
        iterator(iterator const&) = default;
        iterator(iterator&&) = default;

        iterator& operator=(iterator const&) = default;
        iterator& operator=(iterator&&) = default;

        std::tuple<Cs&...> operator*() {
            if (stop) {
                throw std::runtime_error(
                    "Attempting to dereference an invalid iterator");
            }
            return std::tie((*it)->template get_component<Cs>()...);
        }

        std::tuple<Cs const&...> operator*() const {
            if (stop) {
                throw std::runtime_error(
                    "Attempting to dereference an invalid iterator");
            }
            return std::tie((*it)->template get_component<Cs>()...)
        }

        // prefix increment
        iterator& operator++() {
            if (stop) { return *this; }

            if (it == last) { //#MaybeOptimize Is this check redundant?
                stop = true;
                return *this;
            }

            ++it;

            if (it == last) {
                stop = true;
                return *this;
            }
            if (!((*it)->template has_component<Cs>() && ...)) { ++(*this); }
            return *this;
        }

        iterator operator++(int) {
            iterator copy = *this;
            ++(*this);
            return copy;
        }

        bool operator==(iterator const& rhs) {
            if (last == rhs.last && rhs.it == rhs.last) return it == rhs.it;
            if (stop == false && rhs.stop == false) return true;
            return *it == *rhs.it;
        }

        bool operator!=(iterator const& rhs) { return !(*this == rhs); }

    private:
        void ensure_valid_iterator() {
            if (stop || it == last) return;
            while (!((*it)->template has_component<Cs>() && ...)) { ++(*this); }
        }

        entity_iterator it;
        entity_iterator last;

        bool stop = false;
    };

    iterator begin() { return iterator{first, last}; }
    iterator end() { return iterator{last, last}; }

private:
    entity_iterator first;
    entity_iterator last;
};

} // namespace Saturn

#endif
