///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Field.hpp"
#include "Physics.hpp"

using namespace Euclidean;


/// World construction                                                        
///   @param producer - window owner                                          
///   @param descriptor - window descriptor                                   
Field::Field(World* producer, const Neat& descriptor)
   : Instance {MetaOf<Field>(), producer, descriptor} {}

/// Refresh the world component on environment change                         
void Field::Refresh() {

}

/// Update the window                                                         
void Field::Update(Real) {

}