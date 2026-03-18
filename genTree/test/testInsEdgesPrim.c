/*I am useing this to test if the insert edges function
`  works for prim.c
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
   for(siRand = 0; siRand < 100; ++siRand)
   { /*Loop: test my array*/
      if(! (siRand % 10) )
         printf("on round %i\n", siRand);

      srand(siRand);
      blank_heap_prim(primHeapST);

      checkArySI[0] = 0; /*ensures the first half of the
                         `  array always has the lowest
                         `  value
                         */
      for(siPos = 1; siPos < def_size; ++siPos)
         checkArySI[siPos] = rand() % def_size;

         /*lets me know the order of items to come off*/

      for(siPos = 0; siPos < def_size / 2; ++siPos)
         insEdge_heap_prim(
            checkArySI[siPos],
            siPos,
            0,
            primHeapST
         );
      extractEdge_heap_prim(primHeapST); /*add pivots*/
      for( ; siPos < def_size; ++siPos)
         insEdge_heap_prim(
            checkArySI[siPos],
            siPos,
            0,
            primHeapST
         );
            
      sort_siQuick(checkArySI, def_size);

      /*_________find_the_pivots________________________*/
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: remove top entries and find next pivot*/
         extractEdge_heap_prim(primHeapST);

         if(
               primHeapST->edgeArySI[siPos]
            != checkArySI[siPos]
         ) printf("%i index %i no match\n",siRand,siPos);
      } /*Loop: remove top entries and find next pivot*/
   } /*Loop: test my array*/

   freeHeap_heap_prim(primHeapST);
   return 0;
} /*main*/
