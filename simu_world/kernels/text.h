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
      void done( void );

      void display_contents( const float x,
                             const float y,
                             const float scale );

   private:

      std::string *edit_buffer;
      std::string *disp_buffer;
      std::string  buffer_1[20];
      std::string  buffer_2[20];
      int row;
      bool edit_complete;
      bool display_complete;
};

#endif
