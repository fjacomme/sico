#include "sico/conversions/ecef_lla.hpp"

#pragma warning(disable : 5045) // spectre
#include <cmath>

using namespace sico;

//
// ECEF/LLA using Olson's algorithm
//
namespace {
struct {
    double const a  = 6378137.0;
    double const e2 = 6.6943799901377997e-3; // first eccentricity
    double const a1 = 4.2697672707157535e+4; // a1 = a*e2
    double const a2 = 1.8230912546075455e+9; // a2 = a1*a1
    double const a3 = 1.4291722289812413e+2; // a3 = a1*e2/2
    double const a4 = 4.5577281365188637e+9; // a4 = 2.5*a2
    double const a5 = 4.2840589930055659e+4; // a5 = a1+a3
    double const a6 = 9.9330562000986220e-1; // a6 = 1-e2
} Earth;
}

pos_lla sico::to_lla(pos_ecef const& from)
{
    auto const x   = raw(from.x);
    auto const y   = raw(from.y);
    auto const z   = raw(from.z);
    auto const lon = atan2(y, x);

    auto const absz   = abs(z);
    auto const x2y2   = pow(x, 2.0) + pow(y, 2.0);
    auto const sqx2y2 = sqrt(x2y2);
    auto const z2     = pow(z, 2.0);
    auto const r2     = x2y2 + z2;
    auto const r      = sqrt(r2);
    auto const s2     = z2 / r2;
    auto const c2     = x2y2 / r2;
    double     c, s, ss;
    double     lat;
    {
        auto const u = Earth.a2 / r;
        auto const v = Earth.a3 - Earth.a4 / r;

        if (c2 > 0.3) {
            s   = (absz / r) * (1.0 + c2 * (Earth.a1 + u + s2 * v) / r);
            lat = asin(s);
            ss  = s * s;
            c   = sqrt(1.0 - ss);
        }
        else {
            c   = (sqx2y2 / r) * (1.0 - s2 * (Earth.a5 - u - c2 * v) / r);
            lat = acos(c);
            ss  = 1.0 - c * c;
            s   = sqrt(ss);
        }
    }
    auto const g     = 1.0 - Earth.e2 * ss;
    auto const rg    = Earth.a / sqrt(g);
    auto const rf    = Earth.a6 * rg;
    auto const u     = sqx2y2 - rg * c;
    auto const v     = absz - rf * s;
    auto const f     = c * u + s * v;
    auto const m     = c * v - s * u;
    auto const p     = m / (rf / g + f);
    lat              = lat + p;
    double const alt = f + m * p / 2.0;
    if (z < 0.0) {
        lat *= -1.0;
    }

    pos_lla to;
    to.lat = radians(lat);
    to.lon = radians(lon);
    to.alt = meters(alt);
    return to;
}

pos_ecef sico::to_ecef(pos_lla const& from)
{
    pos_ecef   to;
    auto const lat = raw(from.lat);
    auto const lon = raw(from.lon);
    auto const alt = raw(from.alt);

    auto const coslat  = cos(lat);
    auto const coslon  = cos(lon);
    auto const sinlat  = sin(lat);
    auto const sinlon  = sin(lon);
    auto const sin2lat = pow(sinlat, 2.0);

    auto const n = Earth.a / sqrt(1.0 - Earth.e2 * sin2lat);
    auto const c = (n + alt) * coslat;

    to.x = meters(c * coslon);
    to.y = meters(c * sinlon);
    to.z = meters((n * (1 - Earth.e2) + alt) * sinlat);
    return to;
}
