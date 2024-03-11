///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Physics.hpp"

LANGULUS_DEFINE_MODULE(
   Euclidean::Physics, 9, "Physics",
   "Euclidean physics module, implementing spatiality", "",
   Euclidean::Physics, Euclidean::World, Euclidean::Instance
)

using namespace Euclidean;


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
Physics::Physics(Runtime* runtime, const Neat&)
   : A::PhysicalModule {MetaOf<Physics>(), runtime}
   , mWorlds {this} {
   VERBOSE_PHYSICS("Initializing...");
   VERBOSE_PHYSICS("Initialized");
}

/// Module update routine                                                     
///   @param dt - time from last update                                       
bool Physics::Update(Time) {
   for (auto& world : mWorlds)
      world.Update();
   return true;
}

/// Create/Destroy worlds                                                     
///   @param verb - the creation/destruction verb                             
void Physics::Create(Verb& verb) {
   mWorlds.Create(verb);
}