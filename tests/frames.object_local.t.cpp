#include "catch2/catch.hpp"
#include "sico/frames/object_local.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace sico::literals;
using namespace Catch::literals;

TEST_CASE("Frames/ObjectLocal")
{
    SECTION("Zero One")
    {
        frame_child_object const frame(
            pos_local { 1_m, 1_m, 1_m },
            ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(frame.to_child(pos_local { 1_m, 1_m, 1_m }) == pos_local { 0_m, 0_m, 0_m });
        REQUIRE(frame.to_parent(pos_local { 0_m, 0_m, 0_m }) == pos_local { 1_m, 1_m, 1_m });
    }
}