#pragma once
#include "sico/types/lla.hpp"
#include "sico/types/utm.hpp"

namespace sico {

/// Convert LLA to UTM position
pos_utm to_utm(pos_lla const& lla);

/// Convert LLA to UTM position
pos_lla to_lla(pos_utm const& utm);

/// UTM Zone number for given latitude and longitude
int utm_zone_number(degrees lat, degrees lon);

/// UTM Zone letter for given latitude
char utm_zone_letter(degrees lat);

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//