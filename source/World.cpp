///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "World.hpp"
#include "Physics.hpp"
#include <Math/Gradient.hpp>

using namespace Euclidean;


/// World construction                                                        
///   @param producer - window owner                                          
///   @param descriptor - window descriptor                                   
World::World(Physics* producer, const Neat& descriptor)
   : A::World {MetaOf<Euclidean::World>(), descriptor}
   , ProducedFrom {producer, descriptor}
   , mParticles {this}
   , mInstances {this}
   , mConstraints {this}
   , mFields {this} {
   // Extract properties from descriptor and hierarchy                  
   //SeekValueAux<Traits::Size>(descriptor, mSize);
   Couple(descriptor);
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
      Logger::Warning("No time gradient found, so no update performed");
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
}

/// Introduce instances, particles, etc.                                      
///  @param verb - creation verb                                              
void World::Create(Verb& verb) {
   mInstances.Create(verb);
   mParticles.Create(verb);
   mConstraints.Create(verb);
   mFields.Create(verb);
}