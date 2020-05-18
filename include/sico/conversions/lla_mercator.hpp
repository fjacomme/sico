#pragma once
#include "sico/types/enu.hpp"
#include "sico/types/lla.hpp"

namespace sico {

/// Convert LLA position to ENU in mercator projection
pos_enu_m to_enu(radians lon0, pos_lla const& lla);

/// Convert ENU position in mercator projection position to LLQ
pos_lla to_lla(radians lon0, pos_enu_m const& enu);

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//