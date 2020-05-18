#pragma once
#include "sico/types/units.hpp"

namespace sico {

#pragma warning(push)
#pragma warning(disable : 4820) // filling bytes

/// Universal Transverse Mercator Grid vector
template<typename Unit>
struct vec_utm {
    Unit easting;
    Unit northing;
    Unit altitude;
    int  zone_number;
    char zone_letter;
};
#pragma warning(pop)

/// Universal Transverse Mercator Grid position
using pos_utm = vec_utm<meters>;
/// Universal Transverse Mercator Grid velocity
using vel_utm = vec_utm<meters_ps>;
/// Universal Transverse Mercator Grid acceleration
using acc_utm = vec_utm<meters_ps2>;

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//