#ifndef SHAPE_H
#define SHAPE_H

class shape
{

   public:

      shape( float center[3] );
      ~shape (void);

      float center[3];

};

class sphere : public shape
{

   public:

      sphere( float center[3],
              float radius );

      float radius;

};

#endif
