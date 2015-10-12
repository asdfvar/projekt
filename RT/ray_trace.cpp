#include "ray_trace.h"
#include "linalg.h"
#include "sphere.h"
#include "fileio.h"
#include <iostream>
#include <cmath>
#include "ray.h"

#define PI 3.14159265358979323846264338327f

/*
** Constructor NAME: RayTrace
*/
RayTrace::RayTrace (float start_position_in[3],
                    float start_rotation[3], /* rotation about (x,y,z) */
                    float window_width,
                    float window_height,
                    float window_distance,
                    float dx,
                    float dy)
{ 

   start_rotation[0] *= PI / 180.0f;
   start_rotation[1] *= PI / 180.0f;
   start_rotation[2] *= PI / 180.0f;

   float rotation_x[9] = {1.0f, 0.0f,                     0.0f,
                          0.0f, cosf(start_rotation[0]), -sinf(start_rotation[0]),
                          0.0f, sinf(start_rotation[0]),  cosf(start_rotation[0])};

   float rotation_y[9] = {cosf(start_rotation[1]), 0.0f, sinf(start_rotation[1]),
                          0.0f,                    1.0f, 0.0f,
                         -sinf(start_rotation[1]), 0.0f, cosf(start_rotation[1])};

   float rotation_z[9] = {cosf(start_rotation[2]), -sinf(start_rotation[2]), 0.0f,
                          sinf(start_rotation[2]),  cosf(start_rotation[2]), 0.0f,
                          0.0f,                     0.0f,                    1.0f};

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

   float direction[3];

   /*
   ** Establish the grid of rays
   */

   grid = new Ray[nxy];

   /*
   ** Build the grid of rays
   */

   float xp, yp;
   float point[3];
   float temp1[3];
   float temp2[3];

   for (k = 0, i = 0; i < ny; i++)
   {
      for (j = 0; j < nx; j++, k++)
      {
         grid[k].set_position( position );

         xp = left_end   + j * dx;
         yp = bottom_end + i * dy;

         point[0] = window_distance;
         point[1] = xp;
         point[2] = yp;

         linalg::linear_transform<float> (point,
                                          rotation_x,
                                          temp1,
                                          3,
                                          3);

         linalg::linear_transform<float> (temp1,
                                          rotation_y,
                                          temp2,
                                          3,
                                          3);

         linalg::linear_transform<float> (temp2,
                                          rotation_z,
                                          direction,
                                          3,
                                          3);

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
   Sphere *sphere = new Sphere( position, radius);
   all_objects.insert( sphere );
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
                  Ray     ray,
                  Ray    *new_ray,
                  float  *distance,
                  float **reflection_table_x,
                  float **reflection_table_y,
                  int    *reflection_table_N)
{

   return all_objects.intersect (
                 ray,
                 new_ray,
                 distance,
                 reflection_table_x,
                 reflection_table_y,
                 reflection_table_N);

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

    fio::write_bin_int ("./result.r",
                        &nx,
                         false,
                         1);

    fio::write_bin_int ("./result.r",
                        &ny,
                         true,
                         1);

    fio::write_bin_float ("./result.r",
                           data,
                           true,
                           nxy);

    for (int k = 0; k < nxy; k++)
    {
       data[k] = grid[k].get_intensity(1);
    }

    fio::write_bin_int ("./result.g",
                        &nx,
                         false,
                         1);

    fio::write_bin_int ("./result.g",
                        &ny,
                         true,
                         1);

    fio::write_bin_float ("./result.g",
                           data,
                           true,
                           nxy);

    for (int k = 0; k < nxy; k++)
    {
       data[k] = grid[k].get_intensity(2);
    }

    fio::write_bin_int ("./result.b",
                        &nx,
                         false,
                         1);

    fio::write_bin_int ("./result.b",
                        &ny,
                         true,
                         1);

    fio::write_bin_float ("./result.b",
                           data,
                           true,
                           nxy);

    delete[] data;
}
