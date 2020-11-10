#include "catch2/catch.hpp"
#include "sico/frames/mercator.hpp"
#include "sico/types/angles.hpp"
#include "sico/types/stream.hpp"

#include <fstream>
#include <sstream>

using namespace sico;
using namespace Catch::literals;
using namespace sico::literals;

using std::vector;

#define eqp(p1, p2) REQUIRE(abs((p1 - p2)) < 0.1)

static void test_enu(
    pos_lla const& reflla, double lat, double lon, double alt, double e, double n, double u)
{
    frame_mercator frame(reflla);

    auto const lla = pos_lla { degrees(lat), degrees(lon), meters(alt) };

    auto const enu = frame.to_enu(lla);

    INFO(reflla << "\n" << lla << "\n" << enu);

    eqp(enu.east, e);
    eqp(enu.north, n);
    eqp(enu.up, u);

    auto const lla2 = frame.to_lla(enu);

    eqp(lla2.lat, lla.lat);
    eqp(lla2.lon, lla.lon);
    eqp(lla2.alt, lla.alt);
}

static void test_locs(double lat, double lon, double alt, vector<vector<double>> const& vals)
{
    pos_lla reflla { degrees(lat), degrees(lon), meters(alt) };

    for (auto& v : vals) {
        test_enu(reflla, v[0], v[1], v[2], v[3], v[4], v[5]);
    }
}

TEST_CASE("Frames/Mercator")
{
    test_locs(0, 0, 0,
              { { 0, 0.1, 0, 11127.5, 0, 0 },
                { 0, -0.1, 0, -11127.5, 0, 0 },
                { 0.1, 0, 0, 0, 11053, 0 },
                { -0.1, 0, 0, 0, -11053, 0 },
                { 0, 0, 100, 0, 0, 100 },
                { 0.1, 0.1, 100, 11127.5, 11053, 100 } });

    test_locs(1, 1, 0,
              { { 1, 1.1, 0, 11125.8, 0.2, 0 },
                { 1, 0.9, 0, -11125.8, 0.2, 0 },
                { 1.1, 1, 0, 0, 11053, 0 },
                { 0.9, 1, 0, 0, -11053, 0 },
                { 1, 1, 100, 0, 0, 100 },
                { 1.1, 1.1, 100, 11125.5, 11053.2, 100 } });

    test_locs(-1, -1, 0,
              { { -1, -1.1, 0, -11125.8, -0.2, 0 },
                { -1, -0.9, 0, 11125.8, -0.2, 0 },
                { -1.1, -1, 0, 0, -11053, 0 },
                { -0.9, -1, 0, 0, 11053, 0 },
                { -1, -1, 100, 0, 0, 100 },
                { -1.1, -1.1, 100, -11125.5, -11053.2, 100 } });

    test_locs(0, 0, 100,
              { { 0, 0, 0, 0, 0, -100 }, { 0, 0, 100, 0, 0, 0 }, { 0, 0, 200, 0, 0, 100 } });
}