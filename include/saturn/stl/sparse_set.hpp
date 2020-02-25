#ifndef SATURN_STL_SPARSE_SET_HPP_
#define SATURN_STL_SPARSE_SET_HPP_

#include <saturn/stl/vector.hpp>
#include <saturn/stl/traits.hpp>
#include <saturn/stl/assert.hpp>

// stl namespace is an extension of the standard library. It might contain the a stl implementation later
namespace stl {

template<typename T>
class sparse_set {
public:
    // Requirements
    static_assert(is_unsigned_v<T>, "sparse_set value type must be an unsigned integral type");

    // Typedefs
    using value_type = T;
    
    // Note that the iterator is always const, since the values are always returned by value.
    class iterator {
    public:
        using value_type = T;

        iterator(std::vector<T> const* direct_ref, size_t index) :
            direct_ref(direct_ref), index(index) {
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
            return direct_ref == other.direct_ref && index == other.index;
        }

        bool operator!=(iterator other) const {
            return !(*this == other);
        }

        bool operator<(iterator other) const {
            SATURN_ASSERT(direct_ref == other.direct_ref, "Cannot compare incompatible iterators");
            return index < other.index;
        }

        bool operator<=(iterator other) const {
            SATURN_ASSERT(direct_ref == other.direct_ref, "Cannot compare incompatible iterators");
            return index <= other.index;
        }

        bool operator>(iterator other) const {
            SATURN_ASSERT(direct_ref == other.direct_ref, "Cannot compare incompatible iterators");
            return index > other.index;
        }

        bool operator>=(iterator other) const {
            SATURN_ASSERT(direct_ref == other.direct_ref, "Cannot compare incompatible iterators");
            return index >= other.index;
        }

        T operator*() const {
            return direct_ref->at(index);
        }

        auto operator->() const {
            return this;
        }

        size_t get_index() const {
            return index;
        }

    private:
        friend class sparse_set;

        vector<T> const* direct_ref;
        size_t index;
    };

    sparse_set() = default;
    sparse_set(sparse_set const& rhs) = default;
    sparse_set(sparse_set&& rhs) = default;

    sparse_set& operator=(sparse_set const& rhs) = default;
    sparse_set& operator=(sparse_set&& rhs) = default;

    ~sparse_set() = default;

    iterator begin() const {
        return iterator(&direct, 0);
    }

    iterator end() const {
        return iterator(&direct, direct.size());
    }

    iterator find(T value) const {
        // If the value cannot fit in our reverse vector, it certainly isn't in the set
        if (value >= reverse.size()) return end();
        // If a value is in the set, the direct and reverse values point at each other
        T index = reverse[value];
        T direct_val = direct[index];

        if (direct_val == value) {
            return iterator(&direct, index);
        }

        return end();
    }

    // Returns an iterator pointing to the inserted value
    iterator insert(T value) {
        SATURN_ASSERT(find(value) == end(), "sparse_set cannot have duplicate values.");

        size_t index = direct.size();

        direct.push_back(value);

        // Make sure the reverse vector can hold at least enough values to make index value valid.
        assure_capacity(value + 1);
        reverse[value] = index;

        return iterator(&direct, index);
    }

    void clear() {
        reverse.clear();
        direct.clear();
    }

    size_t size() const {
        return direct.size();
    }

private:
    void assure_capacity(T n) {
        if (reverse.size() < n) {
            reverse.resize(n);
        }
    }

    vector<T> direct;
    vector<T> reverse;
};

} // namespace stl

#endif