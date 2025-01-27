#pragma once
#include "Point.h"
#include "../amr/Util.hpp"


/// Each tree node represents a volume of the system which is determined by   
/// the respective upper and lower local boundaries it is created with        
template<class T, CT::Dimension...D>
class Node {
   static constexpr Count Dimensions   = sizeof...(D);
   static constexpr auto  BranchFactor = AMR::branch_factor(Dimensions);
   static_assert(Dimensions > 0, "Dimensions should be greater than zero");
   using Vector = TVector<Real, Dimensions>;

   // Elements held by this node                                        
   TMany<T*> mContents;
   // Pointer to the parent Node                                        
   Node*  mParent;
   // Points to the lower and upper bounds arrays                       
   Vector mLocalLower;
   Vector mLocalUpper;

public:
   // Max number of points allowed to be held by a Node before split    
   static constexpr Count MAX_POINTS_ALLOWED = 8;
   // Min number of points allowed to be held by a Node before merge    
   static constexpr size_t MIN_POINTS_ALLOWED = 3;

   // Pointer to the array of children Nodes                            
   TMany<Node> mChildren;

   ///   @param parent - pointer to the parent Node object                    
   ///   @param dimensions - number of dimensions of the system (and length   
   ///      of the "upper" and "lower"  parameter arrays)                     
   ///   @param lower - local lower boundary given in an array of length      
   ///      "dimensions"                                                      
   ///   @param upper - local upper boundary given in an array of length      
   ///      "dimensions"                                                      
   Node(Node* parent, const Vector& lower, const Vector& upper)
      : mParent     {parent}
      , mLocalLower {lower}
      , mLocalUpper {upper} {
      // Reserve early                                                  
      mChildren.Allocate(BranchFactor);
   }

   Node() = delete;

   /// Recursively inserts a single pointer of a T (template) object into the 
   /// proper Node based off of local boundaries. This function either accepts
   /// the pointer into its array of T, or it will determine the correct child
   /// to pass this pointer to and will call this function again recursively  
   /// on its children until the lowest level Node is found with the correct  
   /// boundaries.                                                            
   ///   @param T_ptr - pointer to a template T object that will be attempted 
   ///      to be inserted into this Node.                                    
   void insert(T* T_ptr) {
      if (not mChildren) {
         // There are no child nodes, so T should be held in this one   
         add(T_ptr);
         return;
      }

      // Determine the child that this point should belong to           
      int child_num = determine_child_number(T_ptr, 0, BranchFactor - 1);
      mChildren[child_num].insert(T_ptr);
   }

   /// Insert an array of pointers to objects into the AMR_Tree object. This  
   /// function calls the AMR_Tree::insert() function for each point          
   ///   @param T_ptrs - pointer to the first element of a Point pointer array
   void insert(TMany<T*> T_ptrs) {
      for (auto i : T_ptrs)
         insert(i);
   }

   /// Creates 2^dimensions children Nodes to subdivide this Node's volume    
   void generate_children() {
      // Initialize new children nodes                                  
      for (int i = 0; i < BranchFactor; i++) {
         // Initialize lower and upper boundaries for the child Node    
         Vector child_lower;
         Vector child_upper;

         // Determine the lower and upper boundaries of the child Node  
         calculate_child_boundaries(child_lower, child_upper, i, 0, BranchFactor - 1);

         // Create new child Node                                       
         mChildren << Node<T>(this, child_lower, child_upper);
      }
   }

   /// Recursively passes through all Nodes to check if any Nodes should be   
   /// further subdivided or merged. This function is typically called after  
   /// the Node::restructure_points() function, which is typically called     
   /// after the points contained in this AMR Tree have had their position    
   /// updated.                                                               
   void restructure_tree() {
      if (not mChildren) {
         // No children nodes, this Node is the lowest on its chain     
         bool subdivide = check_overpopulation_criteria();
         if (subdivide) {
            // This Node will be subdivided                             
            generate_children();

            // Insert points by calling insert function on this node,   
            // which will properly pass the contents to the newly       
            // generated children                                       
            insert(mContents);
            mContents.Clear();

            // Move to children nodes                                   
            for (auto& child : mChildren)
               child.restructure_tree();
         }

         return;
      }

      // There are children, check if a merge should be performed       
      // Check if there are any grandchildren                           
      bool grandchildren = false;
      for (auto& child : mChildren) {
         if (child.mChildren) {
            grandchildren = true;
            break;
         }
      }

      // Check if a merge is allowed                                    
      if (not grandchildren) {
         // Check if merge should happen                                
         bool merge = check_underpopulation_criteria();
         if (merge) {
            // Depopulate children nodes                                
            for (auto& child : mChildren)
               child.depopulate_node();
            mChildren.clear();
         }
      }
      else {
         // Merge not allowed, recursively call the restructure         
         // function on all of this Node's children                     
         for (auto& child : mChildren)
            child.restructure_tree();
      }
   }

   /// Recursively generate children nodes until depth is achieved            
   ///   @param depth - depth of AMR tree (number of levels)                  
   void set_depth(int depth) {
      // Check for depth achieved                                       
      if (depth == 0)
         return;

      // Generate children                                              
      generate_children();

      // Decrease depth counter                                         
      depth -= 1;

      // Recursively call set_depth(depth-1) for all children           
      for (auto& child : mChildren)
         child.set_depth(depth);
   }

   /// Recursively passes through all Nodes' children to check if any points  
   /// should be redistributed into different local Nodes. This function is   
   /// typically called after the positions of the Points have been updated,  
   /// and should therefore be updated in the AMR Tree as well. This function 
   /// recursively moves through all children first to reach the bottom, where
   /// it checks if there are any points that do no belong in the current Node
   /// they are stored in. These out-of-bounds Points will be passed upwards  
   /// until they are held by a Node that does contain the Point. At that     
   /// time, this function will call the insert_point function to insert it   
   /// into that Node, or recursively into its children, as handled by the    
   /// insert_point function                                                  
   void restructure_points() {
      // Move to children Nodes                                         
      for (auto& child : mChildren)
         child.restructure_points();

      // Check if this Node has points to check the boundaries of       
      int offset = 0;
      for (int i = 0; i < local_points.size(); i++) {
         // Check if each point belongs in this Node                    
         Point* point = local_points[i + offset];
         bool   local = check_point_boundaries(point);

         if (not local) {
            // This T does not belong in this Node, it will be          
            // percolated up to its parent                              
            percolate_to_parent(point);
            // Remove this T from this Node                             
            local_points.erase(local_points.begin() + i + offset);
            // Increase offset amount                                   
            offset += -1;
         }
      }

      // Now that Points have been percolated up into the correctly     
      // boundaried Nodes, we must percolate them down into the lowest  
      // Nodes that each T belongs in. (Note: if this Node's            
      // local_points vector does not contain any Points, then the      
      // Node::insert() function will simply return                     
      if (not children.empty())
         insert(local_points);
   }

   int get_depth() {
      if (children.size() == 0)
         return 1;
      else
         return 1 + children.at(0).get_depth();
   }

private:
   /// Recursively determines the integer value (in array of child Node       
   /// pointers) that the given point belongs in                              
   ///   @param point_ptr Point pointer to the Point that will be passed to a 
   ///      child Node                                                        
   ///   @param dim Number of dimensions to work with in the current iteration
   ///      (should be number of dimensions of the system when this function  
   ///      is called)                                                        
   ///   @return the integer value of the index of the Point pointer array of children
   int determine_child_number(T* T_ptr, int min, int max) {
      // Determine number of children in the range min to max (1-indexed)
      int num_of_children = max - min + 1;
      // Determine the current dimension number to divide axes          
      // (total_dimensions - num_of_checked_dimensions)                 
      int dim = dimensions - (int)log2(num_of_children);
      // Check recursive stop condition (one index remaining)           
      if (num_of_children == 1)
         return min;

      // Calculate dividing value                                       
      double middle_value = (upper_local[dim] - lower_local[dim]) / 2 + (lower_local[dim]);

      // Determine if the T falls within the lower or upper half of the 
      // current node                                                   
      if (T_ptr->getCoordinates().at(dim) <= middle_value) {
         // T belongs on lower side of this axis                        
         return determine_child_number(T_ptr, min, num_of_children / 2 + min - 1);
      }
      else {
         // T belongs on the upper side of this axis                    
         return determine_child_number(T_ptr, num_of_children / 2 + min, max);
      }
   }

   /// Add point to this Node's Point pointer array                           
   ///   @param T_ptr - point to be added to this Node's Point pointer array  
   void add(T* T_ptr) {
      local_points.push_back(T_ptr);
   }

   /// Recursively determines the boundaries of the child Node that the given 
   /// child Node index belongs in. The children Nodes are recursively divided
   /// in half, where the lower half belongs on the lower half of this Node's 
   /// local boundaries. For example, there are 2^dimensions children nodes.  
   /// In a 3 dimensional system, there are 8 nodes. The (0-indexed) 0-3 nodes
   /// belong on the lower half of this Node's x boundaries, while the 4-7    
   /// nodes belong on the upper half. This function will recursively call    
   /// itself again on the lower or upper half and determine if the child node
   /// belongs on the lower (0-1, or 4-5) half or the upper (2-3, or 6-7) half
   /// for the y dimension. Lastly, it will call itself one more time to      
   /// determine if the z is in the upper or lower half of this boundary.     
   /// So the result is a determination of if each axis is in the upper or    
   /// lower half of this Node's boundary for each dimension.                 
   ///   @param child_lower - pointer to the array of the child's lower       
   ///      boundaries to be determined by this function                      
   ///   @param child_upper - pointer to the array of the child's upper       
   ///      boundaries to be determined by this function                      
   ///   @param child_index - index of the child Node (from 0 to              
   ///      (2^dimensions)-1)                                                 
   ///   @param min - minimum index of the range to search of children        
   ///      indexes (from 0 to (2^dimensions)-1). Value of the first call to  
   ///      this function should be 0                                         
   ///   @param max - maximum index of the range to search of children        
   ///      indexes (from 0 to (2^dimensions)-1). Value of the first call to  
   ///      this function should be (2^dimensions)-1                          
   void calculate_child_boundaries(double* child_lower, double* child_upper, int child_index, int min, int max) {
      // Determine number of children in the range min to max (1-indexed)
      int num_of_children = max - min + 1;
      // Determine the current dimension number to divide axes (total_dimensions - num_of_checked_dimensions).
      int dim = dimensions - (int)log2(num_of_children);
      // Check recursive stop condition (one index remaining)           
      if (num_of_children == 1)
         return;

      // Calculate dividing value                                       
      double middle_value = (upper_local[dim] - lower_local[dim]) / 2.0 + (lower_local[dim]);

      // Determine if the T falls within the lower or upper half of the 
      // current node                                                   
      if (child_index < min + num_of_children / 2) {
         // This dimension's boundary belongs on lower side of the      
         // middle of this Node's boundaries                            
         child_lower[dim] = lower_local[dim];
         child_upper[dim] = middle_value;
         // Determine next dimension values                             
         return calculate_child_boundaries(child_lower, child_upper, child_index, min, num_of_children / 2 + min - 1);
      }
      else {
         // This dimension's boundary belongs on upper side of the      
         // middle of this Node's boundaries                            
         child_lower[dim] = middle_value;
         child_upper[dim] = upper_local[dim];
         // Determine next dimension values                             
         return calculate_child_boundaries(child_lower, child_upper, child_index, num_of_children / 2 + min, max);
      }
   }

   /// Pass all Point pointers to the parents Node                            
   void depopulate_node() {
      LANGULUS_ASSUME(DevAssumes, parent, "Shouldn't depopulate the root node");
      for (auto& p : local_points)
         parent->add(p);
   }

   /// Check if the total number of points in the children Nodes are greater  
   /// than MIN_POINTS_ALLOWED                                                
   ///   @return true if there are less points than the minimum, or false if  
   ///      there are more than the minimum                                   
   bool check_underpopulation_criteria() {
      // Count the number of points in the children Node                
      int count = 0;
      for (int i = 0; i < (int)pow(2, dimensions); i++)
         count += children.at(i).local_points.size();

      // Check if the Node's children are underpopulated                
      if (count < MIN_POINTS_ALLOWED) {
         // Children Nodes are underpopulated and should be merged back 
         // into the parent Node                                        
         return true;
      }
      else {
         // Children Nodes are sufficiently populated and should not be 
         // merged back into the parent Node                            
         return false;
      }
   }

   /// Check if the number of Points this Node contains is over the           
   /// MAX_POINTS_ALLOWED value and should be subdivided                      
   ///   @return - true if this Node is overpopulated, or false if it is not  
   bool check_overpopulation_criteria() {
      // Check if there are more points in this Node than the           
      // MAX_POINTS_ALLOWED value                                       
      return local_points.size() > MAX_POINTS_ALLOWED;
   }

   /// Given a Point pointer, this function will check if the Point's         
   /// coordinates are within this Node's boundaries                          
   ///   @param T_ptr - point pointer to be check if it is within the local   
   ///      Node's boundaries                                                 
   ///   @return true if the Point is within this Node's boundaries           
   bool check_point_boundaries(T* T_ptr) {
      // Get T's coordinates                                            
      std::vector<double> coords = T_ptr->getCoordinates();

      // Check if point is within local boundaries of this Node         
      for (int dim = 0; dim < dimensions; dim++) {
         if ((coords[dim] < lower_local[dim]) or (coords[dim] > upper_local[dim])) {
            // T is not within local boundaries                         
            return false;
         }
      }
      return true;
   }

   /// Inserts Point into the Parent Node                                     
   ///   @param T_ptr - pointer pointer to be added to the parent Node        
   void percolate_to_parent(T* T_ptr) {
      LANGULUS_ASSUME(DevAssumes, parent, "Shouldn't percolate in root node");
      parent->add(T_ptr);
   }
};