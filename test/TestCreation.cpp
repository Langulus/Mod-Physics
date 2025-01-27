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

template<class T>
void CreationTest(Thing& parent) {
   auto instance = parent.template CreateUnit<T>();
   REQUIRE(instance.GetCount() == 1);
   REQUIRE(instance.IsSparse());
   REQUIRE(instance.template CastsTo<T>());
}

template<class T>
void CreationTestToken(Thing& parent, Token token) {
   auto instance = parent.CreateUnitToken(token);
   REQUIRE(instance.GetCount() == 1);
   REQUIRE(instance.IsSparse());
   REQUIRE(instance.CastsTo<T>());
}

SCENARIO("Physics creation", "[physics]") {
   static Allocator::State memoryState;

   for (int repeat = 0; repeat != 10; ++repeat) {
      GIVEN(std::string("Init and shutdown cycle #") + std::to_string(repeat)) {
         // Create root entity                                          
         auto root = Thing::Root<false>("Physics");
         
         WHEN("The instance is created via abstraction") {
            CreationTest<A::World>(root);
            CreationTest<A::Instance>(root);
            CreationTest<A::Bond>(root);
            CreationTest<A::Field>(root);
            CreationTest<A::Particles>(root);

            root.DumpHierarchy();
            REQUIRE(root.GetUnits().GetCount() == 5);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The instance is created via token") {
            CreationTestToken<A::World>(root, "A::World");
            CreationTestToken<A::Instance>(root, "A::Instance");
            CreationTestToken<A::Bond>(root, "A::Bond");
            CreationTestToken<A::Field>(root, "A::Field");
            CreationTestToken<A::Particles>(root, "A::Particles");

            root.DumpHierarchy();
            REQUIRE(root.GetUnits().GetCount() == 5);
         }
      #endif

         // Check for memory leaks after each cycle                     
         REQUIRE(memoryState.Assert());

         root.Reference(-1);
      }
   }
}

