#include "catch2/catch.hpp"
#include "sico/conversions/lla_utm.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace Catch::literals;
using namespace sico::literals;

void test(pos_lla const& lla, pos_utm const& ref)
{
    INFO("test: " << lla);
    auto const utm = to_utm(lla);
    REQUIRE(utm.zone_letter == ref.zone_letter);
    REQUIRE(utm.zone_number == ref.zone_number);
    REQUIRE(utm.altitude == lla.alt);
    REQUIRE(utm.altitude == ref.altitude);
    REQUIRE(utm.northing == Approx(ref.northing));
    REQUIRE(utm.easting == Approx(ref.easting));
    auto const lla2 = to_lla(utm);
    REQUIRE(lla2 == lla);
    REQUIRE(lla2.lat == Approx(lla.lat));
    REQUIRE(lla2.lon == Approx(lla.lon).margin(0.01));
    REQUIRE(lla2.alt == lla.alt);
}

TEST_CASE("Conversions/UTM")
{
    test(pos_lla { 0_rad, 0_rad, 0_m }, pos_utm { 166021.44_m, 0_m, 0_m, 31, 'N' });
    test(pos_lla { 89_deg, 0_deg, 0_m },
         pos_utm { 494157.03_m, 9886468.74_m, 0_m, 31, 'Z' });
    test(pos_lla { 0_deg, 90_deg, 0_m },
         pos_utm { 166021.44_m, 0_m, 0_m, 46, 'N' });
    test(pos_lla { 45_deg, 45_deg, 45_m },
         pos_utm { 500000.00_m, 4982950.40_m, 45_m, 38, 'T' });
    test(pos_lla { -45_deg, -45_deg, 45_m },
         pos_utm { 500000.00_m, 5017049.60_m, 45_m, 23, 'G' });
}