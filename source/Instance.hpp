///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Common.hpp"
#include <Langulus/Flow/Factory.hpp>
#include <Langulus/Math/Instance.hpp>
#include <Langulus/Mesh.hpp>
#include <Langulus/Math/Color.hpp>


///                                                                           
///   An Euclidean instance                                                   
///                                                                           
/// Manages particles, instances, fields, constraints                         
///                                                                           
struct Euclidean::Instance : A::Instance, ProducedFrom<World> {
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
   Instance(World*, const Many&);

   void Move(Verb&);

   void Update(Real);
   void Refresh() override;
   void Teardown();
   auto Cull(const LOD&) const noexcept -> bool override;
   auto GetLevel() const noexcept -> Level override;
   auto GetModelTransform(const LOD&) const noexcept -> Mat4 override;
   auto GetModelTransform(const Level& = {}) const noexcept -> Mat4 override;
   auto GetViewTransform(const LOD&) const noexcept -> Mat4 override;
   auto GetViewTransform(const Level& = {}) const noexcept -> Mat4 override;
   auto GetColor() const noexcept -> RGBA override;
};