#include "sico/conversions/enu_local.hpp"

#include "sico/types/linear_algebra.hpp"

using namespace sico;

pos_local sico::to_local(pos_enu const& enu, quat_enu const& q)
{
    vector3d const vec { raw(enu.north), raw(enu.east), -raw(enu.up) };
    vector3d const loc = make_quat(q) * vec;
    return { meters(loc[1]), meters(loc[0]), -meters(loc[2]) };
}

pos_enu sico::to_enu(pos_local const& pos, quat_enu const& q)
{
    vector3d const vec { raw(pos.front), raw(pos.right), -raw(pos.up) };
#ifdef SICO_USE_EIGEN
    vector3d const loc = make_quat(q).inverse() * vec;
#else
    vector3d const loc = conj(make_quat(q)) * vec;
#endif
    return { meters(loc[1]), meters(loc[0]), -meters(loc[2]) };
}

ori_local sico::operator-(ori_enu const& obj_ori, ori_enu const& ori)
{
    return normalized(ori_local { -angleDiff(ori.yaw, obj_ori.yaw),
                                  -angleDiff(ori.pitch, obj_ori.pitch),
                                  -angleDiff(ori.roll, obj_ori.roll) });
}

ori_enu sico::operator+(ori_enu const& obj_ori, ori_local const& ori)
{
    return normalized(
        ori_enu { obj_ori.yaw + ori.yaw, obj_ori.pitch + ori.pitch, obj_ori.roll + ori.roll });
}