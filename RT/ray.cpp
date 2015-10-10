#include "ray.h"

/*
** Constructor NAME: Ray
*/
Ray::Ray( void )
{
   for (int k = 0; k < 3; k++) position[k]  = 0.0f;
   for (int k = 0; k < 3; k++) direction[k] = 0.0f;
   for (int k = 0; k < 3; k++) intensity[k] = 0.0f;

   valid = true;
}

/*
** Constructor NAME: Ray
*/
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

   valid = true;

}

/*
** Function NAME: get_position
*/
float Ray::get_position( int k )
{
   return position[k];
}

/*
** Function NAME: get_direction
*/
float Ray::get_direction( int k )
{
   return direction[k];
}

/*
** Function NAME: get_intensity
*/
float Ray::get_intensity( int k )
{
   return intensity[k];
}

/*
** Function NAME: set_position
*/
void Ray::set_position( float pos[3] )
{
   position[0] = pos[0];
   position[1] = pos[1];
   position[2] = pos[2];
}

/*
** Function NAME: is_valid
*/
bool Ray::is_valid( void )
{
   return valid;
}

/*
** Function NAME: set_invalid
*/
void Ray::set_invalid( void )
{
   valid = false;
}

/*
** Function NAME: set_direction
*/
void Ray::set_direction( float dir[3] )
{
   direction[0] = dir[0];
   direction[1] = dir[1];
   direction[2] = dir[2];
}

/*
** Function NAME: set_intensity
*/
void Ray::set_intensity( float Int[3] )
{
   intensity[0] = Int[0];
   intensity[1] = Int[1];
   intensity[2] = Int[2];
}

/*
** Function NAME: increment_intensity
*/
void Ray::increment_intensity( float Int[3] )
{
   intensity[0] += Int[0];
   intensity[1] += Int[1];
   intensity[2] += Int[2];
}
