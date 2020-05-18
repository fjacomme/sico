#include "catch2/catch.hpp"
#include "sico/conversions/geo_tangent.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace Catch::literals;
using namespace sico::literals;

using std::vector;

static void test_ori(pos_lla const& reflla, ori_enu ori, ori_ecef res)
{
    INFO("ref=" << reflla << " o=" << ori << " e=" << res);
    auto const ecef = normalized(to_ecef(reflla, ori));
    REQUIRE(ecef == res);
    auto const enu = normalized(to_enu(reflla, ecef));
    REQUIRE(enu == normalized(ori));
}

static void test_oris(pos_lla const& reflla, vector<vector<double>> const& vals)
{
    for (auto& v : vals) {
        test_ori(reflla, { degrees(v[0]), degrees(v[1]), degrees(v[2]) },
                 { radians(v[3]), radians(v[4]), radians(v[5]) });
    }
}

TEST_CASE("Conversions/LocalTangent")
{
    SECTION("Altitude")
    {
        for (int i = 1; i < 9; i++) {
            for (int j = 1; j < 9; j++) {
                pos_lla const ref { -pi_over_2() + pi() * double(i / 9.), -pi() + pi() * double(j / 4.), 0_m };
                INFO(i << ", " << j << " ref=" << ref);
                REQUIRE(to_lla(ref, pos_enu { 0_m, 0_m, 0_m }) == ref);
                REQUIRE(to_lla(ref, pos_enu { 0_m, 0_m, 10_m })
                        == pos_lla { ref.lat, ref.lon, 10_m });
                REQUIRE(to_enu(ref, ref) == pos_enu { 0_m, 0_m, 0_m });
                REQUIRE(to_enu(ref, to_lla(ref, pos_enu { 10_m, 10_m, 10_m }))
                        == pos_enu { 10_m, 10_m, 10_m });
            }
        }
    }

    SECTION("Orientation")
    {
        test_oris({ 0_deg, 178.511_deg, 0_m },
                  { { 0.1, 0, 0, -1.596784, -1.569051, -1.570796 },
                    { 90, 0, 0, -1.59678, 0, -1.5708 },
                    { -90, 0, 0, 1.54481, 0, 1.5708 } });
        test_oris({ 39.6520183_deg, -40.0483315_deg, 140_m },
                  { { 0, 0, 0, 2.44262, -0.878738, 3.14159 },
                    { 90, 0, 0, 0.871821, 0, -2.26285 },
                    { -90, 0, 0, -2.26977, 0, 2.26285 } });
        test_oris({ 29.4079512_deg, -13.7641538_deg, 150_m },
                  { { 0, 0, 0, 2.90138, -1.05753, 3.14158 },
                    { 75, 0, 0, 1.46139, -0.227425, -2.09902 },
                    { 180, 0, 0, -0.240230, 1.057531, 3.141593 } });
    }
}