#include "map.h"
#include <iostream>
#include <cstdlib>

/*
** constructor name: Map
*/
Map::Map( unsigned int id_in, int *map_dim_in, float *position_in)
{
   for (int ind = 0; ind < 3; ind++)
   {
      if (map_dim_in[ind] % 2 == 0)
      {
         std::cout << "map dimension must be odd" << std::endl;
      }
   }

   for (int ind = 0; ind < 3; ind++) map_dim[ind] = map_dim_in[ind];

   blocks = new int[map_dim[0] * map_dim[1] * map_dim[2]];

   id = id_in;

   for (int ind = 0; ind < 3; ind++) position[ind] = position_in[ind];

   create_random();
}

/*
** function name: change_position from: Map
** TODO: obsolete
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
      if (random_num == 0)
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
** function name: position_in_map from Map
**
** returns true if the position is inside the Map
** with the boundaries defined as:
** [ , ) x [ , ) x [ , )
*/
bool Map::position_in_map( float *position_in)
{
   if (position_in[0] >= position[0]                                  &&
       position_in[0] <  position[0] + static_cast<float>(map_dim[0]) &&
       position_in[1] >= position[1]                                  &&
       position_in[1] <  position[1] + static_cast<float>(map_dim[1]) &&
       position_in[2] >= position[2]                                  &&
       position_in[2] <  position[2] + static_cast<float>(map_dim[2]))
   {
      return true;
   }

   return false;
}

/*
** function name: move from Map
**
** move the map a discrete number of chunks
*/
void Map::move( int x, int y, int z)
{
   position[0] += static_cast<float>(x) * map_dim[0];
   position[1] += static_cast<float>(y) * map_dim[1];
   position[2] += static_cast<float>(z) * map_dim[2];
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

   if (block_index >= map_dim[0] * map_dim[1] * map_dim[2])
   {
      std::cout << "block index exceeds map dimensions" << std::endl;
      return 0;
   }

   // get block x, y, z indices
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

   position_out[0] = block_pos_x + 0.5f;
   position_out[1] = block_pos_y + 0.5f;
   position_out[2] = block_pos_z + 0.5f;

   return blocks[block_index];

}

/*
** constructor name: Map_grid
*/
Map_grid::Map_grid(void)
{

   local_grid_size[0] = 13;
   local_grid_size[1] = 13;
   local_grid_size[2] = 13;
   int total_local_grid_size = local_grid_size[0] * local_grid_size[1] * local_grid_size[2];

   virtual_grid_id_x = new int[local_grid_size[0]];
   virtual_grid_id_y = new int[local_grid_size[1]];
   virtual_grid_id_z = new int[local_grid_size[2]];

   for (int k = 0; k < local_grid_size[0]; k++) virtual_grid_id_x[k] = k;
   for (int k = 0; k < local_grid_size[1]; k++) virtual_grid_id_y[k] = k;
   for (int k = 0; k < local_grid_size[2]; k++) virtual_grid_id_z[k] = k;

   maps.reserve( total_local_grid_size);
   std::vector<Map*>::size_type sz = maps.capacity();

   int map_dim[3] = {5, 5, 5};
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

void Map_grid::update( float *position)
{
   int virtual_grid[3] = { -1, -1, -1 };

   for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
   {
      for (int y_dir = 0; y_dir < local_grid_size[0]; y_dir++)
      {
         for (int z_dir = 0; z_dir < local_grid_size[0]; z_dir++)
         {

            Map *this_map = access_map( x_dir,
                                        y_dir,
                                        z_dir);

            if (this_map->position_in_map( position))
            {
               virtual_grid[0] = virtual_grid_id_x[x_dir];
               virtual_grid[1] = virtual_grid_id_y[y_dir];
               virtual_grid[2] = virtual_grid_id_z[z_dir];
            }

         }
      }
   }

   int mid_block[3] = { local_grid_size[0] / 2,
                        local_grid_size[1] / 2,
                        local_grid_size[2] / 2 };

   if (virtual_grid[0] > mid_block[0])
   {
      shift(1, 0, 0);
   }
   if (virtual_grid[0] < mid_block[0])
   {
      shift(-1, 0, 0);
   }
   if (virtual_grid[1] > mid_block[1])
   {
      shift(0, 1, 0);
   }
   if (virtual_grid[1] < mid_block[1])
   {
      shift(0, -1, 0);
   }
   if (virtual_grid[2] > mid_block[2])
   {
      shift(0, 0, 1);
   }
   if (virtual_grid[2] < mid_block[2])
   {
      shift(0, 0, -1);
   }

}

/*
** function name: access_map from: Map_grid
** Return pointer to the specified Map block
*/
Map *Map_grid::access_map(int p_id_x,
                          int p_id_y,
                          int p_id_z)
{
   if (p_id_x < 0 || p_id_x >= local_grid_size[0] ||
       p_id_y < 0 || p_id_y >= local_grid_size[1] ||
       p_id_z < 0 || p_id_z >= local_grid_size[2])
   {
      std::cout << "select index of " << p_id_x << ", "  << p_id_x << ", "  << p_id_x <<
                   " out of bounds: " << local_grid_size[0] << ", " <<
                                         local_grid_size[1] << ", " <<
                                         local_grid_size[2] << std::endl;
   }

   unsigned int index =  p_id_x + 
                         p_id_y * local_grid_size[0] +
                         p_id_z * local_grid_size[0] * local_grid_size[1];

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
      
      virtual_grid_id_x[k] -= x;
      if (virtual_grid_id_x[k] >= local_grid_size[0])
      {
         for (;virtual_grid_id_x[k] >= local_grid_size[0];
               virtual_grid_id_x[k] -= local_grid_size[0]) { /* NA */}

         for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local map position
               Map *this_map = access_map( k,
                                           y_dir,
                                           z_dir);

               this_map->move( -local_grid_size[0], 0, 0);
            }
         }

      }

      if (virtual_grid_id_x[k] < 0)
      {
         for (;virtual_grid_id_x[k] < 0; virtual_grid_id_x[k] += local_grid_size[0]) { /* NA */}

         for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local map position
               Map *this_map = access_map( k,
                                           y_dir,
                                           z_dir);

               this_map->move( local_grid_size[0], 0, 0);
            }
         }
      }

   }

   for (int k = 0; k < local_grid_size[1]; k++)
   {
      virtual_grid_id_y[k] -= y;
      if (virtual_grid_id_y[k] >= local_grid_size[1])
      {
         for (;virtual_grid_id_y[k] >= local_grid_size[1];
               virtual_grid_id_y[k] -= local_grid_size[1]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local map position
               Map *this_map = access_map( x_dir,
                                           k,
                                           z_dir);

               this_map->move( 0, -local_grid_size[1], 0);
            }
         }

      }

      if (virtual_grid_id_y[k] < 0)
      {
         for (;virtual_grid_id_y[k] < 0; virtual_grid_id_y[k] += local_grid_size[1]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int z_dir = 0; z_dir < local_grid_size[2]; z_dir++)
            {

               // shift the local map position
               Map *this_map = access_map( x_dir,
                                           k,
                                           z_dir);

               this_map->move( 0, local_grid_size[1], 0);
            }
         }
      }


   }

   for (int k = 0; k < local_grid_size[2]; k++)
   {
      virtual_grid_id_z[k] -= z;
      if (virtual_grid_id_z[k] >= local_grid_size[2])
      {
         for (;virtual_grid_id_z[k] >= local_grid_size[2];
               virtual_grid_id_z[k] -= local_grid_size[2]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
            {

               // shift the local map position
               Map *this_map = access_map( x_dir,
                                           y_dir,
                                           k);

               this_map->move( 0, 0, -local_grid_size[2]);
            }
         }

      }

      if (virtual_grid_id_z[k] < 0)
      {
         for (;virtual_grid_id_z[k] < 0; virtual_grid_id_z[k] += local_grid_size[2]) { /* NA */}

         for (int x_dir = 0; x_dir < local_grid_size[0]; x_dir++)
         {
            for (int y_dir = 0; y_dir < local_grid_size[1]; y_dir++)
            {

               // shift the local map position
               Map *this_map = access_map( x_dir,
                                           y_dir,
                                           k);

               this_map->move( 0, 0, local_grid_size[2]);
            }
         }
      }


   }
}

int Map_grid::get_grid_size(int ind)
{
   return local_grid_size[ind];
}
