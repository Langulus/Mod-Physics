///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "World.hpp"
#include "Physics.hpp"
#include <Langulus/Flow/Time.hpp>
#include <Langulus/Math/Gradient.hpp>

using namespace Euclidean;


/// World construction                                                        
///   @param producer - window owner                                          
///   @param descriptor - window descriptor                                   
World::World(Physics* producer, const Many& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   // Extract properties from descriptor and hierarchy                  
   //SeekValueAux<Traits::Size>(descriptor, mSize);
   VERBOSE_PHYSICS("Initializing...");
   Couple(descriptor);
   VERBOSE_PHYSICS("Initialized");
}

/// First stage destruction                                                   
void World::Teardown() {
   mFields.Teardown();
   mInstances.Teardown();
   mBonds.Teardown();
   mParticles.Teardown();
}

/// Refresh the world component on environment change                         
void World::Refresh() {
   // Refresh unpinned properties from hierarchy                        
   //SeekValue<Traits::Size>(mSize);
}

/// Update the window                                                         
void World::Update() {
   // Get the time gradient                                             
   const auto flow = GetFlow();
   if (not flow) {
      VERBOSE_PHYSICS(Logger::DarkYellow, "No time gradient found, so no update performed");
      return;
   }

   // Calculate the real delta time factor with required precision      
   const auto timeAsReal = GetFlow()->GetDeltaTime().Seconds();

   // Update all components of the simulation                           
   for (auto& field : mFields)
      field.Update(timeAsReal);
   for (auto& instance : mInstances)
      instance.Update(timeAsReal);
   for (auto& bonds : mBonds)
      bonds.Update(timeAsReal);
   for (auto& particle : mParticles)
      particle.Update(timeAsReal);

   VERBOSE_PHYSICS("Advanced ", timeAsReal, " seconds");
}

/// Introduce instances, particles, etc.                                      
///  @param verb - creation verb                                              
void World::Create(Verb& verb) {
   mInstances.Create(this, verb);
   mParticles.Create(this, verb);
   mBonds.Create(this, verb);
   mFields.Create(this, verb);
}