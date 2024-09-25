///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
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
      LANGULUS_BASES(A::Physical /*Instance bases intentionally obscured*/);

   public:
      Particles(World*, const Neat&);

      void Update(Real);
      void Refresh() override;
   };

}