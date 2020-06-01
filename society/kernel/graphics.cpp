#include "society.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "map.h"
#include "utils.h"
#include "item.h"

#define X_START -1.0f
#define Y_START -1.0f

#define WIDTH   1.0f
#define HEIGHT  1.0f

void Society::draw_units (
      float *transform,
      float *translation,
      int    map_layer)
{
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);
      unit->draw (transform, translation);
   }
}

void MAP::draw (float *transform, float *translation)
{
   int row_max = size[1];
   int col_max = size[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   float starting_row_loc = Y_START;
   float starting_col_loc = X_START;

#ifdef DEVELOPER
   // Draw grid lines
   glLineWidth (0.1f);
   for (int row = 0; row < row_max; row++)
   {
      for (int col = 0; col < col_max; col++)
      {
         float vertex_x = starting_col_loc + block_width  * (float)col;
         float vertex_y = starting_row_loc + block_height * (float)row;

         float point0[2] = { vertex_x,               vertex_y               };
         float point1[2] = { vertex_x + block_width, vertex_y               };
         float point2[2] = { vertex_x + block_width, vertex_y + block_width };
         float point3[2] = { vertex_x,               vertex_y + block_width };

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glBegin (GL_LINES);
         glColor3f (0.0f, 0.0f, 1.0f);
         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glVertex2f (point3[0], point3[1]);
         glVertex2f (point0[0], point0[1]);
         glEnd();
      }
   }
#endif

   // Draw cells
   for (int row = 0, ind = 0; row < row_max; row++)
   {
      for (int col = 0; col < col_max; col++, ind++)
      {
         float vertex_x = starting_col_loc + block_width  * (float)col;
         float vertex_y = starting_row_loc + block_height * (float)row;

         // Determine the color of the cell. This will depend on the view depth and material
         glBegin (GL_POLYGON);
         if (view_plain[ind] == 0 || view_depth[ind] == 0) {
            glColor3f (0.2f, 0.2f, 0.2f);
         } else {
            float scale = (1.0f - (float)view_depth[ind] / (float)max_depth);
            if (scale < 0.0f) scale = 0.0f;
            if (view_plain[ind] == mid::dirt)
               glColor3f (0.3922f * scale, 0.2549f * scale, 0.0902f * scale); // UPS brown
            if (view_plain[ind] == mid::grass)
               glColor3f (0.4f * scale, 0.7f * scale, 0.2f * scale); // grass green
            if (view_plain[ind] == mid::stone)
               glColor3f (0.9f * scale, 0.9f * scale, 0.9f * scale); // grey
         }

         float point0[2] = { vertex_x,               vertex_y               };
         float point1[2] = { vertex_x + block_width, vertex_y               };
         float point2[2] = { vertex_x + block_width, vertex_y + block_width };
         float point3[2] = { vertex_x,               vertex_y + block_width };

         transformation (point0, transform, translation);
         transformation (point1, transform, translation);
         transformation (point2, transform, translation);
         transformation (point3, transform, translation);

         glVertex2f (point0[0], point0[1]);
         glVertex2f (point1[0], point1[1]);
         glVertex2f (point2[0], point2[1]);
         glVertex2f (point3[0], point3[1]);
         glEnd ();
      }
   }
}

void Society::draw_uncommitted_jobs (
      float       *transform,
      float       *translation,
      int          map_layer)
{
   int row_max = size[1];
   int col_max = size[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float starting_row_loc = Y_START;
   const float starting_col_loc = X_START;

   glColor3f (1.0f, 0.0f, 0.0f);

   const bool *uncommitted_jobs = Map->access_uncommitted_jobs ();

   // Draw the job cells
   for (int indy = 0, job_ind = 0; indy < size[1]; indy++)
   {
      for (int indx = 0; indx < size[0]; indx++, job_ind++)
      {
         bool local_select = uncommitted_jobs[size[0]*size[1] * map_layer + job_ind];

         if (local_select == true)
         {
            float vertex_x = starting_col_loc + block_width  * (float)indx;
            float vertex_y = starting_row_loc + block_height * (float)indy;

            glBegin (GL_POLYGON);

            float point0[2] = { vertex_x,               vertex_y               };
            float point1[2] = { vertex_x + block_width, vertex_y               };
            float point2[2] = { vertex_x + block_width, vertex_y + block_width };
            float point3[2] = { vertex_x,               vertex_y + block_width };

            transformation (point0, transform, translation);
            transformation (point1, transform, translation);
            transformation (point2, transform, translation);
            transformation (point3, transform, translation);

            glVertex2f (point0[0], point0[1]);
            glVertex2f (point1[0], point1[1]);
            glVertex2f (point2[0], point2[1]);
            glVertex2f (point3[0], point3[1]);
            glEnd ();
         }
      }
   }
}

void Society::draw_queued_jobs (
      float       *transform,
      float       *translation,
      int          map_layer)
{
   int row_max = size[1];
   int col_max = size[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float starting_row_loc = Y_START;
   const float starting_col_loc = X_START;

   glColor3f (0.3f, 0.0f, 0.0f);

   // Draw the job cells
   for (int job_ind = 0; job_ind < queued_jobs.size (); job_ind++)
   {
      Job *job = queued_jobs.access (job_ind);
//if (queued_jobs.test_nulls ()) std::cout << "NULLS!" << std::endl;
if (job == nullptr) std::cout << __FILE__ << __LINE__ << ":queued_jobs size = " << queued_jobs.size () << std::endl;
      if (job->get_position (2) != map_layer) continue;

      int col = job->get_position (0);
      int row = job->get_position (1);

      float vertex_x = starting_col_loc + block_width  * (float)col;
      float vertex_y = starting_row_loc + block_height * (float)row;

      glBegin (GL_POLYGON);

      float point0[2] = { vertex_x,               vertex_y               };
      float point1[2] = { vertex_x + block_width, vertex_y               };
      float point2[2] = { vertex_x + block_width, vertex_y + block_width };
      float point3[2] = { vertex_x,               vertex_y + block_width };

      transformation (point0, transform, translation);
      transformation (point1, transform, translation);
      transformation (point2, transform, translation);
      transformation (point3, transform, translation);

      glVertex2f (point0[0], point0[1]);
      glVertex2f (point1[0], point1[1]);
      glVertex2f (point2[0], point2[1]);
      glVertex2f (point3[0], point3[1]);
      glEnd ();
   }
}

void Society::draw_unit_jobs (
      float *transform,
      float *translation,
      int    map_layer)
{
   int row_max = size[1];
   int col_max = size[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float starting_row_loc = Y_START;
   const float starting_col_loc = X_START;

   // Draw the active job cells
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      if (unit->num_jobs () > 0) {
         for (int job_ind = 0; job_ind < unit->num_jobs (); job_ind++)
         {
            Job *job;
            if (job_ind == 0)
            {
               job = unit->access_active_job ();
               glColor3f (0.0f, 1.0f, 1.0f);
            }
            else
            {
               job = unit->access_job (job_ind - 1);
               glColor3f (0.0f, 0.5f, 0.5f);
            }

            if (job == nullptr) return;

            if (job->get_position (2) != map_layer) continue;

            int col = job->get_position (0);
            int row = job->get_position (1);

            float vertex_x = starting_col_loc + block_width  * (float)col;
            float vertex_y = starting_row_loc + block_height * (float)row;

            glBegin (GL_POLYGON);

            float point0[2] = { vertex_x,               vertex_y               };
            float point1[2] = { vertex_x + block_width, vertex_y               };
            float point2[2] = { vertex_x + block_width, vertex_y + block_width };
            float point3[2] = { vertex_x,               vertex_y + block_width };

            transformation (point0, transform, translation);
            transformation (point1, transform, translation);
            transformation (point2, transform, translation);
            transformation (point3, transform, translation);

            glVertex2f (point0[0], point0[1]);
            glVertex2f (point1[0], point1[1]);
            glVertex2f (point2[0], point2[1]);
            glVertex2f (point3[0], point3[1]);
            glEnd ();
         }
      }
   }
}

void Society::draw (
      float *transform,
      float *translation,
      int    map_layer)
{

   draw_unit_jobs (
         transform,
         translation,
         map_layer);

   draw_uncommitted_jobs (
         transform,
         translation,
         map_layer);

   draw_units (
         transform,
         translation,
         map_layer);

   draw_queued_jobs (
         transform,
         translation,
         map_layer);

   Map->draw (transform, translation);
}
