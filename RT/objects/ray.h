#ifndef RAY_H
#define RAY_H

class Ray
{
   public:

      Ray (float position[3],
           float direction[3],
           float intensity);

   private:

      float position[3];
      float direction[3];
      float intensity;
};

#endif
