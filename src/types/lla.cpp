#include "sico/types/lla.hpp"
#include "sico/conversions/ecef_lla.hpp"
#include "sico/conversions/geo_tangent.hpp"
#include "sico/types/angles.hpp"

#include <algorithm>

using namespace sico;

pos_lla::operator vec_ecef<meters>() const
{
    return to_ecef(*this);
}

pos_lla sico::operator+(pos_lla const& p1, vec_enu<meters, ref_tangent> const& p2)
{
    return to_lla(p1, p2);
}

pos_lla sico::operator-(pos_lla const& p1, vec_enu<meters, ref_tangent> const& p2)
{
    return to_lla(p1, -p2);
}

void sico::normalize(pos_lla& p)
{
    p.lat = radians(std::max(std::min(raw(p.lat), SICO_PI2), -SICO_PI2));
    p.lon = normalizeAnglePi(p.lon);
}