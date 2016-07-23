#ifndef OPENGL_INTERFACE
#define OPENGL_INTERFACE

class Opengl_interface
{

   public:

      Opengl_interface(void);
      ~Opengl_interface(void);

      /*
      ** Display to screen
      */
      void display(void);

      /*
      ** Main function kernel
      */
      void idle(void);

      /*
      ** Mouse motion input
      */
      void mouse_motion(void);

      /*
      ** Mouse click input
      */
      void mouse_click(void);

      /*
      ** Keyboard up-keys input
      */
      void keyboard_up(void);

      /*
      ** Keyboard down-keys input
      */
      void keyboard_down(void);

};

#endif
