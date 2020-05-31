#ifndef SOCIETY_H
#define SOCIETY_H

#include "map.h"
#include <queue>
#include <vector>
#include <list>
#include "unit.h"
#include "jobs.h"
#include "container.h"

class Society
{
   public:

      Society (void);
      ~Society (void);

      void set_view (float transform[4], float translation[2]);
      void set_map_layer (int map_layer) { Map->set_map_layer (map_layer); };
      void update (float time_step);
      void set_destination (int dest[3], bool selected_units);
      const bool *access_air ();
      const int get_size (int ind) { return size[ind]; };
      void select_units (int cell_selections[2][3], int map_layer, bool control_down);
      void select_all (void);
      void unselect_all (void);
      void set_group (int group_number);
      void select_group (int group_number);
      void select_cells (int cell_selections[2][3]);
      void set_select_cells (bool reset_uncommitted_jobs_size, int mode);

      void set_jobs (int job_type, unsigned int jobmaterial);

      // Graphics
      void draw (
            float *transform,
            float *translation,
            int    map_layer);

      // Draw units
      void draw_units (float *transform, float *translation, int map_layer);

      // Draw selected cells not set to be queued
      void draw_uncommitted_jobs (
            float       *transform,
            float       *translation,
            int          map_layer);

      // Draw jobs that are queued for assignment to a unit
      void draw_queued_jobs (
            float       *transform,
            float       *translation,
            int          map_layer);

      // Draw jobs that are assigned to individual units
      void draw_unit_jobs (
            float       *transform,
            float       *translation,
            int          map_layer);

   private:

      // Contains information and contents that make up the map as well as
      // functionality for accessing specifics about the map
      MAP *Map;

      // Map size
      int  size[3];

      // Float-type scratch space
      float *fbuffer;

      // Integer-type scratch space
      int *ibuffer;

      // Current unit index associated with assigning jobs to units
      int current_job_unit_index;

      // Current job index associated with assigning jobs to units
      int current_job_index;

      // Units
      Container<Unit> units;

      // Jobs committed to the queue for assignment to a unit
      Container<Job> queued_jobs;

      // Array of containers that contain pointers to the units assigned
      // to the the specified group number
      Container<Unit> group[10];
};

#endif
