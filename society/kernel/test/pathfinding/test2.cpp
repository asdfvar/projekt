#include "pathfinding.h"
#include "timer.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

int main ()
{

   int I, J, K;

   int Imax = 100;
   int Jmax = 100;
   int Kmax = 100;

   float *nodes  = new float[Imax * Jmax * Kmax];
   float *cost   = new float[Imax * Jmax * Kmax];
   int   *path   = new int  [Imax * Jmax * Kmax];
   float *buffer = new float[Imax * Jmax * Kmax];

   int length = 40;

#if 0
   I = J = K = length;

   int dim[3] = { I, J, K };

   int src[3] = { 0, 0, 0 };
   int dst[3] = { I - 1, J - 1, K - 1 };

   int random_num = rand();

   for (int ind = 0; ind < Imax * Jmax * Kmax; ind++) nodes[ind] = 0.0f;

   std::cout << I << " x " << J << " x " << K << " = " << I * J * K << std::endl;

   long start_time = startTime ();
   // start from the destination point and branch outward
   int itt = cost_function (
         nodes,
         cost,
         dim,
         dst,       // source (cost = 0) will be the destination point
         src,       // destination point will be the starting point
         buffer);

   float cost_function_time = endTime (start_time);
   std::cout << "cost function time: " << cost_function_time << std::endl;
   std::cout << "with " << itt << " itterations" << std::endl;

   start_time = startTime ();
   int path_size = pathfinding (
         cost,
         dim,
         src,
         dst,
         path);

   float path_finding_time = endTime (start_time);
   std::cout << "path finding time: " << path_finding_time << std::endl;
   std::cout << std::endl;
#endif

#if 0
   for (int k = 0, ind = 0; k < K; k++) {
      for (int j = 0; j < J; j++) {
         for (int i = 0; i < I; i++, ind++)
         {
            bool found = false;
            for (int search_ind = 0; search_ind < path_size; search_ind++)
               if (path[search_ind] == ind) found = true;

            if (ind == (src[2] * I * J) + (src[1] * I) + src[0]) std::cout << "S";
            else if (ind == (dst[2] * I * J)+(dst[1] * I) + dst[0]) std::cout << "E";
            else if (found)          std::cout << "x";
            else if (nodes[ind] < 0) std::cout << "O";
            else                     std::cout << ".";
         }
         std::cout << std::endl;
         }
         std::cout << std::endl;
      }
#endif

   std::cout << std::endl;

#if 1
   for (float density = 0.0f; density < 1.00f; density += 0.1f) {

      I = J = K = length;

      int dim[3] = { I, J, K };

      int src[3] = { 0, 0, 0 };
      int dst[3] = { I - 1, J - 1, K - 1 };


      for (int ind = 0; ind < Imax * Jmax * Kmax; ind++) nodes[ind] = 0.0f;

      for (int ind = 0; ind < I * J * K; ind++) {
         int random_num = rand() % 100;

         float frand_num = ((float)random_num) / 100.0f;

         if (frand_num < density) nodes[ind] = -1;
      }

      std::cout << I << " x " << J << " x " << K << " = "
                << I * J * K << " @ density " << density
                << std::endl;

      long start_time = startTime ();
      // start from the destination point and branch outward
      bool found =
      cost_function (
            nodes,
            cost,
            dim,
            dst,       // source (cost = 0) will be the destination point
            src,       // destination point will be the starting point
            buffer);

      float cost_function_time = endTime (start_time);
      std::cout << "cost function time: " << cost_function_time << std::endl;
      if (found) std::cout << "found solution" << std::endl;

      start_time = startTime ();
      int path_size = pathfinding (
            cost,
            dim,
            src,
            dst,
            path);

      float path_finding_time = endTime (start_time);
      std::cout << "path finding time: " << path_finding_time << std::endl;
      std::cout << std::endl;

#if 0
      for (int k = 0, ind = 0; k < K; k++) {
         for (int j = 0; j < J; j++) {
            for (int i = 0; i < I; i++, ind++)
            {
               bool found = false;
               for (int search_ind = 0; search_ind < path_size; search_ind++)
                  if (path[search_ind] == ind) found = true;

               if (ind == (src[2] * I * J) + (src[1] * I) + src[0]) std::cout << "S";
               else if (ind == (dst[2] * I * J)+(dst[1] * I) + dst[0]) std::cout << "E";
               else if (found)          std::cout << "x";
               else if (nodes[ind] < 0) std::cout << "O";
               else                     std::cout << ".";
            }
            std::cout << std::endl;
         }
         std::cout << std::endl;
      }
#endif

   }
#endif

   delete[] nodes;
   delete[] cost;
   delete[] path;
   delete[] buffer;

   return 0;
}
