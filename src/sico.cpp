extern "C" {
#include "sico/sico.h"
}

#include "sico/conversions/ecef_lla.hpp"

using namespace sico;

extern "C" void sico_lla2ecef(double lat, double lon, double alt, double* x, double* y, double* z)
{
    auto const ecef = to_ecef({ radians(lat), radians(lon), meters(alt) });

    *x = raw(ecef.x);
    *y = raw(ecef.y);
    *z = raw(ecef.z);
}

extern "C" void sico_ecef2lla(double x, double y, double z, double* lat, double* lon, double* alt)
{
    auto const lla = to_lla({ meters(x), meters(y), meters(z) });

    *lat = raw(lla.lat);
    *lon = raw(lla.lon);
    *alt = raw(lla.alt);
}