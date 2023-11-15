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


namespace Euclidean
{

   /// World construction                                                     
   ///   @param producer - window owner                                       
   ///   @param descriptor - window descriptor                                
   World::World(Physics* producer, const Neat& descriptor)
      : A::World {MetaOf<Euclidean::World>(), descriptor}
      , ProducedFrom {producer, descriptor} {
      // Extract properties from descriptor and hierarchy               
      //SeekValueAux<Traits::Size>(descriptor, mSize);
   }

   /// Refresh the world component on environment change                      
   void World::Refresh() {
      // Refresh unpinned properties from hierarchy                     
      //SeekValue<Traits::Size>(mSize);
   }

   /// Update the window                                                      
   void World::Update() {
       // Get the time gradient                                         
       TimeGradient timeGradient;
       if (!SeekValue<Traits::Time>(timeGradient)) {
           pcLogSelfError << "No time gradient found, so no update performed";
           return;
       }

       if (timeGradient.Delta() == 0) {
           // No time had passed, so no need to update anything         
           // This is not an error                                      
           return;
       }

       // Calculate the real delta time factor with required precision  
       real deltaTimeReal = timeGradient.Delta().SecondsReal();

       for (auto& field : mFields)
           field.Update(deltaTimeReal);

       for (auto& instance : mInstances)
           instance.Update(deltaTimeReal);

       for (auto& constraint : mConstraints)
           constraint.Update(deltaTimeReal);

       for (auto& particle : mParticles)
           particle.Update(deltaTimeReal);
   }

   /// Introduce instances, particles, etc.                                     
   ///  @param verb - creation verb                                             
   void World::Create(Verb& verb) {
       mInstances.Create(verb);
       mFractals.Create(verb);
       mParticles.Create(verb);
       mConstraints.Create(verb);
       mFields.Create(verb);
       mStructures.Create(verb);
       mHeats.Create(verb);
       mFluids.Create(verb);
   }
}