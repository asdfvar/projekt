#include "ray_trace.h"
#include "ray.h"

RayTrace::RayTrace (float start_position[3],
                    float start_look_direction[3],
                    float window_width,
                    float window_height,
                    float window_distance,
                    float dx,
                    float dy)
{

   int i,j,k;

   int nx = (int) (window_width  / dx);
   int ny = (int) (window_height / dy);
   int nxy = nx * ny;

   float left_end;

   if (nx % 2 == 0) left_end = ((float)(-nx / 2) + 0.5f) * dx;
   else             left_end =  (float)(-nx / 2) * dx;

   float bottom_end;

   if (nx % 2 == 0) bottom_end = ((float)(-ny / 2) + 0.5f) * dy;
   else             bottom_end =  (float)(-ny / 2) * dy;

   float look_theta, look_phi, look_gamma;

   grid = new Ray[nxy];

   float xp, yp;

   for (k = 0, i = 0; i < ny; i++)
   {
      for (j = 0; j < nx; j++, k++)
      {
         grid[k].set_position( start_position );

         xp = left_end   + j * dx;
         yp = bottom_end + i * dy;

//         grid[k].set_direction(
      }
   }

}

RayTrace::~RayTrace(void)
{
   delete[] grid;
}
