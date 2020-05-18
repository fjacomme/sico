#include "sico/types/local.hpp"
#include "sico/types/angles.hpp"

using namespace sico;

void sico::normalize(ori_local& o)
{
    o.yaw   = normalizeAnglePi(o.yaw);
    o.pitch = normalizeAnglePi(o.pitch);
    o.roll  = normalizeAnglePi(o.roll);
}