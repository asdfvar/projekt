#include "hud.h"
#include "opengl_interface.h"

namespace hud
{

  void display( void )
  {

     /*
     ** Draw reticle
     */
     const float width_height = 0.05f;
     float       half_width_height = 0.5f * width_height;

     ogl::normal( 0.0f, 0.0f, 1.0f );

     /*
     ** Black interior lines
     */
     ogl::color( 0.0f, 0.0f, 0.0f );
     ogl::line_width( 2.5f );
     ogl::begin_lines();
       ogl::vertex( -half_width_height + 0.005f, 0.0f, 0.0f );
       ogl::vertex( half_width_height - 0.005f, 0.0f, 0.0f );
       ogl::begin_lines();
       ogl::vertex( 0.0f, -half_width_height + 0.005f, 0.0f );
       ogl::vertex( 0.0f, half_width_height - 0.005f, 0.0f );
     ogl::end();

     /*
     ** White-thicker exterior lines
     */
     ogl::color( 1.0f, 1.0f, 1.0f );
     ogl::line_width( 3.5f );
     ogl::begin_lines();
       ogl::vertex( -half_width_height, 0.0f, 0.0f );
       ogl::vertex( half_width_height, 0.0f, 0.0f );
       ogl::begin_lines();
       ogl::vertex( 0.0f, -half_width_height, 0.0f );
       ogl::vertex( 0.0f, half_width_height, 0.0f );
     ogl::end();


  }

}
