///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"


namespace Euclidean
{

   ///                                                                        
   ///   An Euclidean instance                                                
   ///                                                                        
   /// Manages particles, instances, fields, constraints                      
   ///                                                                        
   struct Instance final : A::Instance, ProducedFrom<Euclidean::World> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) Euclidean::World;
      LANGULUS_BASES(A::Instance);

   private:
      Ref<A::Mesh> mDomain;

   public:
      Instance(Euclidean::World*, const Neat&);

      bool Cull(const LodState&) const override;
      MatrixType GetLeveledTransformation(const LodState&) const override;
      MemberType Distance(const A::Instance*) const override;
      bool Intersect(const A::Instance*) const override;
      void LookAt(const vec4&) override;

      void Update(MemberType);
      void Refresh();
   };

}