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
#include <Flow/Factory.hpp>
#include <Math/Instance.hpp>
#include <Langulus/Mesh.hpp>
#include <Math/Color.hpp>


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
      RTTI::Tag<Pin<RGBA>, Traits::Color> mColor = Colors::White;

   public:
      Instance(World*, const Neat&);

      auto Cull(const LOD&) const noexcept -> bool override;
      auto GetLevel() const noexcept -> Level override;
      auto GetModelTransform(const LOD&) const noexcept -> Mat4 override;
      auto GetModelTransform(const Level& = {}) const noexcept -> Mat4 override;
      auto GetViewTransform(const LOD&) const noexcept -> Mat4 override;
      auto GetViewTransform(const Level& = {}) const noexcept -> Mat4 override;
      auto GetColor() const noexcept -> RGBA override;

      void Update(Real);
      void Refresh() override;
      void Detach();
   };

} // namespace Euclidean
