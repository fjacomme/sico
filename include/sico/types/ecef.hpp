#pragma once
#include "sico/types/lla.hpp"
#include "sico/types/units.hpp"

namespace sico {

/// Geocentric Earth-Centered/Earth-Fixed vector
template<typename Unit>
struct vec_ecef {
    Unit x;
    Unit y;
    Unit z;

    template<class OtherType>
    explicit operator OtherType() const;
};

/// Geocentric position
using pos_ecef = vec_ecef<meters>;
/// Geocentric velocity
using vel_ecef = vec_ecef<meters_ps>;
/// Geocentric acceleration
using acc_ecef = vec_ecef<meters_ps2>;

/// Geocentric Earth-Centered/Earth-Fixed orientation
struct ori_ecef {
    radians psi;
    radians theta;
    radians phi;
};

/// normalize angles in ]-pi;pi]
void normalize(ori_ecef& o);

/// normalize angles in ]-pi;pi]
inline ori_ecef normalized(ori_ecef const& o)
{
    ori_ecef r = o;
    normalize(r);
    return r;
}

template<typename U>
vec_ecef<U> operator+(vec_ecef<U> const& p1, vec_ecef<U> const& p2)
{
    return { p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
}
template<typename U>
vec_ecef<U> operator-(vec_ecef<U> const& p)
{
    return { -p.x, -p.y, -p.z };
}
template<typename U>
vec_ecef<U> operator-(vec_ecef<U> const& p1, vec_ecef<U> const& p2)
{
    return p1 + -p2;
}
template<typename U>
vec_ecef<U> operator*(vec_ecef<U> const& p1, double const f)
{
    return { p1.x * f, p1.y * f, p1.z * f };
}
template<typename U>
vec_ecef<U> operator/(vec_ecef<U> const& p1, double const f)
{
    return p1 * (1.0 / f);
}
template<typename U>
bool operator==(vec_ecef<U> const& p1, vec_ecef<U> const& p2)
{
    return approx_eq(p1.x, p2.x) && approx_eq(p1.y, p2.y) && approx_eq(p1.z, p2.z);
}
template<typename U>
bool operator!=(vec_ecef<U> const& p1, vec_ecef<U> const& p2)
{
    return !(p1 == p2);
}

/// Euclidian norm of the vector
template<typename U>
U norm(vec_ecef<U> const& p)
{
    double const x = raw(p.x);
    double const y = raw(p.y);
    double const z = raw(p.z);
    return sqrt(x * x + y * y + z * z);
}

template<typename U>
U distance(vec_ecef<U> const& p1, vec_ecef<U> const& p2)
{
    return norm(p1 - p2);
}

template<>
template<>
pos_ecef::operator pos_lla() const;

inline bool approx_eq(ori_ecef const& o1, ori_ecef const& o2, radians const prec = radians(1e-4))
{
    return approx_eq(o1.psi, o2.psi, prec) && approx_eq(o1.theta, o2.theta, prec)
        && approx_eq(o1.phi, o2.phi, prec);
}

inline bool operator==(ori_ecef const& o1, ori_ecef const& o2)
{
    return approx_eq(o1, o2);
}

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//