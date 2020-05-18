#include "catch2/catch.hpp"
#include "sico/types/stream.hpp"

using namespace sico;
using namespace sico::literals;

template<class T, class U>
T gen(double v)
{
    return { U(v), U(v), U(v) };
}

template<class T, class U>
void test()
{
    REQUIRE(gen<T, U>(1) == gen<T, U>(1));
    REQUIRE(gen<T, U>(1) != gen<T, U>(2));
    REQUIRE(gen<T, U>(1) + gen<T, U>(2) == gen<T, U>(3));
    REQUIRE(gen<T, U>(3) - gen<T, U>(1) == gen<T, U>(2));
    REQUIRE(gen<T, U>(3) * 2 == gen<T, U>(6));
    REQUIRE(gen<T, U>(4) / 2 == gen<T, U>(2));

    REQUIRE(norm(gen<T, U>(0)) == U(0));
    REQUIRE(norm(gen<T, U>(1)) == U(sqrt(3.0)));
    REQUIRE(norm(gen<T, U>(2)) == U(sqrt(12.0)));
}

TEST_CASE("Types/Operators")
{
    test<pos_ecef, meters>();
    test<pos_enu, meters>();
    test<pos_local, meters>();

    REQUIRE(pos_ned { 1_m, 2_m, 3_m } == pos_ned(pos_enu { 2_m, 1_m, -3_m }));
    REQUIRE(pos_enu { 1_m, 2_m, 3_m } == pos_enu(pos_ned { 2_m, 1_m, -3_m }));

    REQUIRE(vel_enu { 1_m / 1_s, 2_m / 1_s, 3_m / 1_s } * 2_s == pos_enu { 2_m, 4_m, 6_m });

    REQUIRE(pos_lla { 0_rad, 0_rad, 0_m } + pos_enu { 0_m, 0_m, 10_m }
            == pos_lla { 0_rad, 0_rad, 10_m });
}