///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "World.hpp"
#include <Flow/Verbs/Create.hpp>


namespace Euclidean
{

   ///                                                                        
   ///   Physics module                                                       
   ///                                                                        
   /// Manages and produces Euclidean worlds                                  
   ///                                                                        
   struct Physics final : A::PhysicalModule {
      LANGULUS(ABSTRACT) false;
      LANGULUS_BASES(A::PhysicalModule);
      LANGULUS_VERBS(Verbs::Create);

   private:
      // List of created worlds                                         
      TFactory<World> mWorlds;

   public:
      Physics(Runtime*, const Many&);

      bool Update(Time);
      void Create(Verb&);
      void Teardown();
   };

}