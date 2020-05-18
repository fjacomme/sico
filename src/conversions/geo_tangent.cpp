#include "sico/conversions/geo_tangent.hpp"

#include "sico/types/linear_algebra.hpp"

using namespace sico;

static pos_enu internal_to_enu(pos_ecef const& ref, pos_ecef const& pecef, pos_lla const& p)
{
    auto const sinLat = sin(raw(p.lat));
    auto const cosLat = cos(raw(p.lat));
    auto const sinLon = sin(raw(p.lon));
    auto const cosLon = cos(raw(p.lon));

    matrix3d nx;
#ifdef SICO_USE_EIGEN
    nx << -sinLon, cosLon, 0,                       //
        -sinLat * cosLon, -sinLat * sinLon, cosLat, //
        cosLat * cosLon, cosLat * sinLon, sinLat;
#else
    nx = { -sinLon,          cosLon,           0,      //
           -sinLat * cosLon, -sinLat * sinLon, cosLat, //
           cosLat * cosLon,  cosLat * sinLon,  sinLat };
#endif

    auto const     ve = pecef - ref;
    vector3d const v  = { raw(ve.x), raw(ve.y), raw(ve.z) };

    auto const res = nx * v;

    pos_enu enu;
    enu.east  = meters(res[0]);
    enu.north = meters(res[1]);
    enu.up    = meters(res[2]);
    return enu;
}

static pos_ecef internal_to_ecef(pos_ecef const& refecef, pos_lla const& reflla, pos_enu const& p)
{
    vector3d const v { raw(p.east), raw(p.north), raw(p.up) };

    auto const sinLat = sin(raw(reflla.lat));
    auto const cosLat = cos(raw(reflla.lat));
    auto const sinLon = sin(raw(reflla.lon));
    auto const cosLon = cos(raw(reflla.lon));

    matrix3d nx;
#ifdef SICO_USE_EIGEN
    nx << -sinLon, cosLon, 0,                       //
        -sinLat * cosLon, -sinLat * sinLon, cosLat, //
        cosLat * cosLon, cosLat * sinLon, sinLat;
#else
    nx = { -sinLon, -sinLat * cosLon, cosLat * cosLon, //
           cosLon,  -sinLat * sinLon, cosLat * sinLon, //
           0,       cosLat,           sinLat };
#endif

    auto const res = nx * v;

    return refecef + pos_ecef { meters(res[0]), meters(res[1]), meters(res[2]) };
}

pos_enu sico::to_enu(pos_ecef const& ref, pos_ecef const& p)
{
    return internal_to_enu(ref, p, (pos_lla)p);
}
pos_ecef sico::to_ecef(pos_ecef const& refecef, pos_enu const& p)
{
    return internal_to_ecef(refecef, (pos_lla)refecef, p);
}
pos_enu sico::to_enu(pos_lla const& ref, pos_lla const& p)
{
    return internal_to_enu((pos_ecef)ref, (pos_ecef)p, p);
}
pos_lla sico::to_lla(pos_lla const& reflla, pos_enu const& p)
{
    return (pos_lla)internal_to_ecef((pos_ecef)reflla, reflla, p);
}

ori_ecef sico::to_ecef(pos_lla const& ref, ori_enu const& o)
{
    auto const sinLat = sin(raw(ref.lat));
    auto const sinLon = sin(raw(ref.lon));
    auto const cosLat = cos(raw(ref.lat));
    auto const cosLon = cos(raw(ref.lon));

    auto const cosLatCosLon = cosLat * cosLon;
    auto const cosLatSinLon = cosLat * sinLon;
    auto const sinLatCosLon = sinLat * cosLon;
    auto const sinLatSinLon = sinLat * sinLon;

    auto const cosRoll  = cos(raw(o.roll));
    auto const sinRoll  = sin(raw(o.roll));
    auto const cosPitch = cos(raw(o.pitch));
    auto const sinPitch = sin(raw(o.pitch));
    auto const cosYaw   = cos(raw(o.yaw));
    auto const sinYaw   = sin(raw(o.yaw));

    auto const a11
        = -sinLon * sinYaw * cosPitch - sinLatCosLon * cosYaw * cosPitch + cosLatCosLon * sinPitch;
    auto const a12
        = cosLon * sinYaw * cosPitch - sinLatSinLon * cosYaw * cosPitch + cosLatSinLon * sinPitch;

    double const a23
        = cosLat * (-sinYaw * cosRoll + cosYaw * sinPitch * sinRoll) - sinLat * cosPitch * sinRoll;
    double const a33
        = cosLat * (sinYaw * sinRoll + cosYaw * sinPitch * cosRoll) - sinLat * cosPitch * cosRoll;

    return ori_ecef {
        radians(atan2(a12, a11)),                                       // psi
        radians(asin(-cosLat * cosYaw * cosPitch - sinLat * sinPitch)), // theta
        radians(atan2(a23, a33))                                        // phi
    };
}

ori_enu sico::to_enu(pos_lla const& ref, ori_ecef const& o)
{
    auto const sinLat = sin(raw(ref.lat));
    auto const sinLon = sin(raw(ref.lon));
    auto const cosLon = cos(raw(ref.lon));
    auto const cosLat = cos(raw(ref.lat));

    auto const sinLatSinLon = sinLat * sinLon;
    auto const cosLatCosLon = cosLat * cosLon;
    auto const cosLatSinLon = cosLat * sinLon;
    auto const sinLatCosLon = sinLat * cosLon;

    auto const cosTheta = cos(raw(o.theta));
    auto const sinTheta = sin(raw(o.theta));
    auto const cosPsi   = cos(raw(o.psi));
    auto const sinPsi   = sin(raw(o.psi));
    auto const cosPhi   = cos(raw(o.phi));
    auto const sinPhi   = sin(raw(o.phi));

    auto const sinPhiSinTheta = sinPhi * sinTheta;
    auto const cosPhiSinTheta = cosPhi * sinTheta;
    auto const cosThetaCosPsi = cosTheta * cosPsi;
    auto const cosThetaSinPsi = cosTheta * sinPsi;

    auto const b11 = -sinLon * cosThetaCosPsi + cosLon * cosThetaSinPsi;
    auto const b12
        = -sinLatCosLon * cosThetaCosPsi - sinLatSinLon * cosThetaSinPsi - cosLat * sinTheta;

    auto const b23 = cosLatCosLon * (-cosPhi * sinPsi + sinPhiSinTheta * cosPsi)
        + cosLatSinLon * (cosPhi * cosPsi + sinPhiSinTheta * sinPsi) + sinLat * (sinPhi * cosTheta);

    auto const b33 = cosLatCosLon * (sinPhi * sinPsi + cosPhiSinTheta * cosPsi)
        + cosLatSinLon * (-sinPhi * cosPsi + cosPhiSinTheta * sinPsi)
        + sinLat * (cosPhi * cosTheta);

    return ori_enu {
        radians(atan2(b11, b12)), // yaw
        radians(asin(cosLatCosLon * cosTheta * cosPsi + cosLatSinLon * cosTheta * sinPsi
                     - sinLat * sinTheta)), // pitch
        radians(atan2(-b23, -b33))          // roll
    };
}