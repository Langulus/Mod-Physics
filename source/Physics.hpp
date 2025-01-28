///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "World.hpp"
#include <Langulus/Verbs/Create.hpp>


///                                                                           
///   Physics module                                                          
///                                                                           
/// Manages and produces Euclidean worlds                                     
///                                                                           
struct Euclidean::Physics final : A::PhysicalModule {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::PhysicalModule);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of created worlds                                         
   TFactory<Euclidean::World> mWorlds;

public:
   Physics(Runtime*, const Many&);

   bool Update(Time);
   void Create(Verb&);
   void Teardown();
};