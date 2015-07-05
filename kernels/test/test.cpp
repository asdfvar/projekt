#include <iostream>
#include "linalg.h"

int main()
{

   int x[3] = {7, 11, 123};
   int z = linalg::summation<int> (x, 3);
   std::cout << "sum of ints = " << z << std::endl;

   float a[3] = {3.14159, 2.71828, 1.23456};
   float c = linalg::summation<float> (a, 3);
   std::cout << "sum of floats = " << c << std::endl;

   float u[3] = {1.001, 0.002, 1.012};
   float w[3];

   std::cout << "a dot u = " << linalg::dot_product<float> (a, u, 3) <<
             std::endl;

   linalg::project<float> (u, a, w, 3);
   std::cout << "projection of a onto u is " <<
                w[0] << ", " << w[1] << ", " <<
                w[2] << std::endl;

   return 0;
}
