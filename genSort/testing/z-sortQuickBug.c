/*my goal is to debug my quick select with this script*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include <time.h>

#include "../siQuick.h"

int
main(
   void
){
   #define def_size 100000
   signed int siAry[def_size];

   signed int siRand = 0;
   signed int siPos = 0;

   /*srand(time(NULL));*/
   for(siRand = 0; siRand < 10000; ++siRand)
   { /*Loop: test my array*/
      srand(siRand);

      for(siPos = 0; siPos < def_size; ++siPos)
         siAry[siPos] = rand() % def_size;
      sort_siQuick(siAry, def_size);

      for(siPos = 0; siPos < def_size - 1; ++siPos)
      { /*Loop: check for errors*/
         if(siAry[siPos] > siAry[siPos + 1])
         { /*If: the array was not sorted correctly*/
            printf(
               "%i failed to sort at %i\n",
               siRand,
               siPos
            );
            break;
         } /*If: the array was not sorted correctly*/
      } /*Loop: check for errors*/
   } /*Loop: test my array*/

   return 0;
} /*main*/
