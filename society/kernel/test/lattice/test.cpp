#include "container.h"
#include "map.h"
#include <iostream>

#define I 3
#define J 4
#define K 3

int main (void)
{
   int size[3] = {I, J, K};

   Lattice<Cell> lattice (size, 3);

   for (int k = 0; k < K; k++)
   {
      for (int j = 0; j < J; j++)
      {
         for (int i = 0; i < I; i++)
         {
            int index[3] = {i, j, k};
            Cell *cell = lattice.access (index);
            cell->number = k * 100 + j * 10 + i;
         }
      }
   }

   for (int ind = 0; ind < I*J*K; ind++)
   {
      Cell *cell = lattice.next ();
      std::cout << "number = " << cell->number << std::endl;
   }

   std::cout << std::endl;
   for (int k = 0; k < K; k++)
   {
      for (int j = 0; j < J; j++)
      {
         for (int i = 0; i < I; i++)
         {
            int index[3] = {i, j, k};
            Cell *cell = lattice.access (index);
            std::cout << cell->number << ", ";
         }
         std::cout << std::endl;
      }
      std::cout << std::endl;
   }

   return 0;
}
