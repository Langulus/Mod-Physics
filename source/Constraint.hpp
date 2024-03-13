///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>


namespace Euclidean
{

   ///                                                                        
   ///   Constraint                                                           
   ///                                                                        
   /// Connects instances, allowing them to share energy                      
   ///                                                                        
   struct Constraint : A::Physical, ProducedFrom<World> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) World;
      LANGULUS_BASES(A::Physical);

   public:
      Constraint(World*, const Neat&);

      void Update(Real);
      void Refresh() override;
   };

} // namespace Euclidean