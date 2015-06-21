#ifndef ASSEMBLY_H
#define ASSEMBLY_H

#include <list>
#include "shape.h"

class Assembly
{

   public:

      Assembly (float center[3]);

      float center[3];

      void insert (sphere *sphere_object);

   private:

      std::list<sphere*> elements;
      std::list<sphere*>::iterator sphere_it;

};

#if 0
template <class T> class Assembly
{

   public:

      float center[3];

      std::list<T> elements;

      template <class T> void insert (T object);

};
#endif

#if 0
// constructing lists
#include <iostream>
#include <list>

int main ()
{
  // constructors used in the same order as described above:
  std::list<int> first;                                // empty list of ints
  std::list<int> second (4,100);                       // four ints with value 100
  std::list<int> third (second.begin(),second.end());  // iterating through second
  std::list<int> fourth (third);                       // a copy of third

  // the iterator constructor can also be used to construct from arrays:
  int myints[] = {16,2,77,29};
  std::list<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

  std::cout << "The contents of fifth are: ";
  for (std::list<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
    std::cout << *it << ' ';

  std::cout << '\n';

  return 0;
}
#endif

#endif
