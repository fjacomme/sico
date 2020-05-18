#pragma once
#include "sico/types/forward.hpp"
#include "sico/types/units.hpp"

namespace sico {

/// Geodetic position
struct pos_lla {
    radians lat; /// Latitude in radians, positive north
    radians lon; /// Longitude in radians, positive east
    meters  alt; /// Altitude in meters, positive up

    /// Conversion to geocentric
    explicit operator vec_ecef<meters>() const;
};

/// Addition to a local tangent offset
pos_lla operator+(pos_lla const& p1, vec_enu<meters, ref_tangent> const& p2);
/// Substraction to a local tangent offset
pos_lla operator-(pos_lla const& p1, vec_enu<meters, ref_tangent> const& p2);

inline pos_lla operator+(pos_lla const& p1, pos_lla const& p2)
{
    return { p1.lat + p2.lat, p1.lon + p2.lon, p1.alt + p2.alt };
}
inline pos_lla operator-(pos_lla const& p)
{
    return { -p.lat, -p.lon, -p.alt };
}
inline pos_lla operator-(pos_lla const& p1, pos_lla const& p2)
{
    return p1 + -p2;
}
inline bool operator==(pos_lla const& p1, pos_lla const& p2)
{
    return approx_eq(p1.lat, p2.lat) && approx_eq(p1.lon, p2.lon) && approx_eq(p1.alt, p2.alt);
}
inline bool operator!=(pos_lla const& p1, pos_lla const& p2)
{
    return !(p1 == p2);
}
inline bool operator>(pos_lla const& p1, pos_lla const& p2)
{
    return p1.lat > p2.lat || p1.lon > p2.lon || p1.alt > p2.alt;
}
/// clamp lat in [-pi/2,pi/2] and normalize lon in ]-pi,pi]
void normalize(pos_lla& p);

/// clamp lat in [-pi/2,pi/2] and normalize lon in ]-pi,pi]
inline pos_lla normalized(pos_lla const& p)
{
    pos_lla r = p;
    normalize(r);
    return r;
}

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//