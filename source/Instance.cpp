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


/// Instance construction                                                     
///   @param producer - the world that owns the instance                      
///   @param descriptor - instance descriptor                                 
Instance::Instance(World* producer, const Neat& descriptor)
   : Resolvable {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_PHYSICS("Initializing...");
   Couple(descriptor);
   SeekValueAux<Traits::Place>(descriptor, mData.mPosition);
   SeekValueAux<Traits::Size>(descriptor, mData.mScale);
   SeekValueAux<Traits::Aim>(descriptor, mData.mAim);
   SeekValueAux<Traits::Level>(descriptor, mData.mLevel);
   SeekValueAux<Traits::Acceleration>(descriptor, mData.mAcceleration);
   SeekValueAux<Traits::Bilateral>(descriptor, mData.mBilateral);
   SeekValueAux<Traits::Pickable>(descriptor, mData.mPickable);
   SeekValueAux<Traits::Solid>(descriptor, mData.mSolid);
   SeekValueAux<Traits::Velocity>(descriptor, mData.mVelocity);
   SeekValueAux<Traits::Static>(descriptor, mData.mStatic);
   SeekValueAux(descriptor, mColor);
   VERBOSE_PHYSICS("Initialized");
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

/// Refresh the instance's properties on environment change                   
void Instance::Refresh() {
   mDomain = SeekUnit<A::Mesh>();
}

/// Detach instance from other modules                                        
void Instance::Detach() {
   mDomain->Reset();
   ProducedFrom::Detach();
}

/// Move, rotate, resize verb                                                 
///   @param verb - the resize verb                                           
void Instance::Move(Verb& verb) {
   if (verb.GetMass() <= 0)
      TODO();

   const auto mass = verb.GetMass();
   const auto sign = Sign(mass);
   bool   relative = true;
   Yaw    total_yaw;
   Pitch  total_pitch;
   Roll   total_roll;
   std::optional<Point3> total_point;
   Scale3 total_scale;
   Force3 total_move;

   verb.ForEachDeep(
      [&](const Traits::Relative& rel) {
         relative = rel.AsCast<bool>();
      },
      [&](const Yaw& yaw) noexcept {
         total_yaw = yaw * mass;
      },
      [&](const Pitch& pitch) noexcept {
         total_pitch = pitch * mass;
      },
      [&](const Roll& roll) noexcept {
         total_roll = roll * mass;
      },
      [&](const Scale& scale) noexcept {
         total_scale = scale * mass;
      },
      [&](const Point& point) noexcept {
         total_point = point;
      },
      [&](const Force& force) noexcept {
         total_move = force;
      },
      [&](const Vec3& v3) noexcept {
         total_move = v3 * mass;
      },
      [&](const Vec4& v4) noexcept {
         total_move = v4 * mass;
      },
      [&](const Vec2& v2) noexcept {
         total_move = v2 * mass;
      }
   );

   if (total_yaw)
      mData.Rotate(sign, total_yaw, relative);
   if (total_pitch)
      mData.Rotate(sign, total_pitch, relative);
   if (total_roll)
      mData.Rotate(sign, total_roll, relative);

   if (total_point) {
      if (relative)
         mData.SetPosition<true>(total_point.value());
      else
         mData.SetPosition<false>(total_point.value());
   }

   if (total_scale) {
      if (relative)
         mData.SetScale<true>(total_scale);
      else
         mData.SetScale<false>(total_scale);
   }

   if (total_move)
      mData.Move(sign, total_move, relative);
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

auto Instance::GetColor() const noexcept -> RGBA {
   return *mColor;
}
