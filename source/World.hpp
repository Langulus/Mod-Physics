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
#include "Bond.hpp"
#include "Field.hpp"
#include <Flow/Verbs/Create.hpp>


///                                                                           
///   An Euclidean world                                                      
///                                                                           
/// Manages particles, instances, fields, constraints                         
///                                                                           
struct Euclidean::World final : A::World, ProducedFrom<Physics> {
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
   // properties like collision domain, material properties, etc.       
   TFactory<Instance> mInstances;
   // Forces that act upon specific instances                           
   // Can be used to compose complex multi-instance/entity objects,     
   // whose parts have different behaviors like ragdolls and structures 
   TFactory<Bond> mBonds;
   // Heat, fluid, electromagnetic, whatever - fields describe          
   // behavior over a volume, affecting particles and instances         
   TFactory<Field> mFields;


   // Lowest world volume limit. Zeroes mean that the smallest unit     
   // is down to Level::Min and the floating point accuracy. Higher     
   // values define the smallest possible unit of space, essentially    
   // turning the world into an adaptive volume down to this size.      
   // Providing the same components as mLimit will reduce the dimensions
   // of the world. For example:                                        
   //    mPlanck{1,500,1} and mLimit{1000,500,1000}                     
   // means that a 2D world along XZ plane will begin as a cell of size 
   // 1000x1000 and refine down to a resolution of 1x1 as the simulation
   // advances.                                                         
   Adaptive<Vec3> mPlanck;

   // Higher world limit. Zero means that the world extends towards     
   // Level::Max and the range of the floating point number in use. If  
   // non-zero, then there's a limit imposed on the world. This is the  
   // volume of the initial adaptive grid.                              
   Adaptive<Vec3> mLimit;

public:
   World(Physics*, const Many&);

   void Refresh();
   void Update();
   void Create(Verb&);
   void Teardown();
};