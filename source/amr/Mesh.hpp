#pragma once
#include "Buffer.hpp"
#include "Control.hpp"
#include "Util.hpp"
#include <vector>


namespace AMR
{

   using Index32 = u32;
   using Index64 = u64;

   enum Action {None = 0, Refine, Coarsen};

   template<class T>
   concept Config = T::CTTI_MeshConfigTag;
   template<class T>
   concept Grid = T::CTTI_GridConfigTag;

   template<u8 D, u64 S, Grid...G>
   struct MeshConfig;
   template<class T>
   struct GridConfig;

   template<Config> struct MeshHelper;
   template<Config> struct Mesh;
   template<Config> struct RefinePlan;
   template<Config> struct Node;
   template<Config> struct DataView;
   template<Config> struct Tree;


   /// Mesh configuration                                                     
   ///   @tparam D - number of dimensions                                     
   ///   @tparam S - size of block ???
   ///   @tparam G... - used grids configurations                             
   template<u8 D, u64 S, Grid...G>
   struct MeshConfig {
      static constexpr bool CTTI_MeshConfigTag = true;
      static constexpr u8  Dimension = D;
      static constexpr u64 BlockSize = S;

      using Vu64    = TVector<u64, Dimension>;
      using Vi64    = TVector<i64, Dimension>;
      using Grids   = Types<G...>;
      using Datas   = LangulusTypegen(Grids, ([]<class T>{ return TypeOf<T>{}; }));
      using Arrays  = LangulusTypegen(Datas, ([]<class T>{ return Types<Array<T, D>> {}; }));
      using Buffers = LangulusTypegen(Datas, ([]<class T>{ return Ref<Buffer<T, D>> {}; }));

      static auto createBuffers(const Vu64& size) -> Buffers::Tuple;
   };


   /// Grid configuration                                                     
   /// Used as base to classes that implement upsample/downsample             
   ///   @tparam T - type of contained data?                                  
   template<class T>
   struct GridConfig {
      static constexpr bool CTTI_GridConfigTag = true;
      LANGULUS(TYPED) T;

      static void upsample(auto src, auto dst) {
         static_assert(false, "You have to implement this");
      }
      static void downsample(auto src, auto dst) {
         static_assert(false, "You have to implement this");
      }
   };


   /// A node                                                                 
   ///   @tparam C - the mesh configuration                                   
   template<Config C>
   struct Node {
      static constexpr auto Dimension = C::Dimension;
      using Arrays     = typename C::Arrays;
      using Buffers    = typename C::Buffers;
      using Vu64       = typename C::Vu64;
      using NodeArray  = Array<Node*, Dimension>;

      bool isLeaf = true;
      NodeArray children;
      Node* parent = nullptr;
      Arrays::Tuple data;
      Action action = Action::None;
      u32 level = 0;
      Vu64 index = 0;
      std::vector<RefinePlan<C>*> refinePlan;
      NodeArray adjacent;
      bool sync = false;
      bool propagate = false;

   public:
      Node(const Node&) = delete;
      Node(Node&&) = delete;

      Node& operator = (const Node&) = delete;
      Node& operator = (Node&&) = delete;

      Node(Node*, const Buffers::Tuple&, const Vu64& position, const Vu64& indices);
      Node();

      void split(const Buffers::Tuple&, const Vu64& position);
      void merge();

      template<Grid, Index64>
      void upsampleGrid();
      void upsampleAll();

      template<Grid, Index64>
      void downsampleGrid();
      void downsampleAll();

      template<Grid, Index64>
      void upsampleGridRange(const Vu64& fromSrc, const Vu64& toSrc, const Vu64& toDst, Node* child);
      void upsampleAllRange (const Vu64& fromSrc, const Vu64& toSrc, const Vu64& toDst, const Vu64& child);

      void updateAdjacency();
      void calculateRefinePlan();
      void calculateRefinePlanRecursive();

      void restructure();

      void propagateUp();
      void propagateDown();

      static bool isInSameBuffer  (Node* node1, Node* node2);
      template<Index64>
      static void exchangeHaloGrid(Node* node1, Node* node2, const Vu64& fromSrc, const Vu64& toSrc, const Vu64& fromDst);
      static void exchangeHaloAll (Node* node1, Node* node2, const Vu64& fromSrc, const Vu64& toSrc, const Vu64& fromDst);
      void synchronize();
      void synchronizeRecursive();
      void applyKernel(auto&&);
   };


   ///                                                                        
   template<Config C>
   struct DataView {
      static constexpr auto Dimension = C::Dimension;
      using Vi64 = typename C::Vi64;
      using Vu64 = typename C::Vu64;

      Node<C>& node;
      Vu64  base;
      bool  refine = false;
      bool  derefine = false;

   public:
      DataView(Node<C>&, Vu64 base);

      ~DataView() {
         if (refine)
            node.action = Action::Refine;
         else if (not derefine and node.action == Action::Coarsen)
            node.action = Action::None;
      }

      template<Index32 I, class...XS>
      auto get(i64 x1, XS...xs) -> typename C::Datas::template At<I>& {
         return std::get<I>(node.data)[base + Vi64 {x1, xs...}];
      }
   };
   

   ///                                                                        
   template<Config C>
   struct RefinePlan {
      static constexpr auto Dimension = C::Dimension;
      using Vu64 = typename C::Vu64;
      using NodeArray = Array<Node<C>*, Dimension>;

      u32  level = 0;
      Vu64 position = 0;
      Vu64 size = 0;
      NodeArray nodes;
      bool propagate = true;

   public:
      RefinePlan(Node<C>*);
      RefinePlan();

      void propagateUp(const Vu64& index, u32 currentLevel);
      bool merge(RefinePlan*, u32 level);
   };













   template<Config C>
   struct Tree {
      static constexpr auto Dimension = C::Dimension;
      using Vu64 = typename C::Vu64;

      Mesh<C>* mesh;
      Vu64  selfPosition;
      Node<C>* root;

   public:
      Tree(const Tree&) = delete;
      Tree(Tree&&) = delete;
      Tree(Mesh<C>*, Vu64 selfPosition);

      Tree& operator = (const Tree&) = delete;
      Tree& operator = (Tree&&) = delete;

      void restructure();
      void synchronize();
      void applyKernel(auto&&);
   };

   template<Config C>
   struct Mesh {
      static constexpr auto Dimension = C::Dimension;
      using Vu64 = typename C::Vu64;

      //Array<Tree, Dimension> trees; TODO missing constructor

   public:
      Mesh(const Vu64& size);

      //void updateStructure();
      //void applyKernel(auto&&);
   };

} // namespace AMR