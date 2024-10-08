///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Physics.hpp"

#include <Math/Normal.hpp>
#include <Math/Primitives.hpp>
#include <Math/Angle.hpp>
#include <Math/SimplexNoise.hpp>
#include <Math/Config.hpp>

LANGULUS_DEFINE_MODULE(
   Euclidean::Physics, 9, "Physics",
   "Euclidean physics module, implementing spatiality", "",
   Euclidean::Physics, Euclidean::World, Euclidean::Instance
)

using namespace Euclidean;


/// Module construction                                                       
///   @param runtime - the runtime that owns the module                       
///   @param descriptor - instructions for configuring the module             
Physics::Physics(Runtime* runtime, const Many&)
   : Resolvable {this}
   , Module     {runtime} {
   VERBOSE_PHYSICS("Initializing...");
   Math::RegisterVectors();
   Math::RegisterRanges();
   Math::RegisterNormals();
   Math::RegisterPrimitives();
   Math::RegisterAngles();
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
   mWorlds.Create(this, verb);
}