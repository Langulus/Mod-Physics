///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "Bond.hpp"
#include "Physics.hpp"

using namespace Euclidean;


/// Bond construction                                                         
///   @param producer - the worlds that ouns the constraint                   
///   @param descriptor - constraint descriptor                               
Bond::Bond(World* producer, const Many& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_PHYSICS("Initializing...");
   Couple(descriptor);
   VERBOSE_PHYSICS("Initialized");
}

/// Refresh the component on environment change                               
void Bond::Refresh() {

}

/// Update the component                                                      
void Bond::Update(Real) {

}