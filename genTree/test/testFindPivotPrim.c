
/*I am useing this to test if the find pivot function
`  workds for prim.c
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "testPrim.h"
#include "../../genSort/siQuick.h"
#include "../prim.h"

int
main(
   void
){
   #define def_size 100000
   struct heap_prim *primHeapST = mk_heap_prim(def_size);
   signed int checkArySI[def_size];

   signed int siRand = 0;
   signed int siPos = 0;

   if(! primHeapST)
      return 1;

   /*srand(time(NULL));*/
   for(siRand = 0; siRand < 10000; ++siRand)
   { /*Loop: test my array*/
      if(! (siRand % (10000 / 100)) )
         printf("on round %i\n", siRand);

      srand(siRand);
      blank_heap_prim(primHeapST);

      for(siPos = 0; siPos < def_size; ++siPos)
      { /*Loop: setup array and find start and end*/
         primHeapST->edgeArySI[siPos] = rand() % def_size;
         checkArySI[siPos] = primHeapST->edgeArySI[siPos];

         /*these arrays do not effect the quick select*/
         primHeapST->childArySI[siPos] = siPos;
         primHeapST->indexArySI[siPos] = siPos;
      } /*Loop: setup array and find start and end*/

      primHeapST->heapLenSI = def_size;
      sort_siQuick(checkArySI, def_size);
         /*lets me know the order of items to come off*/

      /*_________find_the_pivots________________________*/
      for(siPos = 0; siPos < def_size; ++siPos)
      { /*Loop: remove top entries and find next pivot*/
         findPivots_heap_prim(primHeapST);

         /*this next part simulates an extract step*/
         if(
               primHeapST->edgeArySI[siPos]
            != checkArySI[siPos]
         ) printf("%i index %i no match\n",siRand,siPos);
         ++primHeapST->indexSI; /*move to the next item*/
      } /*Loop: remove top entries and find next pivot*/
   } /*Loop: test my array*/

   freeHeap_heap_prim(primHeapST);
   return 0;
} /*main*/
