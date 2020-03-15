#ifndef SATURN_EDITOR_CAMERA_COMPONENT_HPP_
#define SATURN_EDITOR_CAMERA_COMPONENT_HPP_

namespace saturn::components {

struct [[component]] EditorCamera {
    float sensitivity = 20.0f;
};

}

#endif