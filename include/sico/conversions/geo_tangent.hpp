#pragma once
#include "sico/types/ecef.hpp"
#include "sico/types/enu.hpp"
#include "sico/types/lla.hpp"
#include "sico/types/orientations.hpp"

namespace sico {

/// Convert ECEF vector ref to p into an ENU vector in local tangent frame
pos_enu to_enu(pos_ecef const& ref, pos_ecef const& p);

/// Convert ENU vector in local tangent frame from ref to ECEF position
pos_ecef to_ecef(pos_ecef const& ref, pos_enu const& p);

/// Convert LLA position p to ENU vector in local tangent frame at ref
pos_enu to_enu(pos_lla const& ref, pos_lla const& p);

/// Convert ENU vector in local tangent frame at ref to LLA position
pos_lla to_lla(pos_lla const& ref, pos_enu const& p);

/// Convert ENU orientation in local tangent frame at ref to ECEF orientation
ori_enu to_enu(pos_lla const& ref, ori_ecef const& o);

/// Convert ECEF orientation to ENU orientation in local tangent frame at ref
ori_ecef to_ecef(pos_lla const& ref, ori_enu const& o);

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//