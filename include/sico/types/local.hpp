#pragma once
#include "sico/types/orientations.hpp"
#include "sico/types/units.hpp"

namespace sico {

template<typename Unit>
struct vec_local {
    Unit right;
    Unit front;
    Unit up;

    template<typename OtherUnit>
    explicit operator OtherUnit() const;
};

using pos_local = vec_local<meters>;
using vel_local = vec_local<meters_ps>;
using acc_local = vec_local<meters_ps2>;

struct ref_local {
};
using ori_local  = ori<ref_local>;
using quat_local = quat<ref_local>;

template<typename U>
vec_local<U> operator+(vec_local<U> const& p1, vec_local<U> const& p2)
{
    return { p1.right + p2.right, p1.front + p2.front, p1.up + p2.up };
}
template<typename U>
vec_local<U> operator-(vec_local<U> const& p)
{
    return { -p.right, -p.front, -p.up };
}
template<typename U>
vec_local<U> operator-(vec_local<U> const& p1, vec_local<U> const& p2)
{
    return p1 + -p2;
}
template<typename U>
vec_local<U> operator*(vec_local<U> const& p1, double const f)
{
    return { p1.right * f, p1.front * f, p1.up * f };
}
template<typename U>
vec_local<U> operator/(vec_local<U> const& p1, double const f)
{
    return p1 * (1.0 / f);
}
template<typename U>
bool approx_eq(vec_local<U> const& p1, vec_local<U> const& p2, U p)
{
    return approx_eq(p1.right, p2.right, p) && approx_eq(p1.front, p2.front, p)
        && approx_eq(p1.up, p2.up, p);
}
template<typename U>
bool operator==(vec_local<U> const& p1, vec_local<U> const& p2)
{
    return approx_eq(p1, p2, U(0.0001));
}
template<typename U>
bool operator!=(vec_local<U> const& p1, vec_local<U> const& p2)
{
    return !(p1 == p2);
}

/// Euclidian norm
template<typename U>
U norm(vec_local<U> const& p)
{
    double const r = raw(p.right);
    double const f = raw(p.front);
    double const u = raw(p.up);
    return sqrt(r * r + f * f + u * u);
}

template<typename U>
U distance(vec_local<U> const& p1, vec_local<U> const& p2)
{
    return norm(p1 - p2);
}

/// Normalize angles in ]-pi;pi]
void normalize(ori_local& o);

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//