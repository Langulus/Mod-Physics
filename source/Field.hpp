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
///   Field                                                                   
///                                                                           
/// Affects instances over a volume                                           
struct Euclidean::Field : Things::Field, Instance {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = World;
   using CTTI_Bases     = Things::Field; //Instance base intentionally obscured

public:
   Field(World*, Neat const&);

   void Update(Real);
   void Refresh() override;
};