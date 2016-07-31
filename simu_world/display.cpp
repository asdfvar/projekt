#include "opengl_interface.h"
#include <cmath>

#define TEST_DRAW_CIRCLE
#define TEST_DRAW_LINE
#define TEST_DRAW_3D_TRIANGLE

void Opengl_interface::display(void)
{

float mat_specular[4]   = { 1.0f, 1.0f, 1.0f, 1.0f };
float mat_shininess[1]  = { 50.0f };
//glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
//glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
float light_ambient[] = {0.2, 0.2, 0.2, 1.0};
float light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
float light_specular[] = {0.8, 0.8, 0.8, 1.0};
float light_position[] = {1.0, 1.0, 1.0, 0.0};

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

#ifdef TEST_DRAW_CIRCLE
{
char red   = 100;
char green = 255;
char blue  = 100;

glColor3ub(red, green, blue);
glBegin(GL_POLYGON);
for (int k = 0; k < 80; k++)
{
   float x = 0.0f + 0.1f/2.0f * cosf(2.0f * 3.14159f * (float)k / 80.0f);
   float y = 0.0f + 0.1f/2.0f * sinf(2.0f * 3.14159f * (float)k / 80.0f);
   glVertex3f(x, y, 0.0f);
}
glEnd();
}

#endif

#ifdef TEST_DRAW_3D_TRIANGLE
{
float red   = 0.5f;
float green = 0.5f;
float blue  = 1.0f;
glColor3f(red, green, blue);
glBegin(GL_POLYGON);
glNormal3f(1.0f, 1.0f, -0.5f); // guestimating this (too lazy)
   glVertex3f(-0.5, 0.5, 0.0f);
   glVertex3f(-0.8, 0.8, 0.8f);
   glVertex3f(-0.9, 0.2, -0.8f);
glEnd();
glBegin(GL_POLYGON);
glNormal3f(0.0f, 0.0f, -0.5f); // guestimating this (too lazy)
   glVertex3f(-0.5, 0.5, 0.0f);
   glVertex3f(-0.8, 0.8, 0.8f);
   glVertex3f(-0.6, 0.9, -0.8f);
glEnd();
glBegin(GL_POLYGON);
glNormal3f(1.0f, 1.0f, -0.5f); // guestimating this (too lazy)
   glVertex3f(0.5, 0.5, 0.0f);
   glVertex3f(0.2, 0.5, 0.0f);
   glVertex3f(0.35, 0.7, 0.0f);
glEnd();
glBegin(GL_POLYGON);
glNormal3f(0.0f, 0.0f, -0.5f); // guestimating this (too lazy)
   glVertex3f(0.3, 0.0, 0.8f);
   glVertex3f(0.0, 0.0, 0.8f);
   glVertex3f(0.15, 0.2, 0.8f);
glEnd();
glBegin(GL_POLYGON);
glNormal3f(1.0f, 1.0f, 0.5f); // guestimating this (too lazy)
   glVertex3f(0.5, -0.5, -0.8f);
   glVertex3f(0.2, -0.5, -0.8f);
   glVertex3f(0.35, -0.3, -0.8f);
glEnd();
}
#endif

#ifdef TEST_DRAW_LINE
{
float red   = 1.0f;
float green = 0.5f;
float blue  = 0.5f;
glLineWidth(2.5);
glColor3f(red, green, blue);
glBegin(GL_LINES);
glVertex3f(-1.0f, -0.9f ,0.0);
glVertex3f(0.5, 0.5f, 0.0);
glEnd();
}
#endif

glFlush();

}
