#ifndef TEXT_H
#define TEXT_H

#include <iostream>

class Text
{
   public:

      Text (void);

      void new_line (void);

      void populate     (std::string);
      void populate     (int);
      void populate     (float);
      void populate     (double);

      void display_contents (
            const float x,
            const float y,
            const float alpha,
            const float scale);

   private:

      std::string  buffer[200];
      int          row;

      void write_to_screen (
            std::string input,
            const float x,
            const float y,
            const float alpha,
            const float scale);

};

#endif
