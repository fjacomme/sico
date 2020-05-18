#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2/catch.hpp"
#include "sico/types/angles.hpp"

using namespace sico;
using namespace literals;

TEST_CASE("Types/Angles")
{
    SECTION("PI")
    {
        REQUIRE(90.0_deg == (degrees)pi_over_2());
        REQUIRE(180.0_deg == (degrees)pi());
        REQUIRE(360.0_deg == (degrees)pi_times_2());
    }

    SECTION("Normalize degrees")
    {
        REQUIRE(normalizeAngle180(0._deg) == 0._deg);
        REQUIRE(normalizeAngle180(360._deg) == 0._deg);
        REQUIRE(normalizeAngle180(180._deg) == 180._deg);
        REQUIRE(normalizeAngle180(-180._deg) == 180._deg);
        REQUIRE(normalizeAngle180(270._deg) == -90._deg);

        REQUIRE(normalizeAngle360(0._deg) == 0._deg);
        REQUIRE(normalizeAngle360(360._deg) == 0._deg);
        REQUIRE(normalizeAngle360(370._deg) == 10._deg);
        REQUIRE(normalizeAngle360(-90_deg) == 270._deg);
    }

    SECTION("Normalize radians")
    {
        REQUIRE(normalizeAnglePi(0_rad) == 0_rad);
        REQUIRE(normalizeAnglePi(pi_times_2()) == 0_rad);
        REQUIRE(normalizeAnglePi(pi()) == pi());
        REQUIRE(normalizeAnglePi(-pi()) == pi());
        REQUIRE(normalizeAnglePi(-pi_times_2()) == 0_rad);
        REQUIRE(normalizeAnglePi(3. * pi_times_2() / 4.) == -pi() / 2.);

        REQUIRE(normalizeAngle2Pi(0_rad) == 0_rad);
        REQUIRE(normalizeAngle2Pi(pi_times_2()) == 0_rad);
        REQUIRE(normalizeAngle2Pi(pi_times_2() + pi_over_2()) == pi_over_2());
        REQUIRE(normalizeAngle2Pi(pi()) == pi());
        REQUIRE(normalizeAngle2Pi(-pi()) == pi());
        REQUIRE(normalizeAngle2Pi(-pi_times_2()) == 0_rad);

        for (int i = -8; i < 9; i++) {
            auto a = pi_over_2() * double(i);
            INFO(a);
            REQUIRE(normalizeAnglePi(a) > -pi());
            REQUIRE(normalizeAnglePi(a) <= pi());
            REQUIRE(normalizeAngle2Pi(a) >= 0_rad);
            REQUIRE(normalizeAngle2Pi(a) < pi_times_2());
        }
    }

    SECTION("Diff")
    {
        REQUIRE(angleDiff(0_deg, 0_deg) == 0_deg);
        REQUIRE(angleDiff(0_deg, 1_deg) == 1_deg);
        REQUIRE(angleDiff(1_deg, 0_deg) == -1_deg);
        REQUIRE(angleDiff(0_deg, 360_deg) == 0_deg);
        REQUIRE(angleDiff(0_deg, 180_deg) == 180_deg);
        REQUIRE(angleDiff(-179_deg, 180_deg) == -1_deg);
        REQUIRE(angleDiff(0_deg, 359_deg) == -1_deg);
    }
}