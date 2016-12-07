#include "opengl_interface.h"
#include "draw_scene.h"

Opengl_interface::Opengl_interface(void)
{
   first_frame             = true;
   time_manager            = new Time_manager( 1.0 / 60.0);
   int map_dim[3]          = {11, 11, 11};
   float map_pos[3]        = {0.0f, 0.0f, 0.0f};
   map                     = new Map( 0, map_dim, map_pos);
   rcube                   = 0.0f;
   mousePassiveTask        = true;
   idleTask                = false;
}

void Opengl_interface::keyboardDown( const char key)
{

   switch (key) {
      case 'u':
         user.move_upward();
         break;
      case 'n':
         user.move_downward();
         break;
      case 'w':
         user.move_forward();
         break;
      case 's':
         user.move_backward();
         break;
      case 'a':
         user.move_left();
         break;
      case 'd':
         user.move_right();
         break;
   }

}

void Opengl_interface::keyboardUp( const char key)
{

   switch (key) {
      case 'w':
         user.stop();
         break;
      case 's':
         user.stop();
         break;
   }

}

void Opengl_interface::mousePassive( int x, int y)
{

   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   if ( mousePassiveTask )
   {
      mousePassivePosition[0] = x;
      mousePassivePosition[1] = y;
      mousePassiveTask        = false;
      idleTask                = true;
   }

}

Opengl_interface::~Opengl_interface(void)
{
   delete time_manager;
   delete map;
}

void Opengl_interface::idle( void)
{

   /*
   ** wait for the prescribed time to pass
   */
   time_manager->wait_for_time();

   user.update( time_manager->get_time_step() );

   int window_center_x = glutGet(GLUT_WINDOW_WIDTH)  / 2;
   int window_center_y = glutGet(GLUT_WINDOW_HEIGHT) / 2;

   if (first_frame)
   {
      glutWarpPointer( window_center_x, window_center_y);
      mousePassivePosition[0] = window_center_x;
      mousePassivePosition[1] = window_center_y;
      first_frame             = false;
   }
   else if( idleTask )
   {
      int x_offset = mousePassivePosition[0] - window_center_x;
      int y_offset = window_center_y - mousePassivePosition[1];
   
      const float toggle = 0.003f;
      float cos_x = cosf( -(float)x_offset * toggle);
      float sin_x = sinf( -(float)x_offset * toggle);
      float cos_y = cosf(  (float)y_offset * toggle);
      float sin_y = sinf(  (float)y_offset * toggle);
   
      float direction[3];
      user.get_direction( direction);
   
      float temp[3];
      temp[0] = cos_x * direction[0] - sin_x * direction[1];
      temp[1] = sin_x * direction[0] + cos_x * direction[1];
   
      direction[0] = temp[0];
      direction[1] = temp[1];
   
      float norm = sqrtf( direction[0] * direction[0] +
                          direction[1] * direction[1] +
                          direction[2] * direction[2]);

      direction[0] /= norm;
      direction[1] /= norm;
      direction[2] /= norm;

      float norm_xy = sqrtf( direction[0] * direction[0] +
                             direction[1] * direction[1]);

      float rot_z[3][3];
      rot_z[0][0] = direction[1] / norm_xy; rot_z[0][1] = -direction[0] / norm_xy; rot_z[0][2] = 0.0f;
      rot_z[1][0] = direction[0] / norm_xy; rot_z[1][1] =  direction[1] / norm_xy; rot_z[1][2] = 0.0f;
      rot_z[2][0] = 0.0f;                   rot_z[2][1] =  0.0f;                   rot_z[2][2] = 1.0f;

      float rot_x[3][3];
      rot_x[0][0] = 1.0f; rot_x[0][1] =  0.0f; rot_x[0][2] =  0.0f;
      rot_x[1][0] = 0.0f; rot_x[1][1] = cos_y; rot_x[1][2] = -sin_y;
      rot_x[2][0] = 0.0f; rot_x[2][1] = sin_y; rot_x[2][2] =  cos_y;

      float rot_z_back[3][3];
      rot_z_back[0][0] = direction[1] / norm_xy; rot_z_back[0][1]
                       = direction[0] / norm_xy; rot_z_back[0][2]
                       = 0.0f;
      rot_z_back[1][0] = -direction[0] / norm_xy; rot_z_back[1][1]
                       = direction[1] / norm_xy; rot_z_back[1][2]
                       = 0.0f;
      rot_z_back[2][0] = 0.0f;
      rot_z_back[2][1] = 0.0f;
      rot_z_back[2][2] = 1.0f;

      temp[0] = direction[0] * rot_z[0][0] + direction[1] * rot_z[0][1] + direction[2] * rot_z[0][2];
      temp[1] = direction[0] * rot_z[1][0] + direction[1] * rot_z[1][1] + direction[2] * rot_z[1][2];
      temp[2] = direction[0] * rot_z[2][0] + direction[1] * rot_z[2][1] + direction[2] * rot_z[2][2];

      direction[0] = temp[0];
      direction[1] = temp[1];
      direction[2] = temp[2];

      temp[0] = direction[0] * rot_x[0][0] + direction[1] * rot_x[0][1] + direction[2] * rot_x[0][2];
      temp[1] = direction[0] * rot_x[1][0] + direction[1] * rot_x[1][1] + direction[2] * rot_x[1][2];
      temp[2] = direction[0] * rot_x[2][0] + direction[1] * rot_x[2][1] + direction[2] * rot_x[2][2];

      direction[0] = temp[0];
      direction[1] = temp[1];
      direction[2] = temp[2];

      temp[0] = direction[0] * rot_z_back[0][0] +
                direction[1] * rot_z_back[0][1] +
                direction[2] * rot_z_back[0][2];

      temp[1] = direction[0] * rot_z_back[1][0] +
                direction[1] * rot_z_back[1][1] +
                direction[2] * rot_z_back[1][2];

      temp[2] = direction[0] * rot_z_back[2][0] +
                direction[1] * rot_z_back[2][1] +
                direction[2] * rot_z_back[2][2];

      direction[0] = temp[0];
      direction[1] = temp[1];
      direction[2] = temp[2];

      norm = sqrtf( direction[0] * direction[0] +
                    direction[1] * direction[1] +
                    direction[2] * direction[2]);

      direction[0] /= norm;
      direction[1] /= norm;
      direction[2] /= norm;

      user.set_direction( direction);
   
      glutWarpPointer( window_center_x, window_center_y);

      mousePassiveTask = true;
      idleTask         = false;
   }

}

void Opengl_interface::display(void)
{

   float mat_specular[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };
   float mat_shininess[1] = { 50.0f };
   float light_ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
   float light_diffuse[]  = { 0.8, 0.8, 0.8, 1.0 };
   float light_specular[] = { 0.8, 0.8, 0.8, 1.0 };
   float light_position[] = { 0.0, 0.0, 1.0, 0.0 };
   
   glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv( GL_LIGHT0, GL_POSITION, light_position);
   
   glEnable( GL_LIGHT0);
   glEnable( GL_LESS);
   glEnable( GL_DEPTH_TEST);
   glEnable( GL_LIGHTING);
   glEnable( GL_COLOR_MATERIAL);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   draw_scene( &user, map);

}
