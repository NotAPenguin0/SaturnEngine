#ifndef SATURN_STL_VECTOR_HPP_
#define SATURN_STL_VECTOR_HPP_

#include <saturn/stl/types.hpp>
#include <saturn/stl/algorithm.hpp>
#include <saturn/stl/assert.hpp>

namespace stl {

template<typename T>
class vector {
public:
    using value_type = T;
    using reference = T&;

    using iterator = T*;
    using const_iterator = T const*;

    vector() = default;

    vector(size_t n) {
        reserve_no_copy(n);
    }

    vector(vector const& rhs) {
        reserve_no_copy(rhs.sz);

        copy(rhs.begin(), rhs.end(), begin());
        sz = rhs.sz;
        cap = rhs.cap;
    }

    vector(vector&& rhs) {
        delete[] ptr;
        ptr = rhs.ptr;
        sz = rhs.sz;
        cap = rhs.cap;

        rhs.ptr = nullptr;
        rhs.sz = 0;
        rhs.cap = 0;
    }

    vector& operator=(vector const& rhs) {
        if (this != &rhs) {
            reserve_no_copy(rhs.sz);

            copy(rhs.begin(), rhs.end(), begin());
            sz = rhs.sz;
            cap = rhs.cap;
        }
        return *this;
    }

    vector& operator=(vector&& rhs) {
        if (this != &rhs) {
            delete[] ptr;
            ptr = rhs.ptr;
            sz = rhs.sz;
            cap = rhs.cap;

            rhs.ptr = nullptr;
            rhs.sz = 0;
            rhs.cap = 0;
        }
        return *this;
    }

    ~vector() {
        delete[] ptr;
        sz = 0;
        cap = 0;
    }

    iterator begin() {
        return ptr;
    }

    iterator begin() const {
        return ptr;
    }

    iterator end() {
        return ptr + sz;
    }

    iterator end() const {
        return ptr + sz;
    }

    void reserve(size_t n) {
        if (cap >= n) return;

        T* new_ptr = new T[n];
        // Copy old data
        for (size_t i = 0; i < sz; ++i) {
            new_ptr[i] = std::move(ptr[i]);
        }

        delete[] ptr;
        ptr = new_ptr;
        cap = n;
    }

    void resize(size_t n) {
        if (cap >= n) return;

        T* new_ptr = new T[n];
        // Move old data
        for (size_t i = 0; i < sz; ++i) {
            new_ptr[i] = std::move(ptr[i]);
        }
        // Fill new data with default constructed values
        for (size_t i = sz; i < n; ++i) {
            new_ptr[i] = T{};
        }

        delete[] ptr;
        ptr = new_ptr;
        sz = n;
        cap = n;
    }

    // reserves space but does not keep previous data
    void reserve_no_copy(size_t n) {
        if (cap >= n) return;
        // Free old ptr
        delete[] ptr;
        // Allocate new array
        ptr = new T[n];
        sz = n;
        cap = n;
    }  

    void push_back(T const& value) {
        grow_if_needed(sz + 1);
        back() = value;
    }

    void push_back(T&& value) {
        grow_if_needed(sz + 1);
        back() = std::move(value);
    }

    template<typename... Args>
    void emplace_back(Args&&... args) {
        grow_if_needed(sz + 1);
        T* location = &back();
        new (location) T(std::forward<Args>(args) ...);
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return size() == 0;
    }

    size_t capacity() const {
        return cap;
    }

    T& operator[](size_t index) {
        SATURN_ASSERT(index < size(), "vector index out of range");
        return ptr[index];
    }

    T const& operator[](size_t index) const {
        SATURN_ASSERT(index < size(), "vector index out of range");
        return ptr[index];
    }

    T& at(size_t index) {
        SATURN_ASSERT(index < size(), "vector index out of range");
        return ptr[index];
    }

    T const& at(size_t index) const {
        SATURN_ASSERT(index < size(), "vector index out of range");
        return ptr[index];
    }

    T& front() {
        SATURN_ASSERT(size() > 0, "front() called on empty vector");
        return ptr[0];
    }

    T const& front() const {
        SATURN_ASSERT(size() > 0, "front() called on empty vector");
        return ptr[0];
    }

    T& back() {
        SATURN_ASSERT(size() > 0, "back() called on empty vector");
        return ptr[sz - 1];
    }

    T const& back() const {
        SATURN_ASSERT(size() > 0, "back() called on empty vector");
        return ptr[sz - 1];
    }

private:
    void grow_if_needed(size_t new_sz) {
        if (new_sz > cap) {
            reserve(max(new_sz, cap * 2));
        }
        sz = new_sz;
    }

    T* ptr = nullptr;
    size_t sz = 0;
    size_t cap = 0;
};

} // namespace stl

#endif