#ifndef FACADE_H
#define FACADE_H

#include "society.h"
#include <queue>
#include <mutex>
#include <sys/time.h>

class Facade {
   public:
      Facade (void);
      ~Facade (void);

      void keyboardDown  (const char key, int x, int y);
      void specialFunc   (int        key, int x, int y);
      void specialUpFunc (int        key, int x, int y);
      void keyboardUp    (const char key, int x, int y);
      void mousePassive  (int x, int y);
      void mouseMotion   (int x, int y);
      void mouseClick    (int button, int state, int x, int y);
      void idle          (void);
      void display       (void);

   private:

      Society society;

      struct timeval start, end;

      float *unit_positions_x;
      float *unit_positions_y;
      float *unit_positions_z;
      bool  *unit_selections;

      int map_layer;

      float transform[4];
      float translation[2];

      bool control_down;
      bool shift_down;
      bool button0_down;
      bool button1_down;
      bool button2_down;
      bool z_down;

      int mouse_wheel_backward_count;
      int mouse_wheel_forward_count;

      int mouse_pos[2];

      float selection_box[4];
      bool selection_active;
};

#endif
