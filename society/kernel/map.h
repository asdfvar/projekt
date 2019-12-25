#ifndef MAP_H
#define MAP_H

class MAP
{
   public:

      MAP (int num_cells[3]);
      ~MAP (void);

      const float *access_map (void) { return (const float*)map; };

      const float *access_ground (void) { return (const float*)ground; };

      int map_dim (int dim) { return size[dim]; };

   private:

      float *map;
      float *ground;

      int size[3];

      void set_ground (void);
};

#endif
