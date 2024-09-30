///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Constraint.hpp"
#include "Physics.hpp"

using namespace Euclidean;


/// Constraint construction                                                   
///   @param producer - the worlds that ouns the constraint                   
///   @param descriptor - constraint descriptor                               
Constraint::Constraint(World* producer, const Many& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_PHYSICS("Initializing...");
   Couple(descriptor);
   VERBOSE_PHYSICS("Initialized");
}

/// Refresh the world component on environment change                         
void Constraint::Refresh() {

}

/// Update the window                                                         
void Constraint::Update(Real) {

}