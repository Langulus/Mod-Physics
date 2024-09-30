///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Instance.hpp"
#include "Particles.hpp"
#include "Constraint.hpp"
#include "Field.hpp"
#include <Flow/Verbs/Create.hpp>


namespace Euclidean
{

   ///                                                                        
   ///   An Euclidean world                                                   
   ///                                                                        
   /// Manages particles, instances, fields, constraints                      
   ///                                                                        
   struct World final : A::World, ProducedFrom<Physics> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) Physics;
      LANGULUS_BASES(A::World);
      LANGULUS_VERBS(Verbs::Create);

   private:
      // Particle systems are optimized for large quantity of           
      // instances that share the same physical behavior                
      TFactory<Particles> mParticles;
      // Simulated instances in the world - each instance contains the  
      // spatial data for a thing, and serves as a manager for physical 
      // properties, like collision domain, material properties, etc.   
      TFactory<Instance> mInstances;
      // Forces that act upon specific instances                        
      // Can be used to compose complex multi-instance/entity objects,  
      // whose parts have different behaviors, like for example         
      // ragdolls and structures                                        
      TFactory<Constraint> mConstraints;
      // Heat, fluid, electromagnetic, whatever - fields describe       
      // behavior over a volume, affecting particles and instances      
      TFactory<Field> mFields;

   public:
      World(Physics*, const Many&);

      void Detach();
      void Refresh();
      void Update();
      void Create(Verb&);
   };

} // namespace Euclidean
