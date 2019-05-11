#ifndef MVG_CAMERA_TRANSFORM_HPP_
#define MVG_CAMERA_TRANSFORM_HPP_

#include "Matrix.hpp"
#include "VecMath.hpp"
#include "Vector.hpp"

namespace Saturn::Math {

namespace Transform {

template<typename T>
Matrix4x4<T> look_at(Vec3<T> const& position,
                     Vec3<T> const& target,
                     Vec3<T> const& up = UpVec3<float>) {
    Vec3<T> direction = normalize(position - target);
    Vec3<T> left = normalize(cross(up, direction));
    Matrix4x4<T> result;

    result(0, 0) = left.x;
    result(1, 0) = left.y;
    result(2, 0) = left.z;
    result(0, 1) = up.x;
    result(1, 1) = up.y;
    result(2, 1) = up.z;
    result(0, 2) = direction.x;
    result(1, 2) = direction.y;
    result(2, 2) = direction.z;
    result(3, 3) = 1.0; // last element must always be 1

    Matrix4x4<T> pos_matrix = Matrix4x4<T>::identity();
    pos_matrix(3, 0) = -position.x;
    pos_matrix(3, 1) = -position.y;
    pos_matrix(3, 2) = -position.z;

    return result * pos_matrix;
}

} // namespace Transform

} // namespace Saturn::Math

#endif
