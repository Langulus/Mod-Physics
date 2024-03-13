///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Instance.hpp"
#include "Physics.hpp"
#include <Langulus/Mesh.hpp>

using namespace Euclidean;


/// Abstract instance construction                                            
///   @param concrete - the concrete instance type                            
///   @param producer - the world that owns the instance                      
///   @param descriptor - instance descriptor                                 
Instance::Instance(DMeta concrete, World* producer, const Neat& descriptor)
   : A::Instance {concrete}
   , ProducedFrom {producer, descriptor} {
   // Extract properties from descriptor and hierarchy                  
   //SeekValueAux<Traits::Size>(descriptor, mSize);
   VERBOSE_PHYSICS("Initializing...");
   Couple(descriptor);
   VERBOSE_PHYSICS("Initialized");
}

/// Instance construction                                                     
///   @param producer - the world that owns the instance                      
///   @param descriptor - instance descriptor                                 
Instance::Instance(World* producer, const Neat& descriptor)
   : Euclidean::Instance {MetaOf<Euclidean::Instance>(), producer, descriptor} {}

/// Refresh the instance's properties on environment change                   
void Instance::Refresh() {
   // Update the collision domain, if not pinned                        
   mDomain = SeekUnit<A::Mesh>();
}

/// Detach instance form other modules                                        
void Instance::Detach() {
   // Update the collision domain, if not pinned                        
   mDomain->Reset();
   ProducedFrom<World>::Detach();
}

/// Update the instance                                                       
void Instance::Update(Real dt) {
   mData.mVelocity +=
      (mData.mUseVelocity * dt * mData.mUseBoundness) +
      (mData.mSimVelocity * dt * mData.mSimBoundness);
   mData.mLevel +=
      (mData.mUseLevelChange * Level(dt * mData.mUseBoundness)) +
      (mData.mSimLevelChange * Level(dt * mData.mSimBoundness));

   mData.mUseVelocity = 0;
   mData.mSimVelocity = 0;
   mData.mSimLevelChange = 0;
   mData.mUseLevelChange = 0;

   mData.mPosition = mData.GetPositionNext(dt);
   mData.mVelocity = mData.GetVelocityNext(dt);
}

/// Cull the instance, based on lod state                                     
///   @param state - the lod state to test                                    
///   @return true if instance is culled (doesn't intersect frusta)           
bool Instance::Cull(const LOD& state) const noexcept {
   // Quick octave-based cull                                           
   if (state.mLevel >= mData.mLevel + 1) {
      // We're looking at the instance from a higher octave             
      // Discard - the thing is likely too small to be seen             
      // Small things will be batched as points/volumes and drawn       
      // separately. This is not the place for them                     
      return true;
   }

   if (mData.mScale.IsDegenerate()) {
      // If scale is zero, then instance is never culled                
      //TODO maybe do it opposite and use infinite scale to not cull    
      return false;
   }

   const auto box = mData.GetRangeRotated(state.mLevel);
   return not state.mFrustum.Intersects(box);
}

Level Instance::GetLevel() const noexcept {
   return mData.mLevel;
}

Mat4 Instance::GetModelTransform(const LOD& lod) const noexcept {
   return mData.GetModelTransform(lod.mLevel);
}

Mat4 Instance::GetModelTransform(const Level& level) const noexcept {
   return mData.GetModelTransform(level);
}

Mat4 Instance::GetViewTransform(const LOD& lod) const noexcept {
   return mData.GetViewTransform(lod.mLevel);
}

Mat4 Instance::GetViewTransform(const Level& level) const noexcept {
   return mData.GetViewTransform(level);
}
