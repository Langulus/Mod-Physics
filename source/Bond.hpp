///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Langulus/Flow/Producible.hpp>


///                                                                           
///   Bond                                                                    
///                                                                           
///   A connection between instances, that allows for transfer of energy.     
/// Can be used to compose complex multi-instance/entity objects,             
/// whose parts have different behaviors like ragdolls and structures.        
///   Can emerge from simulation on collision, when some electromagnetic or   
/// chemical interaction forms a strong bond.                                 
///                                                                           
struct Euclidean::Bond : A::Bond, ProducedFrom<World> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) World;
   LANGULUS_BASES(A::Bond);

public:
   Bond(World*, const Many&);

   void Update(Real);
   void Refresh() override;
};