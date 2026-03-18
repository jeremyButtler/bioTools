/*I am useing this to test if the deletion insert edge
`  function in prim.c
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
   signed int zeroArySI[def_size]; /*zero node edges*/
   signed int oneArySI[def_size];  /*one node edges*/

   signed int siRand = 0;
   signed int siPos = 0;
   signed int siPar = 0;
   signed int checkSI = 0;

   if(! primHeapST)
      return 1;

   /*srand(time(NULL));*/
   for(siRand = 0; siRand < 100; ++siRand)
   { /*Loop: test my array*/
      if(! (siRand % 10) )
         printf("on round %i\n", siRand);

      srand(siRand);
      blank_heap_prim(primHeapST);

      zeroArySI[0] = 0; /*ensures the first half of the
                         `  array always has the lowest
                         `  value
                         */
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: get the edges*/
         zeroArySI[siPos] = rand() % def_size;
         oneArySI[siPos] = rand() % def_size;
      } /*Loop: get the edges*/

      for(siPos = 0; siPos < def_size; ++siPos)
         insEdge_heap_prim(
            zeroArySI[siPos],
            siPos,
            0,
            primHeapST
         );
      siPar = extractEdge_heap_prim(primHeapST);
         /*add pivots*/
      oneArySI[siPar] = (((unsigned int) -1) >> 1);
         /*making sure the selected parent node has the
         `  worst possible value (easier compare later)
         */
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: check if children at correct index*/
         checkSI = primHeapST->indexArySI[siPos];
         checkSI = primHeapST->childArySI[checkSI];
         if(siPos != checkSI)
            printf("%i ins-child mismatch\n", siPos);
      } /*Loop: check if children at correct index*/
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: add new edges in*/
         if(siPos == siPar)
            continue;
         if(oneArySI[siPos] < zeroArySI[siPos])
            delInsEdge_heap_prim(
               oneArySI[siPos],
               siPos,
               0,
               primHeapST
            ); /*add the new edges in*/
      } /*Loop: add new edges in*/
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: check if children at correct index*/
         checkSI = primHeapST->indexArySI[siPos];
         checkSI = primHeapST->childArySI[checkSI];
         if(siPos != checkSI)
            printf("%i del-child mismatch\n", siPos);
      } /*Loop: check if children at correct index*/

            
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: find the smallest edge*/
         if(zeroArySI[siPos] > oneArySI[siPos])
            zeroArySI[siPos] = oneArySI[siPos];
      } /*Loop: find the smallest edge*/

      /*_________find_the_pivots________________________*/
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: remove top entries and find next pivot*/
         siPar = extractEdge_heap_prim(primHeapST);

         checkSI = zeroArySI[siPar];
         if(checkSI != primHeapST->edgeArySI[siPos])
            printf(
               "%i index: %i pos; %i child no match\n",
               siRand,
               siPos,
               siPar
            );
      } /*Loop: remove top entries and find next pivot*/
   } /*Loop: test my array*/

   freeHeap_heap_prim(primHeapST);
   return 0;
} /*main*/
