#include "catch2/catch.hpp"
#include "sico/conversions/ecef_lla.hpp"
#include "sico/types/angles.hpp"

using namespace sico;
using namespace literals;
using namespace Catch::literals;

TEST_CASE("Conversions/Geo")
{
    SECTION("ECEF/LLA")
    {
        auto const lla = pos_lla{0_rad, 0_rad, 0_m};
        pos_ecef const ecef(lla);
        REQUIRE(ecef.x == 6378137_m);
        REQUIRE(ecef.y == 0_m);
        REQUIRE(ecef.z == 0_m);
        pos_lla const lla2(ecef);
        REQUIRE(lla.lat == lla2.lat);
        REQUIRE(lla.lon == lla2.lon);
        REQUIRE(lla.alt == lla2.alt);
        auto const ecef2 = to_ecef(pos_lla{1_deg, 1_deg, 0_m});
        REQUIRE(ecef2.x == Approx(6376200.806_m));
        REQUIRE(ecef2.y == Approx(111296.998_m));
        REQUIRE(ecef2.z == Approx(110568.773_m));
    }


}