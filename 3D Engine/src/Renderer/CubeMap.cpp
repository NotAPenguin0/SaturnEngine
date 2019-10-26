#include "Renderer/CubeMap.hpp"

namespace Saturn {

CubeMap::~CubeMap() {
    if (handle) { glDeleteTextures(1, &handle); }
}

void CubeMap::assign(unsigned int new_handle) {
    if (handle) { glDeleteTextures(1, &handle); }
    handle = new_handle;
}

} // namespace Saturn
