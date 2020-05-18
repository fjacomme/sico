#pragma once
#include "sico/types/units.hpp"

#include <cmath>

/// Pi
#define SICO_PI 3.141592653589793238462643383279
/// Pi / 2
#define SICO_PI2 1.570796326794896619231321691639
/// 2 * Pi
#define SICO_2PI 6.283185307179586476925286766559

namespace sico {

/// Pi
inline radians pi()
{
    return radians(SICO_PI);
}

/// Pi / 2
inline radians pi_over_2()
{
    return radians(SICO_PI2);
}

/// Pi * 2
inline radians pi_times_2()
{
    return radians(SICO_2PI);
}

/// Generic angle normalization between ]-limit,limit]
template<class T>
T normalizeAngle(T angle, T limit)
{
    auto const l = raw(limit + limit);
    auto const a = std::remainder(raw(angle), l);
    return T(a) == -limit ? limit : T(a);
}

/// Generic angle normalization between [min, max[
template<class T>
T normalizeAngle(T angle, T min, T max)
{
    auto const l = raw(max - min);
    auto const a = std::remainder(raw(angle), l);
    return T(a) < min ? T(a + l) : T(a);
}

/// Normalize between ]-180,180]
inline degrees normalizeAngle180(degrees angle)
{
    return normalizeAngle(angle, degrees(180));
}

/// Normalize between ]-pi;pi]
inline radians normalizeAnglePi(radians angle)
{
    return normalizeAngle(angle, radians(SICO_PI));
}

/// Normalize between [0;360[
inline degrees normalizeAngle360(degrees angle)
{
    return normalizeAngle(angle, degrees(0), degrees(360));
}

/// Normalize between [0;2pi[
inline radians normalizeAngle2Pi(radians angle)
{
    return normalizeAngle(angle, radians(0), radians(SICO_2PI));
}

/// Generic oriented angle difference
template<class T>
T angleDiff(T angle1, T angle2, T rightA)
{
    T const flat = rightA + rightA;
    angle1       = normalizeAngle(angle1, flat);
    angle2       = normalizeAngle(angle2, flat);
    if (angle1 > rightA && angle2 < -rightA) {
        return (flat - angle1) + (flat + angle2);
    }
    else if (angle1 < -rightA && angle2 > rightA) {
        return -(flat + angle1) - (flat - angle2);
    }
    else {
        return angle2 - angle1;
    }
}

/// Oriented angle difference in radians
inline radians angleDiff(radians angle1_rad, radians angle2_rad)
{
    return angleDiff(angle1_rad, angle2_rad, radians(SICO_PI2));
}

/// Oriented angle difference in degrees
inline degrees angleDiff(degrees angle1_deg, degrees angle2_deg)
{
    return angleDiff(angle1_deg, angle2_deg, degrees(90));
}

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//