#include "map.h"
#include <iostream>
#include <cstdlib>

/*
** constructor name: Map
*/
Map::Map( unsigned int id_in, int *map_dim_in, float *position_in)
{
   for (int ind = 0; ind < 3; ind++) if (map_dim_in[ind] % 2 == 0) std::cout << "map dimension must be odd" << std::endl;
   for (int ind = 0; ind < 3; ind++) map_dim[ind] = map_dim_in[ind];
   blocks = new int[map_dim[0] * map_dim[1] * map_dim[2]];

   id = id_in;
   for (int ind = 0; ind < 3; ind++) position[ind] = position_in[ind];

   create_random();
}

/*
** function name: change_position from: Map
*/
void Map::change_position(float *position_in)
{
   position[0] = position_in[0];
   position[1] = position_in[1];
   position[2] = position_in[2];
}

/*
** function name: create_random from: Map
*/
void Map::create_random( void)
{
   for (int ind = 0; ind < map_dim[0] * map_dim[1] * map_dim[2]; ind++)
   {
      int random_num = std::rand() % 100;
      if (random_num == 1)
         blocks[ind] = 1;
      else
         blocks[ind] = 0;
   }
}

void Map::display_info( void)
{
   bool block_exists = false;
   float l_position[3];
   for (int ind = 0; ind < map_dim[0] * map_dim[1] * map_dim[2]; ind++)
   {
      if (blocks[ind] > 0)
      {
         block_exists = true;
         int stat = get_position( l_position, ind);
         if (stat != 0)
         {
            std::cout << "block position = " << l_position[0] << ", "
                                             << l_position[1] << ", "
                                             << l_position[2] << std::endl;
         }
      }
      
   }

   if (block_exists)
   {
      std::cout << "block exists" << std::endl;
      std::cout << "map position = " << position[0] << ", " << position[1] << ", " << position[2] << std::endl;
   }
}

/*
** destructor name: ~Map
*/
Map::~Map(void)
{
   delete[] blocks;
}

/*
** function name: get_dimensions from Map
*/
unsigned int Map::get_dimensions(void)
{
   return map_dim[0] * map_dim[1] * map_dim[2];
}

/*
** function name: get_position from Map
*/
int Map::get_position(float *position_out,
                      int    block_index)
{

   if (block_index >= map_dim[0]*map_dim[1]*map_dim[2])
   {
      std::cout << "block index exceeds map dimensions" << std::endl;
      return 0;
   }

   // get block x,y,z indices
   unsigned int block_index_x = block_index  %  map_dim[0];
   unsigned int block_index_y = (block_index % (map_dim[0] * map_dim[1])) / map_dim[0];
   unsigned int block_index_z = block_index  / (map_dim[0] * map_dim[1]);

   // get relative position to this block
   float rel_block_pos_x = static_cast<float>(block_index_x);
   float rel_block_pos_y = static_cast<float>(block_index_y);
   float rel_block_pos_z = static_cast<float>(block_index_z);

   // convert to absolute position
   float block_pos_x = rel_block_pos_x + position[0];
   float block_pos_y = rel_block_pos_y + position[1];
   float block_pos_z = rel_block_pos_z + position[2];

   position_out[0] = block_pos_x;
   position_out[1] = block_pos_y;
   position_out[2] = block_pos_z;

   return blocks[block_index];

}

/*
** constructor name: Map_grid
*/
Map_grid::Map_grid(void)
{

   local_grid_size[0] = 11; // must be odd
   local_grid_size[1] = 11; // must be odd
   local_grid_size[2] = 11; // must be odd
   int total_local_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];

   virtual_grid_id_x = new int[local_grid_size[0]];
   virtual_grid_id_y = new int[local_grid_size[1]];
   virtual_grid_id_z = new int[local_grid_size[2]];

   for (int k = 0; k < local_grid_size[0]; k++) virtual_grid_id_x[k] = k;
   for (int k = 0; k < local_grid_size[1]; k++) virtual_grid_id_y[k] = k;
   for (int k = 0; k < local_grid_size[2]; k++) virtual_grid_id_z[k] = k;

   maps.reserve( total_local_grid_size);
   std::vector<Map*>::size_type sz = maps.capacity();

   int map_dim[3] = {65, 65, 65};
   int ind;
   for (int ind_z = -local_grid_size[2]/2, ind = 0; ind_z <= local_grid_size[2]/2; ind_z++)
   {
      for (int ind_y = -local_grid_size[1]/2; ind_y <= local_grid_size[1]/2; ind_y++)
      {
         for (int ind_x = -local_grid_size[0]/2; ind_x <= local_grid_size[0]/2; ind_x++, ind++)
         {
           float position[3] = {(float)(ind_x * map_dim[0]),
                                (float)(ind_y * map_dim[1]),
                                (float)(ind_z * map_dim[2])};

           maps.push_back( new Map(ind, map_dim, position));

           if (sz < maps.capacity())
           {
              std::cout << "Map grid capacity increased" << std::endl;
              sz = maps.capacity();
           }

           if (ind % 100 == 0)
           {
              std::cout << (float)ind / (float)total_local_grid_size << std::endl;
           }
         }
      }
   }

}

/*
** destructor name: ~Map_grid
*/
Map_grid::~Map_grid(void)
{
   std::size_t total_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];
   maps.erase (maps.begin(), maps.begin() + total_grid_size);

   delete[] virtual_grid_id_x;
   delete[] virtual_grid_id_y;
   delete[] virtual_grid_id_z;
}

/*
** function name: access_map from: Map_grid
** Return pointer to the specified Map block
*/
Map *Map_grid::access_map(int v_id_x,
                          int v_id_y,
                          int v_id_z)
{
   unsigned int index =  virtual_grid_id_x[v_id_x] + 
                         virtual_grid_id_y[v_id_y] * local_grid_size[0] +
                         virtual_grid_id_z[v_id_z] * local_grid_size[0] * local_grid_size[1];

   return maps.at( index );
}

/*
** function name: shift from: Map_grid
** Circular shift the cube in the specified direction
*/
void Map_grid::shift( int x, int y, int z)
{
   for (int k = 0; k < local_grid_size[0]; k++)
   {
      for (virtual_grid_id_x[k] -= x;
           virtual_grid_id_x[k] < 0;
           virtual_grid_id_x[k] += local_grid_size[0]) { /* N/A */};
   }
   for (int k = 0; k < local_grid_size[1]; k++)
   {
      for (virtual_grid_id_y[k] -= y;
           virtual_grid_id_y[k] < 0;
           virtual_grid_id_y[k] += local_grid_size[1]) { /* N/A */};
   }
   for (int k = 0; k < local_grid_size[2]; k++)
   {
      for (virtual_grid_id_z[k] -= z;
           virtual_grid_id_z[k] < 0;
           virtual_grid_id_z[k] += local_grid_size[2]) { /* N/A */};
   }
}

