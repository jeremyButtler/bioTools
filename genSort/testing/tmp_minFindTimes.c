/*
The goal here was to test if the branchless swapping
  methods was better for finding the minimum values. In
  this case branched (the expected outcome) was faster
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include <time.h>

int
main(
   void
){
   #define def_size 100000
   signed int savedAry[def_size];
   signed int siAry[def_size];

   signed int posSI = 0;
   signed int swapSI = 0;
   clock_t start;
   clock_t end;

   /*__________fill_array_with_random_numbers___________*/
   srand(time(NULL)); /*set seed so always random*/

   for(posSI = 0; posSI < def_size; ++posSI)
   { /*Loop: build the array to swap*/
      savedAry[posSI] = rand();
      siAry[posSI] = savedAry[posSI];
   } /*Loop: build the array to swap*/

   /*_____________branchless_swap_test__________________*/
   start = clock();
   for(posSI = 0; posSI < def_size; ++posSI)
   { /*Loop: swap elements*/
      swapSI = -(siAry[0] > siAry[posSI]);
      siAry[0] ^= (siAry[posSI] & swapSI);
      siAry[posSI] ^= (siAry[0] & swapSI);
      siAry[0] ^= (siAry[posSI] & swapSI);
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branchless__________: %f\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siAry[0]
   );

   /*_____________if_statement_swap_test________________*/
   start = clock();
   for(posSI = 0; posSI < def_size; ++posSI)
   { /*Loop: swap elements*/
      if(savedAry[0] > savedAry[posSI])
      { /*If: swapping*/
         savedAry[0] ^= savedAry[posSI];
         savedAry[posSI] ^= savedAry[0];
         savedAry[0] ^= savedAry[posSI];
      } /*If: swapping*/
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branched____________: %f\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      savedAry[0]
   );

   return 0;
} /*main*/
