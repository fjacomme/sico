#pragma once
#include "sico/types/ecef.hpp"
#include "sico/types/lla.hpp"

namespace sico {

/// Convert Lat/Lon/Alt to Earth-Centered-Earth-Fixed
pos_ecef to_ecef(pos_lla const& from);

/// Convert Earth-Centered-Earth-Fixed to Lat/Lon/Alt
pos_lla to_lla(pos_ecef const& from);

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//