#pragma once

#if defined(SICO_USE_HOLTHAUS_UNITS)

#pragma warning(push)
#pragma warning(disable : 4514) // deleted inline functions
#pragma warning(disable : 4710) // deleted inline functions
#pragma warning(disable : 4820) // fill bytes in time.h
#include <units/units.h>
#pragma warning(pop)
namespace sico {
namespace literals = units::literals;

using meters     = units::length::meter_t;
using meters_ps  = units::velocity::meters_per_second_t;
using meters_ps2 = units::acceleration::meters_per_second_squared_t;
using radians    = units::angle::radian_t;
using radians_ps = units::angular_velocity::radians_per_second_t;
using degrees    = units::angle::degree_t;
using seconds    = units::time::second_t;

template<class U>
double raw(U const& v)
{
    return v.value();
}

} // namespace sico
#elif defined(SICO_USE_BOOST_UNITS)
#include <boost/units/cmath.hpp>
#include <boost/units/io.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/systems/si/acceleration.hpp>
#include <boost/units/systems/si/angular_velocity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/si/time.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <cmath>

namespace sico {
namespace units = boost::units;
template<typename T>
struct q : units::quantity<T> {
    explicit q(double v = 0.)
        : units::quantity<T>(v * T::unit())
    {
    }
    q(units::quantity<T> const& o)
        : units::quantity<T>(o)
    {
    }
    template<typename T2>
    q(units::quantity<T2> const& o);
    explicit operator double() const { return value(); }
    template<typename T2>
    q<T>& operator=(units::quantity<T2> o)
    {
        *this = (q<T>)o;
        return *this;
    }
    template<typename T2>
    operator units::quantity<T2>() const;
};
using meters     = q<units::si::length>;
using meters_ps  = q<units::si::velocity>;
using meters_ps2 = q<units::si::acceleration>;
using radians    = q<units::si::plane_angle>;
using radians_ps = q<units::si::angular_velocity>;
using degrees    = q<units::degree::plane_angle>;
using seconds    = q<units::si::time>;
template<typename T>
double raw(units::quantity<T> const& v)
{
    return v.value();
}
template<>
template<>
inline q<units::si::plane_angle>::q(units::quantity<units::degree::plane_angle> const& v)
    : q<units::si::plane_angle>(v.value() * 0.01745329251994329576923690768489)
{
}
template<>
template<>
inline q<units::degree::plane_angle>::q(units::quantity<units::si::plane_angle> const& v)
    : q<units::degree::plane_angle>(v.value() * 57.295779513082320876798154814105)
{
}
/*template<>
template<>
inline radians::operator units::quantity<units::degree::plane_angle>() const
{
    return degrees { value() * 57.295779513082320876798154814105 };
}
template<>
template<>
inline degrees::operator units::quantity<units::si::plane_angle>() const
{
    return radians { value() * 0.01745329251994329576923690768489 };
}*/
namespace literals {
inline meters operator"" _m(long double v)
{
    return meters((double)v);
}
inline seconds operator"" _s(long double v)
{
    return seconds { (double)v };
}
inline degrees operator"" _deg(long double v)
{
    return degrees { (double)v };
}
inline radians operator"" _rad(long double v)
{
    return radians { (double)v };
}
inline meters operator"" _m(unsigned long long int v)
{
    return meters { (double)v };
}
inline seconds operator"" _s(unsigned long long int v)
{
    return seconds { (double)v };
}
inline degrees operator"" _deg(unsigned long long int v)
{
    return degrees { (double)v };
}
inline radians operator"" _rad(unsigned long long int v)
{
    return radians { (double)v };
}
} // namespace literals
} // namespace sico

#else
#include <cmath>
#include <ostream>

namespace sico {
namespace units {
/// basic quantity class
template<typename T>
struct q {
    q()
        : v(0)
    {
    }
    q(double v)
        : v(v)
    {
    }
    double   v;
    explicit operator double() const { return v; }

    q  operator+(q const& o) const { return { v + o.v }; }
    q& operator+=(q const& o)
    {
        v += o.v;
        return *this;
    }
    q  operator-(q const& o) const { return { v - o.v }; }
    q& operator-=(q const& o)
    {
        v -= o.v;
        return *this;
    }
    q  operator-() const { return { -v }; }
    q  operator*(double f) const { return { v * f }; }
    q& operator*=(double f)
    {
        v *= f;
        return *this;
    }
    q  operator/(double f) const { return { v / f }; }
    q& operator/=(double f)
    {
        v /= f;
        return *this;
    }
    bool operator>(q const& o) const { return { v > o.v }; }
    bool operator>=(q const& o) const { return { v >= o.v }; }
    bool operator<(q const& o) const { return { v < o.v }; }
    bool operator<=(q const& o) const { return { v <= o.v }; }
    bool operator==(q const& o) const { return { v == o.v }; }
    bool operator!=(q const& o) const { return { v != o.v }; }
    template<typename T2>
    operator q<T2>() const;
};
template<typename T>
q<T> operator+(double f, q<T> const& u)
{
    return u + f;
}
template<typename T>
q<T> operator-(double f, q<T> const& u)
{
    return f + (-u);
}
template<typename T>
q<T> operator*(double f, q<T> const& u)
{
    return u * f;
}
template<typename T>
q<T> operator/(double f, q<T> const& u)
{
    return { f / u.v };
}
template<typename T>
q<T> abs(q<T> const& t)
{
    return { std::abs(t.v) };
}

struct meter {
};
struct meter_per_second {
};
struct meter_per_second_squared {
};
struct radians_per_second {
};
struct second {
};
struct radian {
};
struct degree {
};
} // namespace units

template<class U>
double raw(units::q<U> const& v)
{
    return v.v;
}

using meters     = units::q<units::meter>;
using meters_ps  = units::q<units::meter_per_second>;
using meters_ps2 = units::q<units::meter_per_second_squared>;
using radians_ps = units::q<units::radians_per_second>;
using seconds    = units::q<units::second>;
using radians    = units::q<units::radian>;
using degrees    = units::q<units::degree>;

namespace units {
inline std::ostream& operator<<(std::ostream& os, meters const& u)
{
    return os << u.v << " m";
}
inline std::ostream& operator<<(std::ostream& os, meters_ps const& u)
{
    return os << u.v << " m/s";
}
inline std::ostream& operator<<(std::ostream& os, seconds const& u)
{
    return os << u.v << " s";
}
inline std::ostream& operator<<(std::ostream& os, radians const& u)
{
    return os << u.v << " rad";
}
inline std::ostream& operator<<(std::ostream& os, degrees const& u)
{
    return os << u.v << " deg";
}
template<>
template<>
inline q<radian>::operator q<degree>() const
{
    return { v * 57.295779513082320876798154814105 };
}
template<>
template<>
inline q<degree>::operator q<radian>() const
{
    return { v * 0.01745329251994329576923690768489 };
}
} // namespace units
inline meters operator*(meters_ps const& v, seconds const& s)
{
    return { v.v * s.v };
}
inline meters_ps operator*(meters_ps2 const& v, seconds const& s)
{
    return { v.v * s.v };
}
inline meters_ps operator/(meters const& v, seconds const& s)
{
    return { v.v / s.v };
}
inline meters_ps2 operator/(meters_ps const& v, seconds const& s)
{
    return { v.v / s.v };
}
namespace literals {
inline meters operator"" _m(long double v)
{
    return { (double)v };
}
inline seconds operator"" _s(long double v)
{
    return { (double)v };
}
inline degrees operator"" _deg(long double v)
{
    return { (double)v };
}
inline radians operator"" _rad(long double v)
{
    return { (double)v };
}
inline meters operator"" _m(unsigned long long int v)
{
    return { (double)v };
}
inline seconds operator"" _s(unsigned long long int v)
{
    return { (double)v };
}
inline degrees operator"" _deg(unsigned long long int v)
{
    return { (double)v };
}
inline radians operator"" _rad(unsigned long long int v)
{
    return { (double)v };
}
} // namespace literals
} // namespace sico
#endif

namespace sico {

/// Used to compare positions to the millimeter
inline bool approx_eq(meters const& v1, meters const& v2, meters const& p = meters(0.001))
{
    return abs(v1 - v2) < p;
}
/// Used to compare lat/lon in radians with around 6 millimiters precision
inline bool approx_eq(radians const& v1, radians const& v2, radians const& p = radians(1e-9))
{
    return abs(v1 - v2) < p;
}

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//