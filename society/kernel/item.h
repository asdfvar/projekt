#ifndef ITEM_H
#define ITEM_H

namespace tid
{
   const unsigned int dirt  = 1;
   const unsigned int stone = 2;
};

class Item
{
   public:
      Item (unsigned int type_in)
      {
         type = type_in;

         if (type == tid::stone) weight = 1.0f;
         else weight = 0.0f;
      };

   private:
      unsigned int   type;
      float weight;
};

#endif
