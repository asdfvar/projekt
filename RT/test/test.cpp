#include "ray_trace.h"
#include <iostream>

int main()
{

   float position[3] = { 0.0f, 0.0f, 0.0f };
   float look_direction[3] = { 1.0f, 0.0f, 0.0f };

   RayTrace RT(position,
               look_direction,
               1.0f,
               1.0f,
               1.0f,
               0.1f,
               0.1f);

   position[0] = 3.0f;
   position[1] = 0.0f;
   position[2] = 0.0f;

   RT.insert_sphere ( position, 1.0f );

   RT.run( 1 );

   return 0;
}
