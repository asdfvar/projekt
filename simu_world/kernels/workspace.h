#include <memory>
#include <iostream>

class Workspace
{
   public:
      Workspace( size_t total_bytes )
      {
         std::allocator<char> alloc;
         buffer = alloc.allocate( total_bytes );
         buf_ptr = buffer;
      }

      ~Workspace( void ) { /* NULL */ }

      /*
      ** function name: reserve from: Workspace
      */
      template<class Type>
         Type* reserve( size_t size )
         {
            char* buf_ptr_start = buf_ptr;
            buf_ptr += size * sizeof( Type );
            return (Type*)buf_ptr_start;
         }

      /*
      ** function name: finalize from: Workspace
      */
      void finalize( void )
      {
         std::cout << "finalize workspace buffer" << std::endl;
         delete[] buffer;
      }

   private:
      char* buffer;
      char* buf_ptr;
};
