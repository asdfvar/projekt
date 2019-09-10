#ifndef FACADE_H
#define FACADE_H

class Facade {
   public:
      Facade (void);
      ~Facade (void);

      void keyboardDown (const char key);
      void specialFunc  (int key, int x, int y);
      void keyboardUp   (const char key);
      void mousePassive (int x, int y);
      void mouseMotion  (int x, int y);
      void mouse        (int button, int state, int x, int y);
      void idle         (void);
      void display      (void);
};

#endif
