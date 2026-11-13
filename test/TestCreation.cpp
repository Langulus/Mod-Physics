///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include <Langulus/CppAPI/Physical.hpp>
#include <Langulus/Testing.hpp>
#include <Langulus/Thing.hpp>
#include <Langulus/Allocator.hpp>

using namespace Langulus;
using Things::Thing;


template<class T>
void CreationTest(Thing& parent) {
   auto instance = parent.template CreatePart<T>();
   REQUIRE(instance.GetCount() == 1);
   REQUIRE(instance.IsSparse());
   REQUIRE(instance.template CastsTo<T>());
}

template<class T>
void CreationTestToken(Thing& parent, Token token) {
   auto instance = parent.CreatePartToken(token);
   REQUIRE(instance.GetCount() == 1);
   REQUIRE(instance.IsSparse());
   REQUIRE(instance.CastsTo<T>());
}

SCENARIO("Physics creation") {
   static MemoryState memoryState;

   GIVEN("Init and shutdown cycles") {
      for (int repeat = 0; repeat != 10; ++repeat) {
         // Create root entity                                          
         auto root = Thing::Root<false>("Physics");
         
         WHEN("The instance is created via abstraction") {
            CreationTest<Things::World>(root);
            CreationTest<Things::Instance>(root);
            CreationTest<Things::Bond>(root);
            CreationTest<Things::Field>(root);
            CreationTest<Things::Particles>(root);

            root.DumpHierarchy();
            REQUIRE(root.GetParts().GetCount() == 5);
         }

      #if LANGULUS_FEATURE(MANAGED_REFLECTION)
         WHEN("The instance is created via token") {
            CreationTestToken<Things::World>(root, "Things::World");
            CreationTestToken<Things::Instance>(root, "Things::Instance");
            CreationTestToken<Things::Bond>(root, "Things::Bond");
            CreationTestToken<Things::Field>(root, "Things::Field");
            CreationTestToken<Things::Particles>(root, "Things::Particles");

            root.DumpHierarchy();
            REQUIRE(root.GetParts().GetCount() == 5);
         }
      #endif

         REQUIRE(memoryState.Assert());
         REQUIRE_FALSE(Allocator::CollectGarbage());

         root.Reference(-1);
      }
   }
}

