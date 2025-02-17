#pragma once
#include "Util.hpp"


namespace AMR
{

   /// A non-copyable and non-movable D-dimensional buffer                    
   ///   @tparam T - the type of contained data                               
   ///   @tparam D - the number of dimensions                                 
   template<CT::Data T, u8 D>
   struct Buffer {
      LANGULUS(TYPED) T;
      static_assert(D > 0, "D must be a positive integer");
      using Vu64  = TVector<u64, D>;
      using Vu64s = TVector<u64, (D > 1 ? D-1 : D)>;

      Vu64  mSize;
      Vu64s mStride;
      T*    mData;

   public:
       Buffer(const Vu64&);
       Buffer(const Buffer&) = delete;
       Buffer(Buffer&&) = delete;
      ~Buffer() { delete[] mData; }

      Buffer& operator = (Buffer&&) = delete;
      Buffer& operator = (const Buffer&) = delete;
   };


   /// A D-dimensional array                                                  
   /// interfaces a part of a buffer???
   ///   @tparam T - the type of contained data                               
   ///   @tparam D - the number of dimensions                                 
   template<CT::Data T, u8 D>
   struct Array {
      LANGULUS(TYPED) T;
      using Vu64 = TVector<u64, D>;
      using Vi64 = TVector<i64, D>;

      // The buffer we're interfacing                                   
      Ref<Buffer<T, D>> mBuffer;
      // Starting position into the buffer                              
      Vu64 mPosition;
      // Size of the interfaced region                                  
      Vu64 mSize;
      // Cumulative T offset in all dimensions for mData buffer         
      u64  mOffset;

      Array(const Array&) = default;
      Array(Array&&) = default;
      Array(const Ref<Buffer<T, D>>&, const Vu64& position, const Vu64& size);

      Array& operator = (const Array&) = default;
      Array& operator = (Array&&) = default;

      static auto createWithBuffer(const Vu64& size) -> Array;

      auto operator[](const Vu64& coords) const -> T const&;
      auto operator[](const Vu64& coords)       -> T&;

      /// @brief ??????
      struct Getter {
         Array&     mArray;
         const Vu64 mBase;

         template<class...XS>
         auto operator()(i64 x1, XS...xs) const -> const T& {
            return mArray[mBase + Vi64 {x1, xs...}];
         }

         template<class...XS>
         auto operator()(i64 x1, XS...xs) -> T& {
            return mArray[mBase + Vi64 {x1, xs...}];
         }
      };

      auto getter(const Vu64& base) -> Getter {
         return Getter {*this, base};
      }

      auto getter(const Vu64& base) const -> const Getter {
         return Getter {*this, base};
      }
   };

} // namespace AMR

#include "Buffer.inl"
