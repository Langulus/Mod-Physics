#include "Buffer.hpp"

#define TPL() template<CT::Data T, u8 D>
#define TME() TPL() auto Array<T, D>


namespace AMR
{

   /// Construct a buffer                                                     
   /// Will allocate memory for D-dimensional buffer of a given size          
   ///   @param size - the size of the buffer along each dimension            
   TPL()
   Buffer<T, D>::Buffer(const Vu64& size)
      : mSize {size} {
      LANGULUS_ASSERT(mSize[0] != 0, Construct, "Bad buffer size");
      auto bufferSize = mSize[0];

      for (size_t i = 1; i < D; ++i) {
         LANGULUS_ASSERT(mSize[i] != 0, Construct, "Bad buffer size");
         mStride[i - 1] = bufferSize;
         bufferSize *= mSize[i];
      }

      mData = new T[bufferSize];
   }

   /// Construct a buffer interface array                                     
   ///   @param buffer - the buffer we're interfacing                         
   ///   @param position - the D-dimensional starting position of array       
   ///   @param size - the D-dimensional size of the array                    
   TPL()
   Array<T, D>::Array(const Ref<Buffer>& buffer, const Vu64& position, const Vu64& size)
      : mBuffer   {buffer}
      , mPosition {position}
      , mSize     {size}
   {
      // Calculate the 1D offset into mData inside the buffer           
      mOffset = mPosition[0];
      for (u8 i = 0; i < D - 1; ++i)
         mOffset += mSize[i] * mPosition[i + 1];
   }

   /// Create both a buffer and an array interface for it with single call    
   ///   @param size - the size of the buffer and array                       
   TME()::createWithBuffer(const Vu64& size) -> Array {
      return {Ref<Buffer>{}.New(size), 0, size};
   }

   /// Get the T at the given D-dimensional array coordinates                 
   ///   @param coords - the D-dimensional coordinates                        
   ///   @return a reference to the contained data                            
   TME()::operator[](const Vu64& coords) -> T& {
      // Get the absolute index in the mData array of the buffer        
      auto index = mOffset + coords[0];
      for (u8 i = 0; i < D - 1; ++i)
         index += coords[i + 1] * mBuffer->mStride[i];

      // Return the data                                                
      return mBuffer->mData[index];
   }

   TME()::operator[](const Vu64& coords) const -> T const& {
      return const_cast<Array*>(this)->operator[](coords);
   }

} // namespace AMR

#undef TPL
#undef TME