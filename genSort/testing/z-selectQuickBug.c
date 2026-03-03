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
   signed int binArySI[def_size];

   signed int siRand = 0;
   signed int siPos = 0;
   signed int siCnt = 0;

   signed int startSI = def_size + 1;
   signed int midSI = 0;
   signed int endSI = 0;

   signed int foundSI = 0;

   /*srand(time(NULL));*/
   for(siRand = 0; siRand < 10000; ++siRand)
   { /*Loop: test my array*/
      srand(siRand);

      startSI = def_size + 1;
      endSI = 0;
      midSI = 0;

      for(siPos = 0; siPos < def_size; ++siPos)
         binArySI[siPos] = 0; /*counter*/

      for(siPos = 0; siPos < def_size; ++siPos)
      { /*Loop: setup array and find start and end*/
         siAry[siPos] = rand() % def_size;
         ++binArySI[siAry[siPos]];

         if(siAry[siPos] > endSI)
            endSI = siAry[siPos]; /*last value*/
         if(siAry[siPos] < startSI)
            startSI = siAry[siPos]; /*last value*/
      } /*Loop: setup array and find start and end*/

      siPos = 0;
      siCnt = 0;
      while(siCnt < def_size / 2)
      { /*Loop: find the midpiont value*/
         if(binArySI[siPos] > 0)
            midSI = siPos;
         siCnt += binArySI[siPos++];
      } /*Loop: find the midpiont value*/

      /*_________run_quick_select_only_errors_printed___*/
      foundSI =
         select_siQuick(siAry, (def_size/2) -1, def_size);
      if(midSI != foundSI)
         printf("%i mid: %i=%i\n",siRand,foundSI,midSI);

      foundSI = select_siQuick(siAry, 0, def_size);
      if(startSI != foundSI)
       printf("%i start: %i=%i\n",siRand,foundSI,startSI);

      foundSI =
         select_siQuick(siAry, def_size -1, def_size);
      if(endSI != foundSI)
         printf("%i end: %i=%i\n",siRand,foundSI,endSI);
   } /*Loop: test my array*/

   return 0;
} /*main*/
