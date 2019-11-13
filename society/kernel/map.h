#ifndef MAP_H
#define MAP_H

class MAP
{
   public:
      MAP (int num_cells[3]);
      ~MAP (void);

   private:
      float *map;

      int dim_x;
      int dim_y;
      int dim_z;
};

#endif
