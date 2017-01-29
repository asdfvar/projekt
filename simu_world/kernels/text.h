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

      void new_line( void );

      void clear( void );

      void populate( std::string );
      void populate( int );
      void populate( float );

      void display_contents( const float x,
                             const float y,
                             const float scale );

   private:

      std::string buffer[20];
      int row;
};

#endif
