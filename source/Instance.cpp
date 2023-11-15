///                                                                           
/// Langulus::Module::Physics                                                 
/// Copyright (c) 2017 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "Instance.hpp"
#include "World.hpp"


namespace Euclidean
{

   /// World construction                                                     
   ///   @param producer - window owner                                       
   ///   @param descriptor - window descriptor                                
   Instance::Instance(Euclidean::World* producer, const Neat& descriptor)
      : A::Instance {MetaOf<Euclidean::Instance>(), descriptor}
      , ProducedFrom {producer, descriptor} {
      // Extract properties from descriptor and hierarchy               
      //SeekValueAux<Traits::Size>(descriptor, mSize);
   }

   /// Refresh the world component on environment change                      
   void Instance::Refresh() {
       // Scan for geometry unit, to be used as a collision domain				
       mCollider.Reset();
       for (auto owner : GetOwners()) {
           for (auto unit : owner->GetUnits()) {
               auto geometry = dynamic_cast<AGeometry*>(unit);
               if (geometry)
                   mCollider = geometry;
           }
       }

       // Save the results																	
       /*if (mCollider) {
           mCollider->SeekValue<Traits::Scale>(mScale);
           mCollider->SeekValue<Traits::Position>(mPosition);
           mCollider->SeekValue<Traits::Octave>(mOctave);
       }

       // Override traits from owner, if any
       SeekValue<Traits::Scale>(mScale);
       SeekValue<Traits::Position>(mPosition);
   	SeekValue<Traits::Octave>(mOctave);*/
   }

   /// Update the window                                                      
   void Instance::Update() {
       mVelocity +=
           (mUseVelocity * dt * mUseBoundness) +
           (mSimVelocity * dt * mSimBoundness);
       mLevel +=
           (mUseLevelChange * Level(dt * mUseBoundness)) +
           (mSimLevelChange * Level(dt * mSimBoundness));

       mUseVelocity = 0;
       mSimVelocity = 0;
       mSimLevelChange = 0;
       mUseLevelChange = 0;

       mPosition = GetPositionNext(dt);
       mVelocity = GetVelocityNext(dt);
   }

    /// Cull the instance, based on lod state													
    ///	@param state - the lod state to test												
    ///	@return true if instance is culled (doesn't intersect frusta)				
    bool Instance::Cull(const LodState& state) const {
	    // Quick octave-based cull															
	    if (state.mLevel >= mLevel + 1) {
		    // We're looking at the instance from a higher octave					
		    // Discard - the thing is likely too small to be seen					
		    // Small things will be batched as points/volumes and drawn			
		    // separately. This is not the place for them							
		    return true;
	    }

	    if (mScale.IsDegenerate()) {
		    // If scale is zero, then instance is never culled						
		    //TODO maybe do it opposite and use infinite scale to not cull		
		    return false;
	    }

	    const auto box = GetRangeRotated(state.mLevel);
	    return !state.mFrustum.Intersects(box);
    }

    /// Generate all transformations associated with the instance						
    ///	@param output - [out] results go here												
    ///	@param state - lod state																
    ///	@return the number of generated transformations									
    Instance::MatrixType Instance::GetLeveledTransformation(const LodState& state) const {
	    return GetModelTransform(state.mLevel);
    }

    /// Get distance from another instance														
    ///	@param what - the instance to find distance to									
    ///	@return the distance																		
    Instance::MemberType Instance::Distance(const AInstance*) const {
	    TODO();
	    return 0;
    }

    /// Check if this instance intersects another											
    ///	@param what - the instance to find distance to									
    ///	@return true on intersection															
    bool Instance::Intersect(const AInstance*) const {
	    TODO();
	    return false;
    }

    /// Change orientation of the instance, to look at a specific point				
    ///	@param what - the point to look at (4th element is the octave)				
    void Instance::LookAt(const vec4&) {
	    TODO();
    }

    /// Constrain the instance																		
    ///	@param verb - constrain verb															
    void Instance::Constrain(Verb& verb) {
	    // Extract symbolic constraint range from left side						
	    range4 range;
	    verb.GetArgument().ForEachDeep([&range](const Block& group) {
		    group.ForEach([&range](const range1& item) { range = range4(item); });
		    group.ForEach([&range](const range2& item) { range = range4(item); });
		    group.ForEach([&range](const range3& item) { range = range4(item); });
		    group.ForEach([&range](const range4& item) { range = item; });
	    });

	    range.mMin[3] = range.mMax[3] = 0;
	
	    // Use the extracted range to constain against other instances			
	    verb.GetArgument().ForEachDeep([&](const Block& group) {
		    group.ForEach([&](const Entity& entity) {
			    auto instance = entity.GetUnit<CInstance>();
			    if (!instance)
				    return;

			    if (!(mSolid && mSigned) && (instance->mSolid && instance->mSigned)) {
				    // The simulation will automatically constrain it				
				    verb.Done();
				    CINSTANCE_VERBOSE(
					    "Constrain ignored - simulation will deliver");
				    return;
			    }

			    ConstrainPosition(*instance, range);
			    verb.Done();
			    CINSTANCE_VERBOSE(
				    "Constrain inside range of " << ent << ": " << instance->mData.mPosition);
		    });
	    });
    }

    /// Collide with other instances																
    ///	@param verb - collide verb																
    void Instance::Collide(Verb&) {
	    TODO();
    }

    /// Snap instances to other instances														
    ///	@param verb - snap verb																	
    void Instance::Snap(Verb& verb) {
	    // Get the scale of the grid first												
	    vec4 scale = {};
	    verb.GetArgument().ForEachDeep([&scale](const Block& group) {
		    group.ForEach([&scale](const vec4& vector) { scale = vector; });
	    });

	    // Use the extracted range to constain against other instances			
	    verb.GetArgument().ForEachDeep([&](const Block& group) {
		    group.ForEach([&](const Entity& entity) {
			    auto instance = entity.SeekUnit<CInstance>(SeekStyle::UpToHere);
			    if (!instance)
				    return;

			    CINSTANCE_VERBOSE(
				    "Snapping to " << instance << " with scale " << scale);

			    // Get initial offset														
			    mPosition = pcFloor(instance->mPosition / scale) * scale;
			    verb.Done();
		    });
	    });
    }
}