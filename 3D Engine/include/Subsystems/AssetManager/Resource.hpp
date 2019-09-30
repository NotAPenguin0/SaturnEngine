#ifndef SATURN_RESOURCE_HPP_
#define SATURN_RESOURCE_HPP_

#include <cstddef>

#include <string>

namespace Saturn {

template<typename R>
class Resource {
public:
    Resource() : ptr(nullptr), id(-1), loaded(false) {}
    Resource(R* res, std::size_t i, bool load, std::string path) :
        ptr(res), id(i), loaded(load), path(path) {}

    Resource(Resource const& rhs) :
        ptr(rhs.ptr), id(rhs.id), loaded(rhs.loaded), path(rhs.path) {}

    Resource& operator=(Resource const& rhs) {
        ptr = rhs.ptr;
        id = rhs.id;
        loaded = rhs.loaded;
        path = rhs.path;

        return *this;
    }

    ~Resource() {}

    R& get() { return *ptr; }
    R const& get() const { return *ptr; }

    std::size_t get_id() const { return id; }
    bool is_loaded() const { return loaded; }

    R* operator->() { return ptr; }

    R const* operator->() const { return ptr; }

    R& operator*() { return *ptr; };
    R const& operator*() const { return *ptr; }

    std::string get_path() const { return path; }

private:
    R* ptr;
    std::size_t id;
    bool loaded;
    std::string path;
};

} // namespace Saturn

#endif
