///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Instance.hpp"


///                                                                           
///   Particle system                                                         
///                                                                           
/// Manages particle emitters, colliders and state transitions                
///                                                                           
struct Euclidean::Particles : Instance {
   LANGULUS_BASES(A::Physical /*Instance bases intentionally obscured*/);

public:
   Particles(World*, const Many&);

   void Update(Real);
   void Refresh() override;
};