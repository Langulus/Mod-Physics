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
#include <Flow/Factory.hpp>


namespace Euclidean
{

   ///                                                                        
   ///   An Euclidean instance                                                
   ///                                                                        
   /// Manages particles, instances, fields, constraints                      
   ///                                                                        
   struct Instance : A::Instance, ProducedFrom<World> {
      LANGULUS(ABSTRACT) false;
      LANGULUS(PRODUCER) World;
      LANGULUS_BASES(A::Instance);
      using T = Math::TInstance<Vec3>;

   private:
      Pin<Ref<A::Mesh>> mDomain;
      T mData;

   public:
      Instance(World*, const Neat&);
      Instance(DMeta, World*, const Neat&);

      bool Cull(const LOD&) const noexcept override;
      Level GetLevel() const noexcept override;
      Mat4 GetModelTransform(const LOD&) const noexcept override;
      Mat4 GetModelTransform(const Level& = {}) const noexcept override;
      Mat4 GetViewTransform(const LOD&) const noexcept override;
      Mat4 GetViewTransform(const Level& = {}) const noexcept override;

      void Update(Real);
      void Refresh() override;
      void Detach();
   };

} // namespace Euclidean
