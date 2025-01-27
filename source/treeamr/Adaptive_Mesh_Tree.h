#pragma once
#include "AMR_Tree.h"


/// Defines a tree with unknown depth                                         
///   @param T - type of contained data                                       
///   @param D... - spatial dimensions for subdividng the tree in             
template<class T>
struct Adaptive_Mesh_Tree : AMR_Tree<T> {
   int     mDimensions;
   double* mUpperBounds;
   
   Adaptive_Mesh_Tree(int dimensions, double* upper_bounds)
      : AMR_Tree<T>(dimensions, upper_bounds) {
      mDimensions = dimensions;
      mUpperBounds = upper_bounds;
   }
   
   /// Checks all objects in adaptive mesh tree to see if any objects need    
   /// to be moved to new Nodes due to recent coordinate updates. Then, the   
   /// the tree will be restructured if existing Nodes have become too        
   /// populated or too unpopulated                                           
   void updateTree() {
      // Restructure objects in tree                                    
      restructure_points();
      // Restructure tree if necessary                                  
      restructure_tree();
   }
};