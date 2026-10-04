///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/Factory.hpp>
#include <Langulus/Math/Instance.hpp>
#include <Langulus/CppAPI/Mesh.hpp>
#include <Langulus/Color.hpp>


///                                                                           
///   An Euclidean instance                                                   
///                                                                           
/// Manages position and orientation of particles, instances, fields,         
/// constraints, and anything that can be instantiated in space.              
///                                                                           
struct Euclidean::Instance : Things::Instance, ProducedFrom<Euclidean::World> {
   using CTTI_Abstract  = No;
   using CTTI_Producer  = Euclidean::World;
   using CTTI_Bases     = Things::Instance;
   using CTTI_Ability   = Verbs::Move;

private:
   // Collision domain                                                  
   Pin<Ref<Things::Mesh>> mDomain;
   // Instance data                                                     
   Math::TInstance<Vec3> mData;
   // Instance color                                                    
   Pin<RGBA, Tags::Color> mColor = Colors::White;

public:
   Instance(World*, Many const&);

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