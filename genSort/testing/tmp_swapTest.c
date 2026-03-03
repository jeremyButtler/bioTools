/*
The goal here was to test if the branchless swapping
  methods was better for times on my cpu for random
  input. It was faster
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
   signed int savedSecAry[def_size];
   signed int savedThirdAry[def_size];

   signed int siAry[def_size];
   signed int siSecAry[def_size];
   signed int siThirdAry[def_size];

   signed int posSI = 0;
   signed int swapSI = 0;
   clock_t start;
   clock_t end;

   /*__________fill_array_with_random_numbers___________*/
   srand(time(NULL)); /*set seed so always random*/

   for(posSI = 0; posSI < def_size; ++posSI)
   { /*Loop: build the array to swap*/
      savedAry[posSI] = rand();
      savedSecAry[posSI] = rand();
      savedThirdAry[posSI] = rand();
      siAry[posSI] = savedAry[posSI];
      siSecAry[posSI] = savedSecAry[posSI];
      siThirdAry[posSI] = savedThirdAry[posSI];
   } /*Loop: build the array to swap*/

   /*_____________branchless_swap_test__________________*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      swapSI = -(siAry[posSI] > siAry[posSI + 1]);
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);
      siAry[posSI + 1] ^= (siAry[posSI] & swapSI);
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branchless__________: %f\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siAry[0]
   );

   /*_____________if_statement_swap_test________________*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      if(savedAry[posSI] > savedAry[posSI + 1])
      { /*If: swapping*/
         savedAry[posSI] ^= savedAry[posSI + 1];
         savedAry[posSI + 1] ^= savedAry[posSI];
         savedAry[posSI] ^= savedAry[posSI + 1];
      } /*If: swapping*/
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branched____________: %f\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      savedAry[0]
   );

   /*_____________branchless_sync_swap_test_____________*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      swapSI = -(siSecAry[posSI] > siSecAry[posSI + 1]);
      siSecAry[posSI] ^= (siSecAry[posSI + 1] & swapSI);
      siSecAry[posSI + 1] ^= (siSecAry[posSI] & swapSI);
      siSecAry[posSI] ^= (siSecAry[posSI + 1] & swapSI);

      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);
      siAry[posSI + 1] ^= (siAry[posSI] & swapSI);
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branchless_dual_swap: %f\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siSecAry[0],
      siAry[0]
   );

   /*_____________if_statement_sync_swap_test___________*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      if(savedSecAry[posSI] > savedSecAry[posSI + 1])
      { /*If: swapping*/
         savedSecAry[posSI] ^= savedSecAry[posSI + 1];
         savedSecAry[posSI + 1] ^= savedSecAry[posSI];
         savedSecAry[posSI] ^= savedSecAry[posSI + 1];

         savedAry[posSI] ^= savedAry[posSI + 1];
         savedAry[posSI + 1] ^= savedAry[posSI];
         savedAry[posSI] ^= savedAry[posSI + 1];
      } /*If: swapping*/
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branched_dual_swap__: %f\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      savedSecAry[0],
      savedAry[0]
   );

   /*_____________branchless_index_swap_sync_test_______*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      /*the swap*/
      swapSI = -(siThirdAry[posSI] > siThirdAry[posSI+1]);
      siThirdAry[posSI] ^= (siThirdAry[posSI +1] &swapSI);
      siThirdAry[posSI +1] ^= (siThirdAry[posSI] &swapSI);
      siThirdAry[posSI] ^= (siThirdAry[posSI +1] &swapSI);

      /*the sync*/
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);
      siAry[posSI + 1] ^= (siAry[posSI] & swapSI);
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);

      /*the index shift*/
      siSecAry[posSI] =
         (posSI & ~swapSI) | ((posSI + 1) & swapSI);
      siSecAry[posSI + 1] =
         (posSI & swapSI) | ((posSI + 1) & ~swapSI);
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branchless_indexswap: %f\t%i\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siThirdAry[0],
      siAry[0],
      siSecAry[0]
   );

   /*_____________if_statement_index_sync_swap_test_____*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      if(savedThirdAry[posSI] > savedThirdAry[posSI + 1])
      { /*If: swapping*/
         /*the swap*/
         savedThirdAry[posSI] ^= savedThirdAry[posSI +1];
         savedThirdAry[posSI +1] ^= savedThirdAry[posSI];
         savedThirdAry[posSI] ^= savedThirdAry[posSI +1];

         /*the sync*/
         savedAry[posSI] ^= savedAry[posSI + 1];
         savedAry[posSI + 1] ^= savedAry[posSI];
         savedAry[posSI] ^= savedAry[posSI + 1];

         /*the index shift*/
         savedSecAry[posSI] = posSI + 1;
         savedSecAry[posSI + 1] = posSI;
      } /*If: swapping*/
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branched_index_swap_: %f\t%i\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      savedThirdAry[0],
      savedAry[0],
      savedSecAry[0]
   );

   /*____ordered__branchless_index_swap_sync_test_______*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      /*the swap*/
      swapSI = -(siThirdAry[posSI] > siThirdAry[posSI+1]);
      siThirdAry[posSI] ^= (siThirdAry[posSI +1] &swapSI);
      siThirdAry[posSI +1] ^= (siThirdAry[posSI] &swapSI);
      siThirdAry[posSI] ^= (siThirdAry[posSI +1] &swapSI);

      /*the sync*/
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);
      siAry[posSI + 1] ^= (siAry[posSI] & swapSI);
      siAry[posSI] ^= (siAry[posSI + 1] & swapSI);

      /*the index shift*/
      siSecAry[posSI] =
         (posSI & ~swapSI) | ((posSI + 1) & swapSI);
      siSecAry[posSI + 1] =
         (posSI & swapSI) | ((posSI + 1) & ~swapSI);
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branchless_indexordr: %f\t%i\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siThirdAry[0],
      siAry[0],
      siSecAry[0]
   );

   /*____ordered__if_statement_index_sync_swap_test_____*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      if(savedThirdAry[posSI] > savedThirdAry[posSI + 1])
      { /*If: swapping*/
         /*the swap*/
         savedThirdAry[posSI] ^= savedThirdAry[posSI +1];
         savedThirdAry[posSI +1] ^= savedThirdAry[posSI];
         savedThirdAry[posSI] ^= savedThirdAry[posSI +1];

         /*the sync*/
         savedAry[posSI] ^= savedAry[posSI + 1];
         savedAry[posSI + 1] ^= savedAry[posSI];
         savedAry[posSI] ^= savedAry[posSI + 1];

         /*the index shift*/
         savedSecAry[posSI] = posSI + 1;
         savedSecAry[posSI + 1] = posSI;
      } /*If: swapping*/
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branched_index_order: %f\t%i\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      savedThirdAry[0],
      savedAry[0],
      savedSecAry[0]
   );

   /*__ordered_swap_if_statement_index_sync_swap_test___*/
   start = clock();
   for(posSI = 0; posSI < def_size - 1; posSI += 2)
   { /*Loop: swap elements*/
      if(savedThirdAry[posSI] < savedThirdAry[posSI + 1])
      { /*If: swapping*/
         /*the swap*/
         savedThirdAry[posSI] ^= savedThirdAry[posSI +1];
         savedThirdAry[posSI +1] ^= savedThirdAry[posSI];
         savedThirdAry[posSI] ^= savedThirdAry[posSI +1];

         /*the sync*/
         savedAry[posSI] ^= savedAry[posSI + 1];
         savedAry[posSI + 1] ^= savedAry[posSI];
         savedAry[posSI] ^= savedAry[posSI + 1];

         /*the index shift*/
         savedSecAry[posSI] = posSI + 1;
         savedSecAry[posSI + 1] = posSI;
      } /*If: swapping*/
   } /*Loop: swap elements*/

   end = clock();
   printf(
      "branched_index_order: %f\t%i\t%i\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      savedThirdAry[0],
      savedAry[0],
      savedSecAry[0]
   );

   return 0;
} /*main*/
