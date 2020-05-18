#include "sico/conversions/local_local.hpp"

#include "sico/types/linear_algebra.hpp"

using namespace sico;

pos_local sico::to_child(pos_local const& ref, quat_local const& q, pos_local const& pos)
{
    vector3d const vec { raw(pos.front - ref.front), raw(pos.right - ref.right),
                         -raw(pos.up - ref.up) };
    vector3d const loc = make_quat(q) * vec;
    return { meters(loc[1]), meters(loc[0]), -meters(loc[2]) };
}

pos_local sico::to_parent(pos_local const& ref, quat_local const& q, pos_local const& pos)
{
    vector3d const vec { raw(pos.front), raw(pos.right), -raw(pos.up) };
#ifdef SICO_USE_EIGEN
    vector3d const loc = make_quat(q).inverse() * vec;
#else
    vector3d const loc = conj(make_quat(q)) * vec;
#endif
    return { meters(loc[1]) + ref.right, meters(loc[0]) + ref.front, -meters(loc[2]) + ref.up };
}

ori_local sico::to_child(ori_local const& child_ori, ori_local const& ori)
{
    return normalized(ori_local { -angleDiff(ori.yaw, child_ori.yaw),
                                  -angleDiff(ori.pitch, child_ori.pitch),
                                  -angleDiff(ori.roll, child_ori.roll) });
}

ori_local sico::to_parent(ori_local const& child_ori, ori_local const& ori)
{
    return normalized(ori_local { child_ori.yaw + ori.yaw, child_ori.pitch + ori.pitch,
                                  child_ori.roll + ori.roll });
}