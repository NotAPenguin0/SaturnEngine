#ifndef MVG_RESOURCE_HPP_
#define MVG_RESOURCE_HPP_

#include <cstddef>

namespace Saturn {

template<typename R>
class Resource {
public:
    Resource() : ptr(nullptr), id(-1), loaded(false) {}
    Resource(R* res, std::size_t i, bool load) :
        ptr(res), id(i), loaded(load) {}

    Resource(Resource const& rhs) :
        ptr(rhs.ptr), id(rhs.id), loaded(rhs.loaded) {}

    Resource& operator=(Resource const& rhs) {
        ptr = rhs.ptr;
        id = rhs.id;
        loaded = rhs.loaded;

        return *this;
    }

    R& get() { return *ptr; }
    R const& get() const { return *ptr; }

    std::size_t get_id() const { return id; }
    bool is_loaded() const { return loaded; }

    R* operator->() { return ptr; }

    R const* operator->() const { return ptr; }

private:
    R* ptr;
    std::size_t id;
    bool loaded;
};

} // namespace Saturn

#endif
