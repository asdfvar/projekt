#ifndef MAP_H
#define MAP_H

class MAP
{
   public:

      MAP (int num_cells[3], int map_layer);
      ~MAP (void);

      const float *access_map (void) { return (const float*)map; };

      int map_dim (int dimension)
      {
        if (dimension == 0) return dim_x;
        if (dimension == 1) return dim_y;
        if (dimension == 2) return dim_z;
        else return 0;
      }

   private:

      float *map;

      int dim_x;
      int dim_y;
      int dim_z;

      int map_layer;
};

#endif
