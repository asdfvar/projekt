#include "ray.h"

Ray::Ray( float position_in[3],
          float direction_in[3],
          float intensity_in[3])
{

   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];

   direction[0] = direction_in[0];
   direction[1] = direction_in[1];
   direction[2] = direction_in[2];

   intensity[0] = intensity_in[0];
   intensity[1] = intensity_in[1];
   intensity[2] = intensity_in[2];

}

float Ray::get_position( int k )
{
   return position[k];
}

float Ray::get_direction( int k )
{
   return direction[k];
}

float Ray::get_intensity( int k )
{
   return intensity[k];
}

void Ray::set_position( float pos[3] )
{
   position[0] = pos[0];
   position[1] = pos[1];
   position[2] = pos[2];
}

void Ray::set_direction( float dir[3] )
{
   direction[0] = dir[0];
   direction[1] = dir[1];
   direction[2] = dir[2];
}

void Ray::set_intensity( float Int[3] )
{
   intensity[0] = Int[0];
   intensity[1] = Int[1];
   intensity[2] = Int[2];
}
