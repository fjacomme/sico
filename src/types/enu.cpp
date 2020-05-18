#include "sico/types/enu.hpp"
#include "sico/types/angles.hpp"

using namespace sico;

void sico::normalize(ori_enu& o)
{
    o.yaw   = normalizeAngle2Pi(o.yaw);
    o.pitch = normalizeAnglePi(o.pitch);
    o.roll  = normalizeAnglePi(o.roll);
}