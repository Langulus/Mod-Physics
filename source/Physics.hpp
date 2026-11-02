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
struct Euclidean::Physics final : Things::PhysicalModule {
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Things::PhysicalModule;
   using CTTI_Ability   = Verbs::Create;

private:
   // List of created worlds                                         
   TFactory<Euclidean::World> mWorlds;

public:
   Physics(Runtime*, Neat const&);

   bool Update(Time);
   void Create(Verb&);
   void Teardown();
};