/*use this to test if the quick select algorithm is
`  working in prim.c
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "testPrim.h"
#include "../prim.h"

int
main(
   void
){
   #define def_size 20
   struct heap_prim *primHeapST = mk_heap_prim(def_size);
   signed int binArySI[def_size];
   signed int checkArySI[def_size];
   signed int checkIndexArySI[def_size];

   signed int siRand = 0;
   signed int siPos = 0;
   signed int siCnt = 0;

   signed int startSI = def_size + 1;
   signed int midSI = 0;
   signed int endSI = 0;

   signed int foundSI = 0;

   if(! primHeapST)
      return 1;

   /*srand(time(NULL));*/
   for(siRand = 0; siRand < 1; ++siRand)
   { /*Loop: test my array*/
      srand(siRand);

      startSI = def_size + 1;
      endSI = 0;
      midSI = 0;

      for(siPos = 0; siPos < def_size; ++siPos)
         binArySI[siPos] = 0; /*counter*/

      for(siPos = 0; siPos < def_size; ++siPos)
      { /*Loop: setup array and find start and end*/
         primHeapST->edgeArySI[siPos] = rand() % def_size;
         checkArySI[siPos] = primHeapST->edgeArySI[siPos];
         ++binArySI[primHeapST->edgeArySI[siPos]];

         /*these arrays do not effect the quick select*/
         primHeapST->childArySI[siPos] = siPos;
         primHeapST->indexArySI[siPos] = siPos;
         checkIndexArySI[siPos] = siPos;

         if(primHeapST->edgeArySI[siPos] > endSI)
            endSI = primHeapST->edgeArySI[siPos];
         if(primHeapST->edgeArySI[siPos] < startSI)
            startSI = primHeapST->edgeArySI[siPos];
      } /*Loop: setup array and find start and end*/

      primHeapST->heapLenSI = def_size;

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
        qckSelect_heap_prim(
           primHeapST,
           0,
           primHeapST->heapLenSI - 1,
           0
      ); /*find the midpiont not start*/

      for(siPos = 0; siPos < def_size; ++siPos)
      { /*Loop: check if everything still matches*/
         /*make sure the edge weight and children are
         `  still in sync
         */
         siCnt = primHeapST->childArySI[siPos];
         if(
               checkArySI[siCnt]
            != primHeapST->edgeArySI[siPos]
         ) printf("child does not agree with edge\n");

         /*the index should access the same edge weight
         `  at the child (original) index
         */
         siCnt = primHeapST->indexArySI[siPos];
         if(
               checkArySI[siPos]
            != primHeapST->edgeArySI[siCnt]
         ) printf(
              "index %i does not agree with edge\n",
              siPos
           );
      } /*Loop: check if everything still matches*/

      /*so I can keep the checkIndexArySI*/
      printf("%i\n", checkIndexArySI[0]);
      foundSI = primHeapST->edgeArySI[foundSI];
      if(midSI != foundSI)
         printf("%i mid: %i=%i\n",siRand,foundSI,midSI);
   } /*Loop: test my array*/

   freeHeap_heap_prim(primHeapST);
   return 0;
} /*main*/
