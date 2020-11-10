#include "catch2/catch.hpp"
#include "sico/conversions/lla_mercator.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/stream.hpp"

#include <fstream>
#include <sstream>

using namespace sico;
using namespace Catch::literals;
using namespace sico::literals;

void test(radians lonref, pos_lla const& lla, pos_enu_m const& ref)
{
    INFO("test: " << lonref << " " << lla << " " << ref);
    REQUIRE(to_enu(lonref, lla) == ref);
    REQUIRE(to_lla(lonref, ref) == lla);
}

void testzero(double lat, double lon, double x, double y)
{
    INFO("testo: " << lat << " " << lon << " " << x << " " << y);
    auto const      lonref(0_rad);
    pos_lla const   lla { degrees(lat), degrees(lon), 0_m };
    pos_enu_m const enu { meters(x), meters(y), 0_m };
    auto const      enu2 = to_enu(lonref, lla);
    REQUIRE(enu2 == enu);
    CHECK(to_lla(lonref, enu) == lla);
}

TEST_CASE("Conversions/Mercator")
{
    test(0_rad, pos_lla { 0_rad, 0_rad, 0_m }, pos_enu_m { 0_m, 0_m, 0_m });
    test(1_deg, pos_lla { 0_deg, 1_deg, 0_m }, pos_enu_m { 0_m, 0_m, 0_m });
    test(1_deg, pos_lla { 0.0904369469508624_deg, 1_deg, 0_m }, pos_enu_m { 0_m, 9996_m, 0_m });
    test(1_deg, pos_lla { 0_deg, 1.08983149136038_deg, 0_m }, pos_enu_m { 9996_m, 0_m, 0_m });

    testzero(70.579277094557, 45.599419731762, 1548706.7916191491794, 8451449.1987722350778);
    testzero(10.018893710119, 23.313323816796, 2624150.74092867098, 1204434.0416046227631);
    testzero(19.479895588178, 75.662049225092, 9855841.2329353332058, 6145496.1151551160577);
    testzero(21.072464821428, 29.82868439326, 3206390.6919962592145, 2650745.4004060203847);
    testzero(5.458957393183, 36.385237374895, 4328154.0835012728645, 749647.6236903529367);

    std::ifstream data("TMcoords.dat"); // easily found on the internet
    std::string   line;
    int           l = 1;
    while (std::getline(data, line)) {
        INFO(l << ": " << line);
        l++;
        std::stringstream ss(line);
        ss.precision(28);
        double lat, lon, x, y;
        ss >> lat >> lon >> x >> y;
        if (std::abs(lat) > 85 || std::abs(lon) > 70)
            continue; // the method used is not exact enough for such offsets
        testzero(lat, lon, x, y);
    }
}