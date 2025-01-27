#pragma once
#include "Util.hpp"


namespace AMR
{

   template<class...T>
   auto mapTuple(const std::tuple<T...>& items, auto&& func) {
      return std::apply([&](auto...x) {
         return std::make_tuple(func(x)...);
      }, items);
   }

   template<u8 N, u8 M>
   struct LoopImpl {
      using Vu64 = TVector<u64, N>;
      static constexpr u8 R = N - M;

      static void run(const Vu64& from, const Vu64& to, auto&& body, Vu64& i) {
         for (i[R] = from[R]; i[R] < to[R]; ++i[R])
            LoopImpl<N, M - 1>::run(from, to, body, i);
      }
   };

   template<u8 N>
   struct LoopImpl<N, 0> {
      using Vu64 = TVector<u64, N>;

      static void run(const Vu64&, const Vu64&, auto&& body, Vu64& i) {
         body(i);
      }
   };

   template<u8 N>
   struct Loop {
      using Vu64 = TVector<u64, N>;

      Loop(const Vu64& from, const Vu64& to, auto&& body) {
         Vu64 i;
         LoopImpl<N, N>::run(from, to, std::forward<decltype(body)>(body), i);
      }
   };

} // namespace AMR