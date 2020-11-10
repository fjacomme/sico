#pragma once
#include "sico/conversions/lla_mercator.hpp"

namespace sico {

/// Mercator projection from a reference longitude
class frame_mercator {
    pos_lla ref;
    meters  north_offset;

    public:
    frame_mercator(pos_lla const& ref)
        : ref(ref)
    {
        north_offset = sico::to_enu(ref.lon, ref).north;
    }

    pos_enu_m to_enu(pos_lla const& p) const
    {
        auto const enu = sico::to_enu(ref.lon, p);
        return { enu.east, enu.north - north_offset, enu.up - ref.alt };
    }
    
    pos_lla to_lla(pos_enu_m const& p) const
    {
        return sico::to_lla(ref.lon, { p.east, p.north + north_offset, p.up + ref.alt });
    }
};

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//