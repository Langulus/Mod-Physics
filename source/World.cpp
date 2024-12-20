///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "World.hpp"
#include "Physics.hpp"
#include <Flow/Time.hpp>
#include <Math/Gradient.hpp>

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
   mConstraints.Teardown();
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
   Math::TGrad<Time> timeGradient;
   if (not SeekValue<Traits::Time>(timeGradient)) {
      Logger::Warning(Self(), "No time gradient found, so no update performed");
      return;
   }

   if (not timeGradient.Delta()) {
      // No time had passed, so no need to update anything              
      // This is not an error                                           
      return;
   }

   // Calculate the real delta time factor with required precision      
   const auto timeAsReal = timeGradient.Delta().Seconds();

   for (auto& field : mFields)
      field.Update(timeAsReal);
   for (auto& instance : mInstances)
      instance.Update(timeAsReal);
   for (auto& constraint : mConstraints)
      constraint.Update(timeAsReal);
   for (auto& particle : mParticles)
      particle.Update(timeAsReal);

   Logger::Info(Self(), ": Updated");
}

/// Introduce instances, particles, etc.                                      
///  @param verb - creation verb                                              
void World::Create(Verb& verb) {
   mInstances.Create(this, verb);
   mParticles.Create(this, verb);
   mConstraints.Create(this, verb);
   mFields.Create(this, verb);
}