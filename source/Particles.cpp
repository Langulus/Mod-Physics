///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Particles.hpp"
#include "Physics.hpp"

using namespace Euclidean;


/// Particle system construction                                              
///   @param producer - window owner                                          
///   @param descriptor - window descriptor                                   
Particles::Particles(World* producer, const Neat& descriptor)
   : Resolvable {this}
   , Instance   {producer, descriptor} {
   VERBOSE_PHYSICS("Initializing...");
   Couple(descriptor);
   VERBOSE_PHYSICS("Initialized");
}

/// Refresh the particle system on environment change                         
void Particles::Refresh() {

}

/// Update the particle system                                                
void Particles::Update(Real) {

}