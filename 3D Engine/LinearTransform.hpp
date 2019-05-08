#ifndef MVG_LINEAR_TRANSFORM_HPP_
#define MVG_LINEAR_TRANSFORM_HPP_

#include "Matrix.hpp"
#include "Vector.hpp"

#include <cmath>

namespace Saturn {

namespace Math {

namespace Transform {

#define ENSURE_FLOATING_POINT(T)                                               \
    static_assert(std::is_floating_point_v<T>,                                 \
                  "Saturn::Math: Template type must be floating point type")

template<typename T>
constexpr Matrix4x4<T> translate(T x, T y, T z) {
    ENSURE_FLOATING_POINT(T);

    Matrix4x4<T> result = Matrix4x4<T>::identity();
    result(3, 0) = x;
    result(3, 1) = y;
    result(3, 2) = z;
    return result;
}

template<typename T>
constexpr Matrix4x4<T> translate(Vec3<T> const& input) {
    return translate(input.x, input.y, input.z);
}

template<typename T>
constexpr void add_translation(Matrix4x4<T>& mat, T x, T y, T z) {
    mat *= translate(x, y, z);
}

template<typename T>
constexpr void add_translation(Matrix4x4<T>& mat, Vec3<T> const& input) {
    mat *= translate(input.x, input.y, input.z);
}

template<typename T>
constexpr Matrix4x4<T> rotate(T x, T y, T z, T angle_in_radians) {
    ENSURE_FLOATING_POINT(T);

    // Easier to write
    T const& theta = angle_in_radians;

    // Build the matrix
    Matrix4x4<T> result;
    // First row
    auto ct = std::cos(theta);
    auto st = std::sin(theta);
	
    result(0, 0) = ct + x * x * (1 - ct);
    result(1, 0) = x * y * (1 - ct) - z * st;
    result(2, 0) = x * z * (1 - ct) + y * st;
    // Second row
    result(0, 1) = x * y * (1 - ct) + z * st;
    result(1, 1) = ct + y * y * (1 - ct);
    result(2, 1) = y * z * (1 - ct) - x * st;
    // Third row
    result(0, 2) = z * x * (1 - ct) - y * st;
    result(1, 2) = z * y * (1 - ct) + x * st;
    result(2, 2) = ct + z * z * (1 - ct);
    // Last row is all zeros except last element,
    // which should always be 1
    result(3, 3) = 1;

    return result;
}

template<typename T>
constexpr Matrix4x4<T> rotate(Vec3<T> const& axis, T angle_in_radians) {
    return rotate(axis.x, axis.y, axis.z, angle_in_radians);
}

template<typename T>
constexpr void
add_rotation(Matrix4x4<T>& mat, T x, T y, T z, T angle_in_radians) {
    mat *= rotate(x, y, z, angle_in_radians);
}

template<typename T>
constexpr void
add_rotation(Matrix4x4<T>& mat, Vec3<T> const& input, T angle_in_radians) {
    mat *= rotate(input.x, input.y, input.z, angle_in_radians);
}

template<typename T>
constexpr Matrix4x4<T> scale(T x, T y, T z) {
    ENSURE_FLOATING_POINT(T);
    Matrix4x4<T> result;
    result(0, 0) = x;
    result(1, 1) = y;
    result(2, 2) = z;
    result(3, 3) = 1.0;
    return result;
}

template<typename T>
constexpr Matrix4x4<T> scale(Vec3<T> const& input) {
    return scale(input.x, input.y, input.z);
}

template<typename T>
constexpr void add_scale(Matrix4x4<T>& mat, T x, T y, T z) {
    mat *= scale(x, y, z);
}

template<typename T>
constexpr void add_scale(Matrix4x4<T>& mat, Vec3<T> const& input) {
    mat *= scale(input.x, input.y, input.z);
}

template<typename T>
constexpr Matrix4x4<T>
perspective(T left, T right, T bottom, T top, T near_z, T far_z) {
    ENSURE_FLOATING_POINT(T);

    Matrix4x4<T> result;

    /*
     *For information on how to construct the perspective matrix, see
     *http://www.songho.ca/opengl/gl_projectionmatrix.html
     **/

    result(0, 0) = (2 * near_z) / (right - left);
    result(2, 0) = (right + left) / (right - left);
    result(1, 1) = (2 * near_z) / (top - bottom);
    result(2, 1) = (top + bottom) / (top - bottom);
    result(2, 2) = -(far_z + near_z) / (far_z - near_z);
    result(3, 2) = (-2 * far_z * near_z) / (far_z - near_z);
    result(2, 3) = -1;
    return result;
}

template<typename T>
constexpr Matrix4x4<T>
perspective(T fov_in_radians, T aspect_ratio, T near_z, T far_z) {
    ENSURE_FLOATING_POINT(T);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
    Matrix4x4<T> result;
    T inv_tan = 1.0 / std::tan(fov_in_radians / 2);
    result(0, 0) = inv_tan / aspect_ratio;
    result(1, 1) = inv_tan;
    result(2, 2) = -(far_z + near_z) / (far_z - near_z);
    result(3, 2) = (-2 * far_z * near_z) / (far_z - near_z);
    result(2, 3) = -1;
#pragma clang diagnostic pop
    return result;
}

#undef ENSURE_FLOATING_POINT

} // namespace Transform

} // namespace Math

} // namespace Saturn

#endif
