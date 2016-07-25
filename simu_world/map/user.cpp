#include "user.h"

User::User( void)
{
   position[0] = 0.0f;
   position[1] = 0.0f;
   position[2] = 0.0f;

   direction[0] = 1.0f;
   direction[1] = 0.0f;
   direction[2] = 0.0f;

   speed = 0.0f;
}

void User::update( float dt)
{
   position[0] += direction[0] * speed * dt;
   position[1] += direction[1] * speed * dt;
   position[2] += direction[2] * speed * dt;
}

void User::move_forward( void)
{
   speed = 0.3f;
}

void User::stop( void)
{
   speed = 0.0f;
}