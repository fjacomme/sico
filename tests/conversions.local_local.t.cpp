#include "catch2/catch.hpp"
#include "sico/conversions/local_local.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/orientations.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace Catch::literals;
using namespace sico::literals;

TEST_CASE("Conversions/LocalLocal")
{
    pos_local const ref0 { 0_m, 0_m, 0_m };
    SECTION("Zero")
    {
        quat_local const q(ori_local { 0_rad, 0_rad, 0_rad });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 1_m, 1_m, 1_m });
        REQUIRE(to_parent(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 1_m, 1_m, 1_m });
    }

    SECTION("Right")
    {
        quat_local const q(ori_local { 90_deg, 0_rad, 0_rad });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { -1_m, 1_m, 1_m });
    }

    SECTION("Left")
    {
        quat_local const q(ori_local { -90_deg, 0_rad, 0_rad });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 1_m, -1_m, 1_m });
    }

    SECTION("Down")
    {
        quat_local const q(ori_local { 0_rad, -90_deg, 0_rad });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 1_m, -1_m, 1_m });
    }

    SECTION("Up")
    {
        quat_local const q(ori_local { 0_rad, 90_deg, 0_rad });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 1_m, 1_m, -1_m });
    }

    SECTION("Bank")
    {
        quat_local const q(ori_local { 0_rad, 0_rad, 90_deg });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { -1_m, 1_m, 1_m });
    }

    SECTION("BankNeg")
    {
        quat_local const q(ori_local { 0_rad, 0_rad, -90_deg });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 1_m, 1_m, -1_m });
    }

    SECTION("Right Down")
    {
        quat_local const q(ori_local { 90_deg, -90_deg, 0_deg });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { -1_m, -1_m, 1_m });
    }

    SECTION("Right Bank")
    {
        quat_local const q(ori_local { 90_deg, 0_deg, 90_deg });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { -1_m, 1_m, -1_m });
    }

    SECTION("Down Bank")
    {
        quat_local const q(ori_local { 0_deg, -90_deg, 90_deg });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { -1_m, -1_m, 1_m });
    }

    SECTION("Right Up Bank")
    {
        quat_local const q(ori_local { 90_deg, 45_deg, 90_deg });

        auto const sq2  = meters(sqrt(2.));
        auto const sq22 = sq2 / 2.;
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 0_m, 0_m }) == pos_local { sq22, sq22, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 1_m, 0_m }) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_child(ref0, q, pos_local { 0_m, 0_m, 1_m }) == pos_local { -sq22, sq22, 0_m });
        REQUIRE(to_child(ref0, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 0_m, sq2, -1_m });
    }

    SECTION("Offset 1")
    {
        pos_local const  ref { 1_m, 1_m, 1_m };
        quat_local const q(ori_local { 0_rad, 0_rad, 0_rad });
        REQUIRE(to_child(ref, q, pos_local { 1_m, 1_m, 1_m }) == pos_local { 0_m, 0_m, 0_m });
        REQUIRE(to_parent(ref, q, pos_local { 0_m, 0_m, 0_m }) == pos_local { 1_m, 1_m, 1_m });
    }

    SECTION("Ori")
    {
        REQUIRE(ori_local { 0_deg, 0_deg, 0_deg } + ori_local { 0_deg, 0_deg, 0_deg }
                == ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_local { 0_deg, 0_deg, 0_deg } + ori_local { 10_deg, 20_deg, 30_deg }
                == ori_local { 10_deg, 20_deg, 30_deg });
        REQUIRE(ori_local { 170_deg, 0_deg, 0_deg } + ori_local { 20_deg, 0_deg, 0_deg }
                == ori_local { -170_deg, 0_deg, 0_deg });


        REQUIRE(ori_local { 0_deg, 0_deg, 0_deg } - ori_local { 0_deg, 0_deg, 0_deg }
                == ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_local { 90_deg, 45_deg, 20_deg } - ori_local { 90_deg, 45_deg, 20_deg }
                == ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_local { 90_deg, 0_deg, 0_deg } - ori_local { 0_deg, 0_deg, 0_deg }
                == ori_local { -90_deg, 0_deg, 0_deg });
        REQUIRE(ori_local { 00_deg, 0_deg, 0_deg } - ori_local { 90_deg, 0_deg, 0_deg }
                == ori_local { 90_deg, 0_deg, 0_deg });
                
        REQUIRE(ori_local { 10_deg, 20_deg, 30_deg } - ori_local { 10_deg, 20_deg, 30_deg }
                == ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_local { -170_deg, 0_deg, 0_deg } - ori_local { 20_deg, 0_deg, 0_deg }
                == ori_local { -170_deg, 0_deg, 0_deg });
    }
}