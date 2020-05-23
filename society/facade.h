#ifndef FACADE_H
#define FACADE_H

#include "society.h"
#include "graphics.h"
#include "menu_system.h"
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

      BaseMenu          *active_menu;
      MainMenu           main_menu;
      BuildMenu          build_menu;
      BuildMaterialMenu  build_material_menu;

      struct timeval start, end;
      double time_step = 0.01f;

      float *unit_positions_x;
      float *unit_positions_y;
      float *unit_positions_z;
      bool  *unit_selections;

      int map_layer;

      int mode;
      unsigned int jobmaterial;

      // Window-view transform and translation 
      float transform[4];
      float translation[2];

      bool control_down;
      bool shift_down;
      bool button0_down;
      bool button1_down;
      bool button2_down;
      bool z_down;
      bool has_moved_passively;

      int mouse_wheel_backward_count;
      int mouse_wheel_forward_count;

      int mouse_pos[2];

      HUD      hud;
      Graphics graphics;

      float selection_box[2][3];
      bool selection_active;
};

#endif
