#ifndef SATURN_CUBE_MAP_HPP_
#define SATURN_CUBE_MAP_HPP_

#include "OpenGL.hpp"

#include <utility>

namespace Saturn {

class CubeMap {
public:
    CubeMap() = default;
    CubeMap(unsigned int h) : handle(h) {}
    CubeMap(CubeMap const&) = delete;
    CubeMap& operator=(CubeMap const&) = delete;
    ~CubeMap();

    void assign(unsigned int new_handle);
    void swap(CubeMap& other) { std::swap(handle, other.handle); }

    unsigned int handle = 0;
};

} // namespace Saturn

#endif // ifndef SATURN_CUBE_MAP_HPP_
