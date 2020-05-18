#include "catch2/catch.hpp"
#include "sico/conversions/enu_local.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/orientations.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace literals;
using namespace Catch::literals;

TEST_CASE("Conversions/EnuLocal")
{
    SECTION("Zero")
    {
        quat_enu const q(ori_enu { 0_rad, 0_rad, 0_rad });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { 1_m, 1_m, 1_m });
        REQUIRE(to_enu(pos_local { 1_m, 1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("East")
    {
        quat_enu const q(ori_enu { 90_deg, 0_rad, 0_rad });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { -1_m, 1_m, 1_m });
        REQUIRE(to_enu(pos_local { -1_m, 1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("West")
    {
        quat_enu const q(ori_enu { -90_deg, 0_rad, 0_rad });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { 1_m, -1_m, 1_m });
        REQUIRE(to_enu(pos_local { 1_m, -1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("Down")
    {
        quat_enu const q(ori_enu { 0_rad, -90_deg, 0_rad });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { 1_m, -1_m, 1_m });
        REQUIRE(to_enu(pos_local { 1_m, -1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("Up")
    {
        quat_enu const q(ori_enu { 0_rad, 90_deg, 0_rad });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { 1_m, 1_m, -1_m });
        REQUIRE(to_enu(pos_local { 1_m, 1_m, -1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("Bank")
    {
        quat_enu const q(ori_enu { 0_rad, 0_rad, 90_deg });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { -1_m, 1_m, 1_m });
        REQUIRE(to_enu(pos_local { -1_m, 1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("BankNeg")
    {
        quat_enu const q(ori_enu { 0_rad, 0_rad, -90_deg });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { 1_m, 1_m, -1_m });
        REQUIRE(to_enu(pos_local { 1_m, 1_m, -1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("East Down")
    {
        quat_enu const q(ori_enu { 90_deg, -90_deg, 0_deg });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { -1_m, -1_m, 1_m });
        REQUIRE(to_enu(pos_local { -1_m, -1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("East Bank")
    {
        quat_enu const q(ori_enu { 90_deg, 0_deg, 90_deg });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, 1_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { -1_m, 1_m, -1_m });
        REQUIRE(to_enu(pos_local { -1_m, 1_m, -1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("Down Bank")
    {
        quat_enu const q(ori_enu { 0_deg, -90_deg, 90_deg });
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { 0_m, 0_m, 1_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { -1_m, 0_m, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { 0_m, -1_m, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { -1_m, -1_m, 1_m });
        REQUIRE(to_enu(pos_local { -1_m, -1_m, 1_m }, q) == pos_enu { 1_m, 1_m, 1_m });
    }

    SECTION("East Up Bank")
    {
        quat_enu const q(ori_enu { 90_deg, 45_deg, 90_deg });

        auto const sq2  = meters(sqrt(2.));
        auto const sq22 = sq2 / 2.;
        REQUIRE(to_local(pos_enu { 1_m, 0_m, 0_m }, q) == pos_local { sq22, sq22, 0_m });
        REQUIRE(to_local(pos_enu { 0_m, 1_m, 0_m }, q) == pos_local { 0_m, 0_m, -1_m });
        REQUIRE(to_local(pos_enu { 0_m, 0_m, 1_m }, q) == pos_local { -sq22, sq22, 0_m });
        REQUIRE(to_local(pos_enu { 1_m, 1_m, 1_m }, q) == pos_local { 0_m, sq2, -1_m });
    }

    SECTION("Dir to Ori")
    {
        REQUIRE(to_enu(pos_enu { 0_m, 2_m, 0_m }) == ori_enu { 0_deg, 0_deg, 0_deg });
        REQUIRE(to_enu(pos_enu { 1_m, 0_m, 0_m }) == ori_enu { 90_deg, 0_deg, 0_deg });
        REQUIRE(to_enu(pos_enu { 0_m, 0_m, 1_m }) == ori_enu { 0_deg, 90_deg, 0_deg });
        REQUIRE(to_enu(pos_enu { 0_m, 0_m, -1_m }) == ori_enu { 0_deg, -90_deg, 0_deg });
        REQUIRE(to_enu(pos_enu { 1_m, 1_m, 1_m })
                == ori_enu { 45_deg, radians(atan(1.0 / sqrt(2.0))), 0_deg });
    }

    SECTION("Ori to Dir")
    {
        REQUIRE(to_enu(ori_enu { 0_deg, 0_deg, 0_deg }) == pos_enu { 0_m, 1_m, 0_m });
        REQUIRE(to_enu_up(ori_enu { 0_deg, 0_deg, 0_deg }) == pos_enu { 0_m, 0_m, 1_m });
        REQUIRE(to_enu_right(ori_enu { 0_deg, 0_deg, 0_deg }) == pos_enu { 1_m, 0_m, 0_m });

        REQUIRE(to_enu(ori_enu { 90_deg, 0_deg, 0_deg }) == pos_enu { 1_m, 0_m, 0_m });
        REQUIRE(to_enu_up(ori_enu { 90_deg, 0_deg, 0_deg }) == pos_enu { 0_m, 0_m, 1_m });
        REQUIRE(to_enu_right(ori_enu { 90_deg, 0_deg, 0_deg }) == pos_enu { 0_m, -1_m, 0_m });

        REQUIRE(to_enu(ori_enu { 0_deg, 90_deg, 0_deg }) == pos_enu { 0_m, 0_m, 1_m });
        REQUIRE(to_enu_up(ori_enu { 0_deg, 90_deg, 0_deg }) == pos_enu { 0_m, -1_m, 0_m });
        REQUIRE(to_enu_right(ori_enu { 0_deg, 90_deg, 0_deg }) == pos_enu { 1_m, 0_m, 0_m });
    }

    SECTION("Ori to local")
    {
        REQUIRE(ori_enu { 0_deg, 0_deg, 0_deg } - ori_enu { 0_deg, 0_deg, 0_deg }
                == ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_enu { 90_deg, 45_deg, 20_deg } - ori_enu { 90_deg, 45_deg, 20_deg }
                == ori_local { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_enu { 90_deg, 0_deg, 0_deg } - ori_enu { 0_deg, 0_deg, 0_deg }
                == ori_local { -90_deg, 0_deg, 0_deg });
        REQUIRE(ori_enu { 00_deg, 0_deg, 0_deg } - ori_enu { 90_deg, 0_deg, 0_deg }
                == ori_local { 90_deg, 0_deg, 0_deg });
        REQUIRE(ori_enu { 10_deg, 0_deg, 0_deg } - ori_enu { 350_deg, 0_deg, 0_deg }
                == ori_local { -20_deg, 0_deg, 0_deg });
    }

    SECTION("Local to ori")
    {
        REQUIRE(ori_enu { 0_deg, 0_deg, 0_deg } + ori_local { 0_deg, 0_deg, 0_deg }
                == ori_enu { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_enu { 90_deg, 45_deg, 20_deg } + ori_local { 0_deg, 0_deg, 0_deg }
                == ori_enu { 90_deg, 45_deg, 20_deg });
        REQUIRE(ori_enu { 90_deg, 0_deg, 0_deg } + ori_local { -90_deg, 0_deg, 0_deg }
                == ori_enu { 0_deg, 0_deg, 0_deg });
        REQUIRE(ori_enu { 00_deg, 0_deg, 0_deg } + ori_local { 90_deg, 0_deg, 0_deg }
                == ori_enu { 90_deg, 0_deg, 0_deg });
    }
}