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
      LANGULUS_VERBS(Verbs::Move);

   private:
      // Collision domain                                               
      Pin<Ref<A::Mesh>> mDomain;
      // Instance data                                                  
      Math::TInstance<Vec3> mData;
      // Instance color                                                 
      RTTI::Tag<Pin<RGBA>, Traits::Color> mColor = Colors::White;

   public:
      Instance(World*, const Neat&);

      void Update(Real);
      void Refresh() override;
      void Detach();

      void Move(Verb&);

      auto Cull(const LOD&) const noexcept -> bool override;
      auto GetLevel() const noexcept -> Level override;
      auto GetModelTransform(const LOD&) const noexcept -> Mat4 override;
      auto GetModelTransform(const Level& = {}) const noexcept -> Mat4 override;
      auto GetViewTransform(const LOD&) const noexcept -> Mat4 override;
      auto GetViewTransform(const Level& = {}) const noexcept -> Mat4 override;
      auto GetColor() const noexcept -> RGBA override;
   };

} // namespace Euclidean
