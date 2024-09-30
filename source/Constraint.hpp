///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
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
      Constraint(World*, const Many&);

      void Update(Real);
      void Refresh() override;
   };

} // namespace Euclidean