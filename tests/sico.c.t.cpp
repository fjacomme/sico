#include "catch2/catch.hpp"
#include "sico/conversions/ecef_lla.hpp"
extern "C" {
#include "sico/sico.h"
}

using namespace sico;
using namespace sico::literals;

TEST_CASE("C-Wrapper")
{
    SECTION("ECEF/LLA")
    {
        pos_lla const  lla { 0.4_rad, -0.2_rad, 123_m };
        pos_ecef const ecef(lla);
        double         lat, lon, alt, x, y, z;
        sico_lla2ecef(raw(lla.lat), raw(lla.lon), raw(lla.alt), &x, &y, &z);
        REQUIRE(Approx(x) == raw(ecef.x));
        REQUIRE(Approx(y) == raw(ecef.y));
        REQUIRE(Approx(z) == raw(ecef.z));
        sico_ecef2lla(x, y, z, &lat, &lon, &alt);
        REQUIRE(Approx(lat) == raw(lla.lat));
        REQUIRE(Approx(lon) == raw(lla.lon));
        REQUIRE(Approx(alt) == raw(lla.alt));
    }
}