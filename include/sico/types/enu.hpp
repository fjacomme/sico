#pragma once
#include "sico/types/forward.hpp"
#include "sico/types/orientations.hpp"
#include "sico/types/units.hpp"

namespace sico {

/// East-North-Up vector
template<typename Unit, typename Ref>
struct vec_enu {
    Unit east;
    Unit north;
    Unit up;

    explicit operator vec_ned<Unit>() const;
};

struct ref_tangent {
};
struct ref_mercator {
};

/// Local ENU position
using pos_enu = vec_enu<meters, ref_tangent>;
/// Local ENU velocity
using vel_enu = vec_enu<meters_ps, ref_tangent>;
/// Local ENU acceleration
using acc_enu = vec_enu<meters_ps2, ref_tangent>;

/// Mercator ENU position
using pos_enu_m = vec_enu<meters, ref_mercator>;
/// Mercator ENU velocity
using vel_enu_m = vec_enu<meters_ps, ref_mercator>;
/// Mercator ENU acceleration
using acc_enu_m = vec_enu<meters_ps2, ref_mercator>;

struct ref_enu {
};
using ori_enu  = ori<ref_enu>;
using quat_enu = quat<ref_enu>;

template<typename U, typename Ref>
vec_enu<U, Ref> operator+(vec_enu<U, Ref> const& p1, vec_enu<U, Ref> const& p2)
{
    return { p1.east + p2.east, p1.north + p2.north, p1.up + p2.up };
}
template<typename U, typename Ref>
vec_enu<U, Ref> operator-(vec_enu<U, Ref> const& p)
{
    return { -p.east, -p.north, -p.up };
}
template<typename U, typename Ref>
vec_enu<U, Ref> operator-(vec_enu<U, Ref> const& p1, vec_enu<U, Ref> const& p2)
{
    return p1 + -p2;
}
template<typename U, typename Ref>
vec_enu<U, Ref> operator*(vec_enu<U, Ref> const& p1, double const f)
{
    return { p1.east * f, p1.north * f, p1.up * f };
}
template<typename U, typename Ref>
vec_enu<U, Ref> operator/(vec_enu<U, Ref> const& p1, double const f)
{
    return p1 * (1.0 / f);
}
template<typename U, typename Ref>
bool approx_eq(vec_enu<U, Ref> const& p1, vec_enu<U, Ref> const& p2, U p)
{
    return approx_eq(p1.east, p2.east, p) && approx_eq(p1.north, p2.north, p)
        && approx_eq(p1.up, p2.up, p);
}
template<typename U, typename Ref>
bool operator==(vec_enu<U, Ref> const& p1, vec_enu<U, Ref> const& p2)
{
    return approx_eq(p1, p2, U(0.001)); // millimeter precision is usually good enough
}
template<typename U, typename Ref>
bool operator!=(vec_enu<U, Ref> const& p1, vec_enu<U, Ref> const& p2)
{
    return !(p1 == p2);
}
inline vel_enu operator/(pos_enu const& p, seconds s)
{
    return { p.east / s, p.north / s, p.up / s };
}
inline acc_enu operator/(vel_enu const& p, seconds s)
{
    return { p.east / s, p.north / s, p.up / s };
}
inline pos_enu operator*(vel_enu const& p, seconds s)
{
    return { p.east * s, p.north * s, p.up * s };
}
inline vel_enu operator*(acc_enu const& p, seconds s)
{
    return { p.east * s, p.north * s, p.up * s };
}

/// Euclidian norm
template<typename U, typename Ref>
U norm(vec_enu<U, Ref> const& p)
{
    double const e = raw(p.east);
    double const n = raw(p.north);
    double const u = raw(p.up);
    return sqrt(e * e + n * n + u * u);
}

template<typename U, typename Ref>
U distance(vec_enu<U, Ref> const& p1, vec_enu<U, Ref> const& p2)
{
    return norm(p1 - p2);
}

/// Normalize yaw in [0;2pi[ and pitch/roll in ]-pi;pi]
void normalize(ori_enu& o);

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//