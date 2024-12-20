///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Main.hpp"
#include <Langulus/Physical.hpp>
#include <catch2/catch.hpp>


/// See https://github.com/catchorg/Catch2/blob/devel/docs/tostring.md        
CATCH_TRANSLATE_EXCEPTION(::Langulus::Exception const& ex) {
   const Text serialized {ex};
   return ::std::string {Token {serialized}};
}

SCENARIO("Window creation", "[window]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("Physics");
         
         WHEN("The instance is created via abstraction") {
            auto world = root.CreateUnit<A::World>();
            auto instance = root.CreateUnit<A::Instance>();
            root.DumpHierarchy();

            REQUIRE(world.GetCount() == 1);
            REQUIRE(world.IsSparse());
            REQUIRE(world.CastsTo<A::World>());

            REQUIRE(instance.GetCount() == 1);
            REQUIRE(instance.IsSparse());
            REQUIRE(instance.CastsTo<A::Instance>());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The instance is created via token") {
            auto world = root.CreateUnitToken("A::World");
            auto instance = root.CreateUnitToken("A::Instance");

            root.DumpHierarchy();

            REQUIRE(world.GetCount() == 1);
            REQUIRE(world.IsSparse());
            REQUIRE(world.CastsTo<A::World>());
               
            REQUIRE(instance.GetCount() == 1);
            REQUIRE(instance.IsSparse());
            REQUIRE(instance.CastsTo<A::Instance>());

            REQUIRE(root.GetUnits().GetCount() == 2);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());
      }
   }
}

