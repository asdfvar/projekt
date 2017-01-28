#ifndef TEXT_H
#define TEXT_H

#include <iostream>

class Text
{
   public:

      Text( void );

      void write_to_screen( std::string input,
                            const float x,
                            const float y,
                            const float scale );

      void clear( void );

      void populate( std::string input );

      void display_contents( const float x,
                             const float y,
                             const float scale );

   private:

      std::string buffer[20];
      int row;
};

#endif
