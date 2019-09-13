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
      Control (int x_in, int y_in) :
      x (x_in), y (y_in) { };

   protected:
      int x;
      int y;
};

class MouseClick : public Control
{
   public:
      MouseClick (int button_in, int state_in, int x_in, int y_in) :
         Control (x_in, y_in), button (button_in), state (state_in) { };

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

class Keyboard_up : public Control
{
   public:
      Keyboard_up (const char key_in, int x_in, int y_in) :
         Control (x_in, y_in), key (key_in) { };

   private:
      const char key;
};

class Keyboard_down : public Control
{
   public:
      Keyboard_down (
            const char key_in,
            int        x_in,
            int        y_in) :
         Control (x_in, y_in), key (key_in) { };

   private:
      const char key;
};

class Keyboard_special : public Control
{
   public:
      Keyboard_special (int key_in, int x_in, int y_in) :
      Control (x_in, y_in), key (key_in) { };

   private:
      int key;
};

#endif
