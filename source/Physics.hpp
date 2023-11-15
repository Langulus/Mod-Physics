///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
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
      TFactory<World> mWorlds;

   public:
      Physics(Runtime*, const Neat&);

      bool Update(Time);
      void Create(Verb&);
   };

}