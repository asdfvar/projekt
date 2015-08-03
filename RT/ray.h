#ifndef RAY_H
#define RAY_H

class Ray
{
   public:

      Ray (void);

      Ray (float position[3],
           float direction[3],
           float intensity[3]);

   /*
   ** Position
   */

      float get_position( int );

      void set_position( float pos[3] );

   /*
   ** Direction
   */

      float get_direction( int );

      void set_direction( float dir[3] );

   /*
   ** Intensity
   **  [Red, Green, Blue]
   */

      float get_intensity( int );

      void set_intensity( float Int[3] );

   private:

      float position[3];
      float direction[3];
      float intensity[3];
};

#endif
