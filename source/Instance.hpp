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
#include <Math/Instance.hpp>


namespace Euclidean
{

   ///                                                                        
   ///   An Euclidean instance                                                
   ///                                                                        
   /// Manages particles, instances, fields, constraints                      
   ///                                                                        
   struct Instance : A::Instance, ProducedFrom<Euclidean::World> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) Euclidean::World;
      LANGULUS_BASES(A::Instance);
      using T = Math::TInstance<Vec3>;

   private:
      Ref<A::Mesh> mDomain;
      T mData;

   public:
      Instance(Euclidean::World*, const Neat&);

      bool Cull(const LOD&) const noexcept override;
      Level GetLevel() const noexcept override;
      Mat4 GetModelTransform(const LOD&) const noexcept override;
      Mat4 GetModelTransform(const Level& = {}) const noexcept override;
      Mat4 GetViewTransform(const LOD&) const noexcept override;
      Mat4 GetViewTransform(const Level& = {}) const noexcept override;

      void Update(T::ScalarType);
      void Refresh();
   };

}