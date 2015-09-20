#include "ray_trace.h"
#include "sphere.h"
#include "fileio.h"
#include <iostream>
#include <cmath>
#include "ray.h"

/*
** Constructor NAME: RayTrace
*/
RayTrace::RayTrace (float start_position_in[3],
                    float start_look_direction[3],
                    float window_width,
                    float window_height,
                    float window_distance,
                    float dx,
                    float dy)
{ 
   position[0] = start_position_in[0];
   position[1] = start_position_in[1];
   position[2] = start_position_in[2];

   int i,j,k;

   nx = (int) (window_width  / dx);
   ny = (int) (window_height / dy);
   int nxy = nx * ny;

   float left_end;

   if (nx % 2 == 0) left_end = ((float)(-nx / 2) + 0.5f) * dx;
   else             left_end =  (float)(-nx / 2) * dx;

   float bottom_end;

   if (nx % 2 == 0) bottom_end = ((float)(-ny / 2) + 0.5f) * dy;
   else             bottom_end =  (float)(-ny / 2) * dy;

   float look_theta, look_phi;
   float ray_theta, ray_phi;
   float absolute_theta, absolute_phi;
   float direction[3];

   look_theta = atan2( start_look_direction[1], start_look_direction[0] );
   look_phi   = atan2( start_look_direction[2], start_look_direction[0] );

   /*
   ** Establish the grid of rays
   */

   grid = new Ray[nxy];

   /*
   ** Build the grid of rays
   */

   float xp, yp;

   for (k = 0, i = 0; i < ny; i++)
   {
      for (j = 0; j < nx; j++, k++)
      {
         grid[k].set_position( position );

         xp = left_end   + j * dx;
         yp = bottom_end + i * dy;

         ray_phi   = atan2( yp, window_distance );
         ray_theta = atan2( xp, window_distance );

         absolute_theta = look_theta + ray_theta;
         absolute_phi   = look_phi   + ray_phi;

         direction[0] = cos( absolute_theta );
         direction[1] = sin( absolute_theta );
         direction[2] = sin( absolute_phi   );

         grid[k].set_direction( direction );
      }
   }

}

/*
** Destructor NAME: RayTrace
*/
RayTrace::~RayTrace(void)
{
   delete[] grid;
}

/*
** Function NAME: insert_sphere
*/
void RayTrace::insert_sphere (
       float position[3],
       float radius)
{
   all_objects.insert( Sphere( position, radius) );
}

/*
** Function NAME: insert_light
*/
void RayTrace::insert_light (
       float position[3],
       float intensity[3])
{
   lights.push_back( Light_source( position, intensity ));
}

/*
** Function NAME: get_nxy
*/
int RayTrace::get_nxy ( void )
{
   return nx*ny;
}

/*
** Function NAME: intersect_objects
*/
bool RayTrace::intersect_objects (
                  Ray    ray,
                  Ray   *new_ray,
                  float *distance)
{

   return all_objects.intersect (
                 ray,
                 new_ray,
                 distance);

}

/*
** Function NAME: print_grid
*/
void RayTrace::print_grid ( void )
{

   int i,j,k;

   for (k = 0, i = 0; i < ny; i++)
   {

      std::cout << std::endl;

      for (j = 0; j < nx; j++, k++)
      {
         std::cout << grid[k].get_intensity(0) << ", ";
      }
   }

   std::cout << std::endl;
}

/*
** Function NAME: write_grid
*/
void RayTrace::write_grid ( void )
{

    int nxy = nx * ny;

    float *data = new float[nxy];

    for (int k = 0; k < nxy; k++)
    {
       data[k] = grid[k].get_intensity(0);
    }

    fio::write_bin_float ("./result.r",
                           data,
                           false,
                           nxy);

    delete[] data;
}
