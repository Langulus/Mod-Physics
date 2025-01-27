#include <catch2/catch.hpp>
#include "../../source/amr/Buffer.hpp"


TEST_CASE("Allocating buffers", "[buffer]") {
   CHECK_THROWS((AMR::Buffer<int, 2>({0, 5})));
   CHECK_THROWS((AMR::Buffer<int, 4>({3, 0, 4, 6})));
   CHECK_NOTHROW((AMR::Buffer<int, 1>(1)));
   CHECK_NOTHROW((AMR::Buffer<int, 1>(10)));
   CHECK_NOTHROW((AMR::Buffer<int, 5>({1, 1, 1, 1, 1})));
   CHECK_NOTHROW((AMR::Buffer<int, 5>({6, 2, 4, 3, 10})));
}

TEST_CASE("Calculating stride", "[buffer]") {
   {
      AMR::Buffer<int, 3> buf({1, 1, 1});
      CHECK(buf.mStride[0] == 1);
      CHECK(buf.mStride[1] == 1);
   }
   {
      AMR::Buffer<int, 4> buf({4, 5, 6, 7});
      CHECK(buf.mStride[0] == 4);
      CHECK(buf.mStride[1] == 20);
      CHECK(buf.mStride[2] == 120);
   }
}

TEST_CASE("Creating arrays", "[buffer]") {
   // TODO
}

TEST_CASE("Array indexing", "[buffer]") {
   // TODO
}
