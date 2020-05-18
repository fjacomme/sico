#pragma once
#include "sico/types/enu.hpp"

namespace sico {

/// North-East-Down vector, for interoperability
template<typename Unit>
struct vec_ned {
    Unit north;
    Unit east;
    Unit down;

    explicit operator vec_enu<Unit, ref_tangent>() const { return { east, north, -down }; }
};

/// North-East-Down position, for interoperability
using pos_ned = vec_ned<meters>;
/// North-East-Down velocity, for interoperability
using vel_ned = vec_ned<meters_ps>;
/// North-East-Down acceleration, for interoperability
using acc_ned = vec_ned<meters_ps2>;

template<typename Unit, typename Ref>
inline vec_enu<Unit, Ref>::operator vec_ned<Unit>() const
{
    return { north, east, -up };
}

template<typename U>
bool operator==(vec_ned<U> const& p1, vec_ned<U> const& p2)
{
    return approx_eq(p1.north, p2.north) && approx_eq(p1.east, p2.east)
        && approx_eq(p1.down, p2.down);
}
template<typename U>
bool operator!=(vec_ned<U> const& p1, vec_ned<U> const& p2)
{
    return !(p1 == p2);
}

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//