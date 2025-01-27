#pragma once
#include "../Common.hpp"


namespace AMR
{
   using u64 = uint64_t;
   using i64 = int64_t;
   using u32 = uint32_t;
   using i32 = int32_t;
   using  u8 = uint8_t;
   using  i8 = int8_t;

   constexpr u64 ipow(u64 base, u64 exp) {
      return exp == 0 ? 1 : base * ipow(base, exp - 1);
   }

   constexpr u64 branch_factor(u64 exp) {
      return ipow(2, exp);
   }

   constexpr u64 neighbourhood_size(u64 exp) {
      return ipow(3, exp);
   }

} // namespace AMR