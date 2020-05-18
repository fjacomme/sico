#include "catch2/catch.hpp"
#include "sico/frames/local_tangent.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace Catch::literals;
using namespace sico::literals;

TEST_CASE("Frames/LocalTangent")
{
    SECTION("Ref")
    {
        auto lat  = GENERATE(-pi_over_2(), 0_rad, pi_over_2());
        auto lon  = GENERATE(-pi_over_2() * 0.5, 0_rad, pi_over_2() * 0.5);
        auto alt  = GENERATE(-1000_m, 0_m, 1000_m);
        auto offr = GENERATE(0_rad, 0.01_rad);
        auto offm = GENERATE(0_m, 10_m);

        auto const      ref = pos_lla { lat, lon, alt };
        auto const      pos = pos_lla { lat + offr, lon + offr, alt + offm };
        auto const      enu = sico::to_enu(ref, pos);
        frame_enu const frame { ref };
        REQUIRE(frame.to_enu(pos) == enu);
        REQUIRE(frame.to_lla(enu) == sico::to_lla(ref, enu));
    }
}