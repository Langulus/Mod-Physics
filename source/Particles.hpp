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
struct Euclidean::Particles : A::Particles, Instance {
   using CTTI_Abstract = No;
   using CTTI_Producer = World;
   LANGULUS_BASES(A::Particles /*Instance base intentionally obscured*/);

public:
   Particles(World*, Many const&);

   void Update(Real);
   void Refresh() override;
};