#include "ray_trace.h"

int main()
{

   float position[3] = { 0.0f, 0.0f, 0.0f };
   float look_direction[3] = { 0.0f, 0.0f, 0.0f };

   RayTrace RT(position,
               look_direction,
               1.0f,
               1.0f,
               1.0f,
               0.01f,
               0.01f);

   RT.run( 4 );

   return 0;
}
