#pragma once
#include "sico/types/forward.hpp"
#include "sico/types/units.hpp"

#include <cmath>

namespace sico {

/// Local orientation Tait-Bryan angles
template<typename Ref>
struct ori {
    radians yaw;
    radians pitch;
    radians roll;

    explicit operator quat<Ref>() const
    {
        quat<Ref> q;
        internal::ori_to_quat(yaw, pitch, roll, q.x, q.y, q.z, q.w);
        return q;
    }
};

/// copy version of normalize
template<typename Ref>
inline ori<Ref> normalized(ori<Ref> const& o)
{
    ori<Ref> r = o;
    normalize(r);
    return r;
}

template<typename Ref>
struct quat {
    double x, y, z, w;

    explicit operator ori<Ref>() const
    {
        ori<Ref> o;
        internal::quat_to_ori(x, y, z, w, o.yaw, o.pitch, o.roll);
    }
};

template<typename Ref>
inline bool approx_eq(ori<Ref> const& o1, ori<Ref> const& o2, radians const prec = radians(1e-4))
{
    return abs(angleDiff(o1.yaw, o2.yaw)) < prec && abs(angleDiff(o1.pitch, o2.pitch)) < prec
        && abs(angleDiff(o1.roll, o2.roll)) < prec;
}

template<typename Ref>
inline bool operator==(ori<Ref> const& o1, ori<Ref> const& o2)
{
    return approx_eq(o1, o2);
}

namespace internal {
void quat_to_ori(
    double x, double y, double z, double w, radians& yaw, radians& pitch, radians& roll);
void ori_to_quat(
    radians yaw, radians pitch, radians roll, double& x, double& y, double& z, double& w);

template<typename Ref>
ori<Ref> vec_to_ori(meters mx, meters my, meters mz)
{
    double const x = raw(mx);
    double const y = raw(my);
    double const z = raw(mz);
    return {
        radians(atan2(x, y)),
        radians(atan2(z, sqrt(x * x + y * y))),
        radians(0),
    };
}

template<typename Ref>
void ori_to_vec(ori<Ref> const& o, meters& x, meters& y, meters& z)
{
    auto const siny = sin(raw(o.yaw));
    auto const sinp = sin(raw(o.pitch));
    auto const cosy = cos(raw(o.yaw));
    auto const cosp = cos(raw(o.pitch));

    x = meters(siny * cosp);
    y = meters(cosy * cosp);
    z = meters(sinp);
}

template<typename Ref>
void ori_to_vec_up(ori<Ref> const& o, meters& x, meters& y, meters& z)
{
    auto const siny = sin(raw(o.yaw));
    auto const sinp = sin(raw(o.pitch));
    auto const sinr = sin(raw(-o.roll));
    auto const cosy = cos(raw(o.yaw));
    auto const cosp = cos(raw(o.pitch));
    auto const cosr = cos(raw(-o.roll));

    x = meters(-siny * sinp * cosr - cosy * sinr);
    y = meters(-cosy * sinp * cosr + siny * sinr);
    z = meters(cosp * cosr);
}

template<typename Ref>
void ori_to_vec_right(ori<Ref> const& o, meters& x, meters& y, meters& z)
{
    meters xa, ya, za;
    ori_to_vec(o, xa, ya, za);
    meters xb, yb, zb;
    ori_to_vec_up(o, xb, yb, zb);
    x = ya * raw(zb) - za * raw(yb);
    y = za * raw(xb) - xa * raw(zb);
    z = xa * raw(yb) - ya * raw(xb);
}
} // namespace internal
} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//