#ifndef USER_H
#define USER_H

#include "map.h"

class User
{

   public:

      User( void);

      void update( float dt);

      void move_forward( void);
      void move_backward( void);
      void move_upward( void);
      void move_downward( void);
      void move_right( void);
      void move_left( void);
      void stop( void);

      void draw_scene( Map *map);

   private:

      float position[3];
      float direction[3];
      float speed;
      float window_distance;
      float window_width;
      float window_height;
};

#endif
