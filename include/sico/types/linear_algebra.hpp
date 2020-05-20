#pragma once
#include "sico/types/orientations.hpp"

#ifdef SICO_USE_EIGEN
#pragma warning(push)
#pragma warning(disable : 4365)
#pragma warning(disable : 4464)
#pragma warning(disable : 4514)
#pragma warning(disable : 4710)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 4820)
#pragma warning(disable : 5026)
#pragma warning(disable : 5027)
#pragma warning(disable : 5045)
#define EIGEN_PERMANENTLY_DISABLE_STUPID_WARNINGS
#include <Eigen/Dense>
#pragma warning(pop)

namespace sico {

using vector3d   = Eigen::Vector3d;
using matrix3d   = Eigen::Matrix3d;
using quaternion = Eigen::Quaterniond;

} // namespace sico

#else
#include "sico/types/angles.hpp"

#include <array>
#include <type_traits>

#pragma warning(push)
#pragma warning(disable : 5045) // spectre
namespace sico {

using scalar_t = double;

template<size_t S>
using arrayd = std::array<scalar_t, S>;

// Basic linear algebra based on arrays of doubles
using vector3d   = arrayd<3>;
using matrix3d   = arrayd<9>;
using quaternion = arrayd<4>;

// Beneric operations
template<size_t S>
arrayd<S> operator+(arrayd<S> const& s1, arrayd<S> const& s2)
{
    arrayd<S> s;
    for (size_t i = 0; i < S; i++)
        s[i] = s1[i] + s2[i];
    return s;
}
template<size_t S>
arrayd<S>& operator+=(arrayd<S>& s1, arrayd<S> const& s2)
{
    for (size_t i = 0; i < S; i++)
        s1[i] += s2[i];
    return s1;
}
template<size_t S>
arrayd<S>& operator+=(arrayd<S>& s1, scalar_t const v)
{
    for (size_t i = 0; i < S; i++)
        s1[i] += v;
    return s1;
}
template<size_t S>
arrayd<S> operator-(arrayd<S> const& s1)
{
    arrayd<S> s;
    for (size_t i = 0; i < S; i++)
        s[i] = -s1[i];
    return s;
}
template<size_t S>
arrayd<S> operator*(arrayd<S> const& s1, scalar_t const t)
{
    arrayd<S> s;
    for (size_t i = 0; i < S; i++)
        s[i] = s1[i] * t;
    return s;
}
template<size_t S>
arrayd<S>& operator*=(arrayd<S>& s, scalar_t const t)
{
    for (size_t i = 0; i < S; i++)
        s[i] *= t;
    return s;
}
template<size_t S>
bool operator==(arrayd<S> const& s1, arrayd<S> const& s2)
{
    bool r = true;
    for (size_t i = 0; i < S; i++)
        r = r && s1[i] == s2[i];
    return r;
}
template<size_t S>
scalar_t dot(arrayd<S> const& s1, arrayd<S> const& s2)
{
    scalar_t d = 0;
    for (size_t i = 0; i < S; i++) {
        d += s1[i] * s2[i];
    }
    return d;
}

// generic operations, based on previously defined ones
template<size_t S>
arrayd<S> operator-(arrayd<S> const& s1, arrayd<S> const& s2)
{
    return s1 + -s2;
}
template<size_t S>
arrayd<S>& operator-=(arrayd<S>& s1, arrayd<S> const& s2)
{
    s1 += -s2;
    return s1;
}
template<size_t S>
arrayd<S>& operator-=(arrayd<S>& s1, scalar_t const v)
{
    s1 += -v;
    return s1;
}
template<size_t S>
arrayd<S> operator/(arrayd<S> const& s1, scalar_t const d)
{
    return s1 * (1.0 / d);
}
template<size_t S>
arrayd<S>& operator/=(arrayd<S>& s1, scalar_t const d)
{
    s1 *= 1.0 / d;
    return s1;
}
template<size_t S>
bool operator!=(arrayd<S> const& s1, arrayd<S> const& s2)
{
    return !(s1 == s2);
}
template<size_t S>
scalar_t length_squared(arrayd<S> const& s)
{
    return dot(s, s);
}
template<size_t S>
scalar_t length(arrayd<S> const& s)
{
    return sqrt(length_squared(s));
}
template<size_t S>
arrayd<S>& normalize(arrayd<S>& s)
{
    auto const l = length(s);
    if (l < 0.0001)
        return s;
    s /= l;
    return s;
}
template<size_t S>
arrayd<S> normalized(arrayd<S> s)
{
    normalize(s);
    return s;
}

// quaternion operations
inline quaternion operator*(quaternion const& q1, quaternion const& q2)
{
    return { q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1],
             q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2],
             q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0],
             q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] };
}
inline quaternion conj(quaternion const& q)
{
    return { -q[0], -q[1], -q[2], q[3] };
}
inline vector3d operator*(quaternion const& rot, vector3d const& v)
{
    quaternion const pure { v[0], v[1], v[2], 0.0 };
    quaternion const rot_conj = conj(rot);
    quaternion const temp { pure[3] * rot_conj[0] + pure[0] * rot_conj[3] + pure[1] * rot_conj[2]
                                - pure[2] * rot_conj[1],
                            pure[3] * rot_conj[1] + pure[1] * rot_conj[3] + pure[2] * rot_conj[0]
                                - pure[0] * rot_conj[2],
                            pure[3] * rot_conj[2] + pure[2] * rot_conj[3] + pure[0] * rot_conj[1]
                                - pure[1] * rot_conj[0],
                            pure[3] * rot_conj[3] - pure[0] * rot_conj[0] - pure[1] * rot_conj[1]
                                - pure[2] * rot_conj[2] };

    return { rot[3] * temp[0] + rot[0] * temp[3] + rot[1] * temp[2] - rot[2] * temp[1],
             rot[3] * temp[1] + rot[1] * temp[3] + rot[2] * temp[0] - rot[0] * temp[2],
             rot[3] * temp[2] + rot[2] * temp[3] + rot[0] * temp[1] - rot[1] * temp[0] };
}
inline quaternion from_euler(scalar_t xRot, scalar_t yRot, scalar_t zRot)
{
    using std::cos;
    using std::sin;
    auto const       hX = xRot * 0.5;
    auto const       hY = yRot * 0.5;
    auto const       hZ = zRot * 0.5;
    quaternion const qx { sin(hX), 0.0, 0.0, cos(hX) };
    quaternion const qy { 0.0, sin(hY), 0.0, cos(hY) };
    quaternion const qz { 0.0, 0.0, sin(hZ), cos(hZ) };
    quaternion       result = qx * qy * qz;
    normalize(result);
    return result;
}

// matrix operations
inline double& mat(matrix3d& m, size_t row, size_t column)
{
    return m[row * 3 + column];
}
inline double mat(matrix3d const& m, size_t row, size_t column)
{
    return m[row * 3 + column];
}
inline vector3d operator*(matrix3d const& m, vector3d const& v)
{
    vector3d res;
    res.fill(0);
    for (unsigned r = 0; r < 3; ++r)
        for (unsigned c = 0; c < 3; ++c)
            res[r] += mat(m, r, c) * v[c];

    return res;
}
inline matrix3d rot_mat(quaternion const& q)
{
    auto const xs = q[0] + q[0];
    auto const ys = q[1] + q[1];
    auto const zs = q[2] + q[2];
    auto const xx = q[0] * xs;
    auto const xy = q[0] * ys;
    auto const xz = q[0] * zs;
    auto const yy = q[1] * ys;
    auto const yz = q[1] * zs;
    auto const zz = q[2] * zs;
    auto const wx = q[3] * xs;
    auto const wy = q[3] * ys;
    auto const wz = q[3] * zs;

    matrix3d m;
    mat(m, 0, 0) = double(1.0) - (yy + zz);
    mat(m, 1, 0) = xy + wz;
    mat(m, 2, 0) = xz - wy;
    mat(m, 0, 1) = xy - wz;
    mat(m, 1, 1) = double(1.0) - (xx + zz);
    mat(m, 2, 1) = yz + wx;
    mat(m, 0, 2) = xz + wy;
    mat(m, 1, 2) = yz - wx;
    mat(m, 2, 2) = double(1.0) - (xx + yy);
    return m;
}
inline void to_euler(matrix3d const& m, scalar_t& x, scalar_t& y, scalar_t& z)
{
    y = asin(mat(m, 0, 2));
    if (y < SICO_PI2) {
        if (y > -SICO_PI2) {
            x = atan2(-mat(m, 1, 2), mat(m, 2, 2));
            z = atan2(-mat(m, 0, 1), mat(m, 0, 0));
        }
        else {
            x = -atan2(mat(m, 1, 0), mat(m, 1, 1));
            z = 0;
        }
    }
    else {
        x = atan2(mat(m, 1, 0), mat(m, 1, 1));
        z = 0;
    }
}

} // namespace sico
#pragma warning(pop)
#endif

namespace sico {
template<typename Ref>
inline quaternion make_quat(quat<Ref> const& q)
{
#ifdef SICO_USE_EIGEN
    return quaternion(q.w, q.x, q.y, q.z);
#else
    return quaternion { q.x, q.y, q.z, q.w };
#endif
}

template<typename Ref>
inline quat<Ref> make_quat(quaternion const& q)
{
#ifdef SICO_USE_EIGEN
    return quat<Ref> { q.x(), q.y(), q.z(), q.w() };
#else
    return quat<Ref> { q[0], q[1], q[2], q[3] };
#endif
}
} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//