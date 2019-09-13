#ifndef CONTROL_H
#define CONTROL_H

/*
 ** Special keys:
 **
 ** GLUT_KEY_F1
 ** F1 function key.
 **
 ** GLUT_KEY_F2
 ** F2 function key.
 **
 ** GLUT_KEY_F3
 ** F3 function key.
 **
 ** GLUT_KEY_F4
 ** F4 function key.
 **
 ** GLUT_KEY_F5
 ** F5 function key.
 **
 ** GLUT_KEY_F6
 ** F6 function key.
 **
 ** GLUT_KEY_F7
 ** F7 function key.
 **
 ** GLUT_KEY_F8
 ** F8 function key.
 **
 ** GLUT_KEY_F9
 ** F9 function key.
 **
 ** GLUT_KEY_F10
 ** F10 function key.
 **
 ** GLUT_KEY_F11
 ** F11 function key.
 **
 ** GLUT_KEY_F12
 ** F12 function key.
 **
 ** GLUT_KEY_LEFT
 ** Left directional key.
 **
 ** GLUT_KEY_UP
 ** Up directional key.
 **
 ** GLUT_KEY_RIGHT
 ** Right directional key.
 **
 ** GLUT_KEY_DOWN
 ** Down directional key.
 **
 ** GLUT_KEY_PAGE_UP
 ** Page up directional key.
 **
 ** GLUT_KEY_PAGE_DOWN
 ** Page down directional key.
 **
 ** GLUT_KEY_HOME
 ** Home directional key.
 **
 ** GLUT_KEY_END
 ** End directional key.
 **
 ** GLUT_KEY_INSERT
 ** Inset directional key.
 */

// Abstract Control class
class Control
{
   public:
      Control (void) { };

      Control (int x_in, int y_in) :
         x (x_in), y (y_in) { };

      virtual ~Control() = default;

   int get_x (void) { return x; };
   int get_y (void) { return y; };

   protected:
      int x;
      int y;
};

class MouseClick : public Control
{
   public:
      MouseClick (int button_in, int state_in, int x_in, int y_in) :
         Control (x_in, y_in), button (button_in), state (state_in) { };

   int get_button (void) { return button; };
   int get_state (void) { return state; };

   private:
      // GLUT_LEFT_BUTTON
      // GLUT_MIDDLE_BUTTON
      // GLUT_RIGHT_BUTTON
      int button;

      // GLUT_UP
      // GLUT_DOWN
      int state;
};

// when a button is pressed
class MouseMotion : public Control
{
   public:
      MouseMotion (int x_in, int y_in) :
         Control (x_in, y_in) { };
};

class MousePassive : public Control
{
   public:
      MousePassive (int x_in, int y_in) :
         Control (x_in, y_in) { };
};

class KeyboardUp : public Control
{
   public:
      KeyboardUp (const char key_in, int x_in, int y_in) :
         Control (x_in, y_in), key (key_in) { };

   const char get_key (void) { return key; };

   private:
      const char key;
};

class KeyboardDown : public Control
{
   public:
      KeyboardDown (
            const char key_in,
            int        x_in,
            int        y_in) :
         Control (x_in, y_in), key (key_in) { };

   const char get_key (void) { return key; };

   private:
      const char key;
};

class KeyboardSpecial : public Control
{
   public:
      KeyboardSpecial (int key_in, int x_in, int y_in) :
         Control (x_in, y_in), key (key_in) { };

   int get_key (void) { return key; };

   private:
      int key;
};

#endif
