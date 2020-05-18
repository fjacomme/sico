#pragma once
#include "sico/conversions/enu_local.hpp"
#include "sico/conversions/local_local.hpp"
#include "sico/frames/local_tangent.hpp"
#include "sico/types/orientations.hpp"

namespace sico {

/// Local Frame for an object positioned in an ENU frame
class frame_object {
    protected:
    frame_enu frame;
    quat_enu  quat;

    public:
    frame_object(pos_lla const& ref, ori_enu const& ori)
        : frame(ref)
        , quat(ori)
    {
    }

    void set_ref(pos_lla const& ref, ori_enu const& ori)
    {
        frame.set_ref(ref);
        quat = quat_enu(ori);
    }

    pos_local to_local(pos_lla const& p) const { return to_local(frame.to_enu(p)); }
    pos_lla   to_lla(pos_local const& p) const { return frame.to_lla(to_enu(p)); }

    pos_local to_local(pos_enu const& p) const { return sico::to_local(p, quat); }
    pos_enu   to_enu(pos_local const& p) const { return sico::to_enu(p, quat); }
};

/// Local frame for an object on another object
class frame_child_object {
    protected:
    pos_local  ref;
    quat_local quat;

    public:
    frame_child_object(pos_local const& child_pos, ori_local const& ori)
        : ref(child_pos)
        , quat(ori)
    {
    }

    void set_ref(pos_local const& child_pos, ori_local const& ori)
    {
        ref  = child_pos;
        quat = quat_local(ori);
    }

    pos_local to_child(pos_local const& p) const { return sico::to_child(ref, quat, p); }
    pos_local to_parent(pos_local const& p) const { return sico::to_parent(ref, quat, p); }
};

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//