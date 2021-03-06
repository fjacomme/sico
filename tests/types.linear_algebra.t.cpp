#include "catch2/catch.hpp"
#include "sico/types/linear_algebra.hpp"

using namespace sico;

#ifndef SICO_USE_EIGEN
template<class T>
T gen(double v)
{
    T t;
    t.fill(v);
    return t;
}

template<class T>
void test()
{
    REQUIRE(gen<T>(1) == gen<T>(1));
    REQUIRE(gen<T>(1) != gen<T>(2));
    REQUIRE(gen<T>(1) + gen<T>(2) == gen<T>(3));
    REQUIRE((gen<T>(1) + gen<T>(2)) == gen<T>(3));
    {
        auto v = gen<T>(1);
        v += 2;
        REQUIRE(v == gen<T>(3));
    }
    REQUIRE(gen<T>(3) - gen<T>(1) == gen<T>(2));
    {
        auto v = gen<T>(3);
        v -= 1;
        REQUIRE(v == gen<T>(2));
        v -= gen<T>(1);
        REQUIRE(v == gen<T>(1));
    }
    REQUIRE(gen<T>(3) * 2 == gen<T>(6));
    {
        auto v = gen<T>(3);
        v *= 2;
        REQUIRE(v == gen<T>(6));
        v /= 2;
        REQUIRE(v == gen<T>(3));
    }
    REQUIRE(gen<T>(4) / 2 == gen<T>(2));

    auto const TS = size(T());

    REQUIRE(length(gen<T>(1)) == sqrt(TS));
    REQUIRE(normalized(gen<T>(3)) == gen<T>(sqrt(TS) / TS));
}

TEST_CASE("Types/LinearAlgebra")
{
    test<vector3d>();
    test<quaternion>();
}
#endif