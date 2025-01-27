#pragma once
#include "AMR_Tree.h"


/// Same as adaptive tree, but with predefined depth                          
template <class T>
struct Static_Mesh_Tree : AMR_Tree<T> {
    int mDepth;
    int mDimensions;
    double *mUpperBounds;

    Static_Mesh_Tree(int depth, int dimensions, double *upper_bounds)
       : AMR_Tree<T>(dimensions, upper_bounds) {
        // Set member variables                                         
        mDepth = depth;
        mDimensions = dimensions;
        mUpperBounds = upper_bounds;
        // Set depth of AMR Tree                                        
        set_depth(depth);
    }

    /// Checks all objects in adaptive mesh tree to see if any objects need   
    /// to be moved to new Nodes due to recent coordinate updates. Then, the  
    /// the tree will be restructured if existing Nodes have become to        
    /// populated or too unpopulated. Calls restructure_points() and          
    /// restructure_tree() functions in super (AMR_Tree)                      
    void updateTree(){
        // Restructure objects in tree                                  
        restructure_points();
        // Restructure tree if necessary                                
        restructure_tree();
    }
};
