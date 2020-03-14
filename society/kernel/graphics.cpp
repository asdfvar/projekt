#include "society.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include "map.h"

#define X_START -1.0f
#define Y_START -1.0f

#define WIDTH   1.0f
#define HEIGHT  1.0f

static inline void transformation (float point[2], float transform[4], float translation[2])
{
   point[0] += translation[0];
   point[1] += translation[1];

   float part1a = point[0] * transform[0];
   float part1b = point[1] * transform[1];

   float part2a = point[0] * transform[2];
   float part2b = point[1] * transform[3];

   point[0] = part1a + part1b;
   point[1] = part2a + part2b;
}

void Society::draw_units (
      float *transform,
      float *translation,
      int    map_layer)
{
   int row_max = size[1];
   int col_max = size[0];

   float block_height = HEIGHT / static_cast<float>(row_max / 2);
   float block_width  = WIDTH  / static_cast<float>(col_max / 2);

   const float window_start_row = -1.0f;
   const float window_end_row   =  1.0f;
   const float window_start_col = -1.0f;
   const float window_end_col   =  1.0f;

   const float selected_width_offset  = 0.35f * block_width;
   const float selected_height_offset = 0.35f * block_height;

   glLineWidth (0.1f);
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

      float unit_pos[3] = {
         unit->get_position (0),
         unit->get_position (1),
         unit->get_position (2) };

      if (floorf (unit_pos[2]) <= map_layer)
      {
         int map_2d_ind[2] = { (int)unit_pos[0], (int)unit_pos[1] };

         if (unit_pos[2] >= Map->get_view_plain (map_2d_ind))
         {
            float x_pos = unit->get_position (0) / ((float)col_max) *
               (window_end_col - window_start_col) +
               window_start_col;

            float y_pos = unit->get_position (1) / ((float)row_max) *
               (window_end_row - window_start_row) +
               window_start_row;

            float reduction_factor = 1.0f - (float)(map_layer - unit_pos[2]) / 8.0f;
            if (reduction_factor < 0.05f) reduction_factor = 0.05f;

            float width_offset  = 0.3f * block_width  * reduction_factor;
            float height_offset = 0.3f * block_height * reduction_factor;

            float point0[2] = { x_pos - width_offset, y_pos - height_offset };
            float point1[2] = { x_pos - width_offset, y_pos + height_offset };
            float point2[2] = { x_pos + width_offset, y_pos + height_offset };
            float point3[2] = { x_pos + width_offset, y_pos - height_offset };

            transformation (point0, transform, translation);
            transformation (point1, transform, translation);
            transformation (point2, transform, translation);
            transformation (point3, transform, translation);

            glBegin (GL_POLYGON);
            glColor3f (1.0f, 1.0f, 0.0f);
            glVertex2f (point0[0], point0[1]);
            glVertex2f (point1[0], point1[1]);
            glVertex2f (point2[0], point2[1]);
            glVertex2f (point3[0], point3[1]);
            glEnd ();

            if (unit->is_selected())
            {
               float point0[2] = { x_pos - selected_width_offset, y_pos - selected_height_offset };
               float point1[2] = { x_pos - selected_width_offset, y_pos + selected_height_offset };
               float point2[2] = { x_pos + selected_width_offset, y_pos + selected_height_offset };
               float point3[2] = { x_pos + selected_width_offset, y_pos - selected_height_offset };

               transformation (point0, transform, translation);
               transformation (point1, transform, translation);
               transformation (point2, transform, translation);
               transformation (point3, transform, translation);

               glBegin (GL_LINES);
               glColor3f (1.0f, 0.0f, 0.0f);
               glVertex2f (point0[0], point0[1]);
               glVertex2f (point1[0], point1[1]);
               glVertex2f (point1[0], point1[1]);
               glVertex2f (point2[0], point2[1]);
               glVertex2f (point2[0], point2[1]);
               glVertex2f (point3[0], point3[1]);
               glVertex2f (point3[0], point3[1]);
               glVertex2f (point0[0], point0[1]);
               glEnd ();
            }
         }
      }
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

   // Draw cells
   for (int row = 0, ind = 0; row < row_max; row++)
   {
      for (int col = 0; col < col_max; col++, ind++)
      {
         float vertex_x = starting_col_loc + block_width  * (float)col;
         float vertex_y = starting_row_loc + block_height * (float)row;

         glBegin (GL_POLYGON);
         if (view_plain[ind] == 0) {
            glColor3f (0.2f, 0.2f, 0.2f);
         } else {
            float scale = (1.0f - (float)view_plain[ind] / 8.0f);
            if (scale < 0.0f) scale = 0.0f;
            glColor3f (0.4f * scale, 0.7f * scale, 0.2f * scale);
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

   // Draw the active job cells
   for (int unit_ind = 0; unit_ind < units.size (); unit_ind++)
   {
      Unit *unit = units.access (unit_ind);

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
