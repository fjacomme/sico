#pragma once

#include "sico/types/ecef.hpp"
#include "sico/types/enu.hpp"
#include "sico/types/lla.hpp"
#include "sico/types/local.hpp"
#include "sico/types/ned.hpp"
#include "sico/types/orientations.hpp"

#include <iomanip>
#include <ostream>

namespace sico {

// unfortunately, macros
#define DECL_OSTR(pos_type, f1, f2, f3)                                                       \
    inline std::ostream& operator<<(std::ostream& os, pos_type const& p1)                     \
    {                                                                                         \
        return os << std::fixed << "" #pos_type "[" #f1 "=" << p1.f1 << ", " #f2 "=" << p1.f2 \
                  << ", " #f3 "=" << p1.f3 << "]";                                            \
    }

DECL_OSTR(pos_ecef, x, y, z)
DECL_OSTR(pos_lla, lat, lon, alt)
DECL_OSTR(pos_enu, east, north, up)
DECL_OSTR(pos_enu_m, east, north, up)
DECL_OSTR(pos_ned, north, east, down)
DECL_OSTR(pos_local, right, front, up)
DECL_OSTR(ori_ecef, psi, theta, phi)
DECL_OSTR(ori_enu, yaw, pitch, roll)
DECL_OSTR(ori_local, yaw, pitch, roll)

#undef DECL_OSTR

} // namespace sico

//
// Simulation-Coordinates library
// Author F.Jacomme
// MIT Licensed
//