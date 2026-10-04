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
///                                                                           
struct Euclidean::Field : A::Field, Instance {
   using CTTI_Abstract = No;
   using CTTI_Producer = World;
   LANGULUS_BASES(A::Field /*Instance base intentionally obscured*/);

public:
   Field(World*, Many const&);

   void Update(Real);
   void Refresh() override;
};