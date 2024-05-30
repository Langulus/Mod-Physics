///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus.hpp>
#include <Langulus/Physical.hpp>

using namespace Langulus;
using namespace Math;


namespace Euclidean
{
   struct Physics;
   struct World;
   struct Particles;
   struct Instance;
   struct Constraint;
   struct Field;
}

#if 0
   #define VERBOSE_PHYSICS(...)      Logger::Verbose(Self(), __VA_ARGS__)
   #define VERBOSE_PHYSICS_TAB(...)  const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})
#else
   #define VERBOSE_PHYSICS(...)      LANGULUS(NOOP)
   #define VERBOSE_PHYSICS_TAB(...)  LANGULUS(NOOP)
#endif
