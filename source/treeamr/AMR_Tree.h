#pragma once
#include "Node.h"


/// Defines a general purpose tree that is designed to contain discrete       
/// objects, and not continuous properties that can be upscaled or downscaled 
///   @param T - type of contained data                                       
///   @param D... - spatial dimensions for subdividing the tree in            
template<class T, CT::Dimension...D>
class AMR_Tree : public Node<T, D...> {
protected:
   static constexpr Count Dimensions = sizeof...(D);
   static_assert(Dimensions > 0, "Dimensions should be greater than zero");
   using Vector = TVector<Real, Dimensions>;
   using Root   = Node<T, D...>;

   Vector mGlobalLower;
   Vector mGlobalUpper;

public:
   AMR_Tree() = delete;

   /// Create a tree                                                          
   ///   @param upper_bounds - the length of the system along each dimension  
   AMR_Tree(const Vector& upper_bounds)
      : Root         {nullptr, 0, upper_bounds}
      , mGlobalUpper {upper_bounds} {}

   /// Recursively generate children nodes until depth is achieved            
   ///   @param depth - depth of AMR tree (number of levels)                  
   void set_depth(int depth) {
      Root::set_depth(depth - 1);
   }
};
