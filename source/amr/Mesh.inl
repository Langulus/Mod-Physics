#pragma once
#include "Mesh.hpp"
#include "Control.hpp"
#include "Util.hpp"


namespace AMR
{

   /// Allocate all required buffers for a mesh                               
   ///   @param size - the dynamic size of the buffers                        
   ///   @return a tuple with all allocated buffers                           
   template<u8 D, u64 S, Grid...G>
   auto MeshConfig<D, S, G...>::createBuffers(const Vu64& size) -> Buffers::Tuple {
      return Buffers::GenerateData([&]<class T> {
         return T {}.New(size);
      });
   }

   /// Child node constructor                                                 
   /// Relies on externally allocated buffers                                 
   ///   @param parent - the parent node (from the upper level)               
   ///   @param buffers - the preallocated buffers                            
   ///   @param position - starting positions in buffers                      
   ///   @param index ??
   template<Config C>
   Node<C>::Node(Node* parent, const Buffers::Tuple& buffers, const Vu64& position, const Vu64& index)
      : children(NodeArray::createWithBuffer(2))
      , parent(parent)
      , data(mapTuple(buffers, [&]<class T>(const T& buffer) {
            return Array<TypeOf<TypeOf<T>>, Dimension>(buffer, position, C::BlockSize);
         }))
      , level(parent? parent->level + 1 : 0)
      , index(index)
      , adjacent(NodeArray::createWithBuffer(3)) {}

   /// Root node construction                                                 
   /// Allocates the first buffers itself                                     
   template<Config C>
   Node<C>::Node()
      : children(NodeArray::createWithBuffer(2))
      , data(mapTuple(C::createBuffers(C::BlockSize), [&]<class T>(const T& buffer) {
            return Array<TypeOf<TypeOf<T>>, Dimension>(buffer, 0, C::BlockSize);
         }))
      , adjacent(NodeArray::createWithBuffer(3)) {}

   /// Upsample grids in all dimensions                                       
   template<Config C> template<Grid G, Index64 I>
   void Node<C>::upsampleGrid() {
      LANGULUS_ASSUME(DevAssumes, not isLeaf, "Node is a leaf node");
      Loop<Dimension>(0, 2, [&](const auto& it1) {
         Loop<Dimension>(0, C::BlockSize / 2, [&](const auto& it2) {
            const auto src = it2 + C::BlockSize / 2 * it1;
            const auto dst = 2ul * it2;
            G::upsample(
               std::get<I>(data).getter(src),
               std::get<I>(children[it1]->data).getter(dst)
            );
         });
      });
   }

   template<Config C>
   void Node<C>::upsampleAll() {
      C::Grids::ForEachIndexed([&]<Grid G, auto INDEX>() {
         upsampleGrid<G, INDEX>();
      });
   }

   template<Config C> template<Grid G, Index64 I>
   void Node<C>::upsampleGridRange(const Vu64& fromSrc, const Vu64& toSrc, const Vu64& toDst, Node* child) {
      LANGULUS_ASSUME(DevAssumes, not isLeaf, "Node is a leaf node");
      Loop<Dimension>(fromSrc, toSrc, [&](const auto& it) {
         const auto dst = toDst + (it - fromSrc) * 2;
         G::upsample(
            std::get<I>(data).getter(it),
            std::get<I>(child->data).getter(dst)
         );
      });
   }

   template<Config C>
   void Node<C>::upsampleAllRange(const Vu64& fromSrc, const Vu64& toSrc, const Vu64& toDst, const Vu64& child) {
      auto childNode = children[child];
      C::Grids::ForEachIndexed([&]<Grid G, auto INDEX>() {
         upsampleGridRange<G, INDEX>(fromSrc, toSrc, toDst, childNode);
      });
   }

   template<Config C> template<Grid G, Index64 I>
   void Node<C>::downsampleGrid() {
      LANGULUS_ASSUME(DevAssumes, not isLeaf, "Node is a leaf node");
      Loop<Dimension>(0, 2, [&](const auto& it1) {
         Loop<Dimension>(0, C::BlockSize / 2, [&](const auto& it2) {
            const auto dst = it2 + C::BlockSize / 2 * it1;
            const auto src = 2 * it2;
            G::downsample(
               std::get<I>(children[it1]->data).getter(src),
               std::get<I>(data).getter(dst)
            );
         });
      });
   }

   template<Config C>
   void Node<C>::downsampleAll() {
      C::Grids::ForEachIndexed([&]<Grid G, auto INDEX>() {
         downsampleGrid<G, INDEX>();
      });
   }

   template<Config C>
   void Node<C>::updateAdjacency() {
      Loop<Dimension>(0, 3, [&](const auto& it) {
         adjacent[it] = nullptr;
      });

      adjacent[1] = this;

      if (parent) {
         sync = false;
         propagate = false;
         TVector<i64, 6> t {0, 1, 1, 1, 1, 2};

         Loop<Dimension>(0, 3, [&](const auto& it) {
            Node*& adj = adjacent[it];
            auto x = index * 3 + it;
            for (u8 i = 0; i < Dimension; ++i)
               x[i] = t[x[i]];

            adj = parent->adjacent[x];

            if (adj and not adj->isLeaf)
               adj = adj->children[(index + it) % 2];

            if (adj and (adj->isLeaf or isLeaf))
               sync = true;
         });

         if (parent->sync or parent->propagate)
            propagate = true;
      }

      if (not isLeaf) {
         Loop<Dimension>(0, 2, [&](const auto& it) {
            children[it]->updateAdjacency();
         });
      }
   }

   template<Config C>
   RefinePlan<C>::RefinePlan(Node<C>* node)
      : level(node->level)
      , size(1)
      , nodes(NodeArray::createWithBuffer(size)) {
      nodes[position] = node;
   }

   template<Config C>
   RefinePlan<C>::RefinePlan()
      : nodes(NodeArray::createWithBuffer(size)) {}

   template<Config C>
   void Node<C>::calculateRefinePlan() {
      if (isLeaf) {
         if (not (action & Action::Refine))
            return;

         refinePlan.emplace_back(new RefinePlan(this));
      }
      else {
         Loop<Dimension>(0, 2, [&](const auto& it) {
            auto child = children[it];
            for (auto rpit = child->refinePlan.begin();
               rpit != child->refinePlan.end();) {
               if ((*rpit)->propagate) {
                  (*rpit)->propagateUp(it, level);
                  refinePlan.push_back(std::move(*rpit));
                  rpit = child->refinePlan.erase(rpit);
               }
               else rpit++;
            }
         });

         bool changed;
         do {
            changed = false;
            for (size_t pi = 0; pi < refinePlan.size(); ++pi) {
               auto& plan1 = refinePlan[pi];
               for (size_t pj = pi + 1; pj < refinePlan.size(); ++pj) {
                  auto& plan2 = refinePlan[pj];
                  if (plan1->merge(plan2, level)) {
                     delete refinePlan[pj];
                     refinePlan.erase(refinePlan.begin() + pj);
                  }

                  changed = true;
                  pj--;
               }
            }
         } while (changed);
      }
   }

   template<Config C>
   void Node<C>::calculateRefinePlanRecursive() {
      if (not isLeaf) {
         Loop<Dimension>(0, 2, [&](const auto& it) {
            children[it]->calculateRefinePlanRecursive();
         });
      }

      calculateRefinePlan();
   }

   template<Config C>
   void Node<C>::split(const Buffers::Tuple& buffers, const Vu64& position) {
      LANGULUS_ASSUME(DevAssumes, isLeaf, "Node isn't a leaf node");

      Loop<Dimension>(0, 2, [&](const auto& it) {
         auto childPosition = position + it * C::BlockSize;
         children[it] = new Node(this, buffers, childPosition, it);
      });

      isLeaf = false;
      upsampleAll();
   }

   template<Config C>
   void Node<C>::merge() {
      LANGULUS_ASSUME(DevAssumes, not isLeaf, "Node is a leaf node");

      downsampleAll();

      Loop<Dimension>(0, 2, [&](const auto& it) {
         LANGULUS_ASSUME(DevAssumes, children[it]->isLeaf, "Child isn't is a leaf node");
         delete children[it];
         children[it] = nullptr;
      });

      isLeaf = true;
   }

   template<Config C>
   void executeRefinePlan(const RefinePlan<C>& plan) {
      auto size = plan.size * C::BlockSize * 2 + 2;
      auto buffers = C::createBuffers(size);
      Loop<C::Dimension>(0, plan.size, [&](const auto& it) {
         plan.nodes[it]->split(buffers, it);
      });
   }

   template<Config C>
   void Node<C>::restructure() {
      if (not isLeaf) {
         int action = Action::Coarsen;
         Loop<Dimension>(0, 2, [&](const auto& it) {
            action &= children[it]->action;
         });

         if (action & Action::Coarsen) {
            merge();
            return;
         }

         Loop<Dimension>(0, 2, [&](const auto& it) {
            action &= children[it]->action;
         });
      }

      for (size_t i = 0; i < refinePlan.size(); ++i)
         executeRefinePlan<C>(*refinePlan[i]);

      action = Action::None;
      refinePlan.clear();
   }

   template<Config C> template<Index64 I>
   void Node<C>::exchangeHaloGrid(Node* node1, Node* node2, const Vu64& fromSrc, const Vu64& toSrc, const Vu64& fromDst) {
      if (not node1 or not node2) {
         // TODO: boundary conditions
         return;
      }

      if (isInSameBuffer(node1, node2))
         return;

      Loop<Dimension>(fromSrc, toSrc, [&](auto& it) {
         std::get<I>(node1->data)[fromDst + it - fromSrc] = std::get<I>(node2->data)[it];
      });
   }

   template<Config C>
   bool Node<C>::isInSameBuffer(Node* node1, Node* node2) {
      return std::get<0>(node1->data).buffer == std::get<0>(node2->data).buffer;
   }

   template<Config C>
   void Node<C>::exchangeHaloAll(Node* node1, Node* node2, const Vu64& fromSrc, const Vu64& toSrc, const Vu64& fromDst) {
      C::Grids::ForEachIndex([&]<Grid G, auto INDEX>() {
         exchangeHaloGrid<INDEX>(node1, node2, fromSrc, toSrc, fromDst);
      });
   }

   template<Config C>
   void Node<C>::synchronize() {
      Loop<Dimension>(0, 3, [&](auto& it) {
         Vu64 fromSrc, toSrc, fromDst;

         for (u8 i = 0; i < Dimension; ++i) {
            fromSrc[i] = it[i] == 0 ? C::BlockSize : 1;
            toSrc  [i] = it[i] == 2 ? 2 : C::BlockSize + 1;
            fromDst[i] = it[i] == 0 ? 1 : C::BlockSize;
         }

         exchangeHaloAll(this, adjacent[it], fromSrc, toSrc, fromDst);
      });
   }

   template<Config C>
   void Node<C>::synchronizeRecursive() {
      if (sync)
         synchronize();

      if (not isLeaf) {
         Loop<Dimension>(0, 2, [&](auto& it) {
            children[it]->synchronize();
         });
      }
   }

   template<Config C>
   void Node<C>::applyKernel(auto&& func) {
      if (isLeaf) {
         action = Action::Coarsen;
         Loop<Dimension>(1, C::BlockSize, [&](auto& it) {
            func(DataView<C>(*this, it));
         });
      }
      else {
         Loop<Dimension>(0, 2, [&](auto& it) {
            children[it]->applyKernel(func);
         });
      }
   }

   template<Config C>
   void Node<C>::propagateUp() {
      if (not isLeaf) {
         Loop<Dimension>(0, 2, [&](auto& it) {
            children[it]->propagateUp();
         });
      }

      if (propagate)
         downsampleAll();
   }

   template<Config C>
   void Node<C>::propagateDown() {
      if (isLeaf)
         return;

      if (propagate or sync) {
         Loop<Dimension>(0, 3, [&](auto& it) {
            Vu64 fromSrc, toSrc, fromDst;

            for (u8 i = 0; i < Dimension; ++i) {
               fromSrc[i] = it[i] == 0 ? C::BlockSize + 1 : -1;
               toSrc  [i] = it[i] == 2 ?  1 : C::BlockSize + 2;
               fromDst[i] = it[i] == 0 ? -1 : C::BlockSize + 1;
               upsampleAllRange(fromSrc, toSrc, fromDst, 0);
            }
         });
      }
   }

   template<Config C>
   DataView<C>::DataView(Node<C>& node, Vu64 base)
      : node(node)
      , base(base) {}

   template<Config C>
   void RefinePlan<C>::propagateUp(const Vu64& index, u32 currentLevel) {
      // TODO: set propagate
      LANGULUS_ASSUME(DevAssumes, propagate, "Propagation should be true");
      u64 offset = 1 << (level - currentLevel - 1);
      position = index * offset;
   }

   template<Config C>
   bool RefinePlan<C>::merge(RefinePlan* other, u32 level) {
      if (level != other->level)
         return false;

      RefinePlan merged;
      merged.level = level;
      merged.propagate = false;

      int diffCoord = -1;
      for (u8 i = 0; i < Dimension; ++i) {
         if (position[i] == other->position[i] and size[i] == other->size[i]) {
            merged.position[i] = position[i];
            merged.size[i] = size[i];
         }
         else if (diffCoord != -1) {
            return false;
         }
         else if (position[i] + size[i] == other->position[i]) {
            merged.position[i] = position[i];
            merged.size[i] = size[i] + other->size[i];
            diffCoord = i;
         }
         else if (other->position[i] + other->size[i] == position[i]) {
            merged.position[i] = other->position[i];
            merged.size[i] = size[i] + other->size[i];
            diffCoord = i;
         }
         else return false;

         if (merged.position[i] + merged.size[i] == 1 << (merged.level - level) or merged.position[i] == 0)
            merged.propagate = true;
      }

      LANGULUS_ASSUME(DevAssumes, diffCoord != -1, "Invalid difference");

      auto nodesBuffer = Ref<Buffer<Node<C>*, Dimension>>::New(merged.size);
      Vu64 pos1 = 0;
      Vu64 pos2 = 0;
      for (u8 i = 0; i < Dimension; ++i) {
         if (i == diffCoord) {
            if (position[i] > other->position[i])
               pos1[i] = other->size[i];
            else
               pos2[i] = size[i];
         }
      }

      NodeArray array1(nodesBuffer, size, pos1);
      Loop<Dimension>(0, size, [&](auto& it) {
         array1[it] = nodes[it];
      });

      NodeArray array2(nodesBuffer, other->size, pos2);
      Loop<Dimension>(0, other->size, [&](auto& it) {
         array2[it] = other->nodes[it];
      });

      merged.nodes = NodeArray(nodesBuffer, merged.size, 0);
      *this = merged;
      return true;
   }

   template<Config C>
   Tree<C>::Tree(Mesh<C>* mesh, Vu64 selfPosition)
      : root(new Node<C>)
      , mesh(mesh)
      , selfPosition(selfPosition) {}

   template<Config C>
   void Tree<C>::restructure() {
      root->calculateRefinePlanRecursive();
      root->restructure();
      root->updateAdjacency();
   }

   template<Config C>
   void Tree<C>::synchronize() {
      root->propagateUp();
      root->synchronize();
   }

   template<Config C>
   void Tree<C>::applyKernel(auto&& func) {
      root->applyKernel(func);
   }

   /*template<Config C>
   Mesh<C>::Mesh(const Vu64& size)
      : trees(Array<Tree, Dimension>::createWithBuffer(size)) {
      Loop<Dimension>(0, trees.mSize, [&](auto& it) {
         new (&trees[it]) Tree;
      });

      updateStructure();
   }*/

   /*template<Config C>
   void Mesh<C>::updateStructure() {
      Loop<Dimension>(0, trees.mSize, [&](auto& it) {
         trees[it].restructure();
         trees[it].synchronize();
      });
   }

   template<Config C>
   void Mesh<C>::applyKernel(auto&& func) {
      Loop<Dimension>(0, trees.mSize, [&](auto& it) {
         trees[it].applyKernel(func);
      });
   }*/

} // namespace AMR