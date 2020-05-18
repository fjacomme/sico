#pragma once
#include "sico/conversions/geo_tangent.hpp"

namespace sico {

/// Local Tangent ENU reference frame, relative to an LLA position
class frame_enu {
    pos_lla ref;

    public:
    frame_enu(pos_lla const& r) { set_ref(r); }

    void set_ref(pos_lla const& r) { ref = r; }

    pos_enu to_enu(pos_lla const& p) const { return sico::to_enu(ref, p); }
    pos_lla to_lla(pos_enu const& p) const { return sico::to_lla(ref, p); }
};

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//