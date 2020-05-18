#include "sico/conversions/ecef_lla.hpp"
#include "sico/types/angles.hpp"

using namespace sico;

void sico::normalize(ori_ecef& o)
{
    o.psi   = normalizeAnglePi(o.psi);
    o.theta = normalizeAnglePi(o.theta);
    o.phi   = normalizeAnglePi(o.phi);
}

template<>
template<>
pos_ecef::operator pos_lla() const
{
    return to_lla(*this);
}
