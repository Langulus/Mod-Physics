#include <catch2/catch.hpp>
#include "../../source/amr/Control.hpp"
#include <iostream>
#include <limits>


TEST_CASE("mapTuple", "[control]")
{
    auto y = AMR::mapTuple(std::make_tuple(1, 2.0), [](auto x) { return x+1;});
    CHECK(std::get<0>(y) == 2);
    CHECK(std::get<1>(y) == 3.0);
}

TEST_CASE("Loop::simple", "[control]")
{
}
