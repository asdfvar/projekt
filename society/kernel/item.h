#ifndef ITEM_H
#define ITEM_H

namespace mid
{
   const unsigned int dirt  = 1;
   const unsigned int grass = 2;
   const unsigned int stone = 3;
};

class Item
{
   public:
      Item (unsigned int type_in)
      {
         type = type_in;

         if (type == mid::stone) weight = 1.0f;
         else weight = 0.0f;
      };

   private:
      unsigned int   type;
      float weight;
};

#endif
