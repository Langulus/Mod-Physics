///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Instance.hpp"


namespace Euclidean
{

   ///                                                                        
   ///   Particle system                                                      
   ///                                                                        
   /// Manages particle emitters, colliders and state transitions             
   ///                                                                        
   struct Particles : Instance {
      LANGULUS_BASES(Instance);
      using Instance::T;

   public:
      Particles(World*, const Neat&);

      void Update(Real);
      void Refresh() override;
   };

}