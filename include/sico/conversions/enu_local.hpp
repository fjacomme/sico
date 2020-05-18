#pragma once
#include "sico/types/enu.hpp"
#include "sico/types/local.hpp"
#include "sico/types/orientations.hpp"

namespace sico {

/// Convert ENU vector pos from object center to local vector according to object rotation ori
pos_local to_local(pos_enu const& pos, quat_enu const& ori);

/// Convert local vector pos to ENU vector from object center according to object rotation ori
pos_enu to_enu(pos_local const& pos, quat_enu const& ori);

/// Convert ENU rotation to local rotation
ori_local operator-(ori_enu const& obj_ori, ori_enu const& ori);

/// Convert local rotation to ENU rotation
ori_enu operator+(ori_enu const& obj_ori, ori_local const& ori);

/// Convert ENU vector pos from object center to rotation (roll is zero)
inline ori_enu to_enu(pos_enu const& v)
{
    return internal::vec_to_ori<ref_enu>(v.east, v.north, v.up);
}

/// Convert ENU rotation to front vector from object center (roll is ignored)
inline pos_enu to_enu(ori_enu const& o)
{
    pos_enu p;
    internal::ori_to_vec(o, p.east, p.north, p.up);
    return p;
}

/// Convert ENU rotation to right vector from object center
inline pos_enu to_enu_right(ori_enu const& o)
{
    pos_enu p;
    internal::ori_to_vec_right(o, p.east, p.north, p.up);
    return p;
}

/// Convert ENU rotation to up vector from object center
inline pos_enu to_enu_up(ori_enu const& o)
{
    pos_enu p;
    internal::ori_to_vec_up(o, p.east, p.north, p.up);
    return p;
}

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//