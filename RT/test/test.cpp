#include "ray_trace.h"
#include <iostream>

int main()
{

   float position[3] = { 0.0f, 0.0f, 0.0f };
   float look_direction[3] = { 1.0f, 0.0f, 0.0f };
   float intensity[3];

   float dx = 0.001f;
   float dy = 0.001f;

   RayTrace RT(position,
               look_direction,
               1.0f,
               1.0f,
               1.0f,
               dx,
               dy);

   position[0] = -1.0f;
   position[1] = 1.0f;
   position[2] = 1.0f;

   intensity[0] = 1.0f;
   intensity[0] = 1.0f;
   intensity[0] = 1.0f;

   RT.insert_light ( position, intensity );

   position[0] = 3.0f;
   position[1] = 0.0f;
   position[2] = 0.0f;

   RT.insert_sphere ( position, 1.0f );

   RT.run( 6 );

//   RT.print_grid ();

   RT.write_grid ();

   return 0;
}
