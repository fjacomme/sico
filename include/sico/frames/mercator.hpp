#pragma once
#include "sico/conversions/lla_mercator.hpp"

namespace sico {

/// Mercator projection from a reference longitude
class frame_mercator {
    radians lon0;

    public:
    frame_mercator(radians lon0)
        : lon0(lon0)
    {
    }

    pos_enu_m to_enu(pos_lla const& p) const { return sico::to_enu(lon0, p); }
    pos_lla   to_lla(pos_enu_m const& p) const { return sico::to_lla(lon0, p); }
};

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//