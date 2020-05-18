#pragma once
#include "sico/types/local.hpp"
#include "sico/types/orientations.hpp"

namespace sico {

/// Convert vector pos_from_parent to vector from child center,
/// where child is at child_pos from parent center and rotated child_ori in parent space
pos_local to_child(pos_local const& child_pos, quat_local const& child_ori, pos_local const& pos_from_parent);

/// Convert vector pos_from_child to vector from parent center,
/// where child is at child_pos from parent center and rotated child_ori in parent space
pos_local to_parent(pos_local const& child_pos, quat_local const& child_ori, pos_local const& pos_from_child);

/// Convert relative orientation from parent to relative orientation from child
ori_local to_child(ori_local const& child_ori, ori_local const& ori_in_parent_frame);

inline ori_local operator-(ori_local const& child_ori, ori_local const& ori_in_parent_frame)
{
    return to_child(child_ori, ori_in_parent_frame);
}

/// Convert relative orientation from child to relative orientation from parent
ori_local to_parent(ori_local const& child_ori, ori_local const& ori_in_child_frame);

inline ori_local operator+(ori_local const& child_ori, ori_local const& ori_in_child_frame)
{
    return to_parent(child_ori, ori_in_child_frame);
}

/// Convert local vector pos from object center to rotation (roll is zero)
inline ori_local to_local(pos_local const& v)
{
    return internal::vec_to_ori<ref_local>(v.right, v.front, v.up);
}

/// Convert local rotation to front vector from object center (roll is ignored)
inline pos_local to_local(ori_local const& o)
{
    pos_local p;
    internal::ori_to_vec(o, p.right, p.front, p.up);
    return p;
}

/// Convert local rotation to right vector from object center
inline pos_local to_local_right(ori_local const& o)
{
    pos_local p;
    internal::ori_to_vec_right(o, p.right, p.front, p.up);
    return p;
}

/// Convert local rotation to up vector from object center
inline pos_local to_local_up(ori_local const& o)
{
    pos_local p;
    internal::ori_to_vec_up(o, p.right, p.front, p.up);
    return p;
}
} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//