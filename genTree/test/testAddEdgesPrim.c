/*I am useing this to test if the addEdges prim.c function
`  works. Not the best, but should work (maybe)
`  I reduced the test case size because the times take
`  so long
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
   #define def_size 25000
   /*#define def_size 1000*/
   struct heap_prim *primHeapST = mk_heap_prim(def_size);
   signed int zeroArySI[def_size]; /*zero node edges*/
   signed int oneArySI[def_size];  /*one node edges*/
   signed int childArySI[def_size]; /*children*/
   signed int modSI = def_size * 100;

   signed int siRand = 0;
   signed int siPos = 0;
   signed int numEdgesSI = 0;
   signed int checkSI = 0;
   signed int childSI = 0;

   if(! primHeapST)
      return 1;

   /*for(siRand = 0; siRand < 1; ++siRand)*/
   for(siRand = 0; siRand < 100; ++siRand)
   { /*Loop: test my array*/
      if(! (siRand % 10) )
         printf("on round %i\n", siRand);

      srand(siRand);
      blank_heap_prim(primHeapST);

      numEdgesSI = 0;
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: build the parent node*/
         zeroArySI[numEdgesSI] = rand() % modSI;
         oneArySI[numEdgesSI] = zeroArySI[numEdgesSI];
         childArySI[numEdgesSI] = siPos;
         ++numEdgesSI;
      } /*Loop: build the parent node*/

      for(childSI = 0; childSI < def_size; ++childSI)
      { /*Loop: add all children edges*/
         addEdges_heap_prim(
            oneArySI,
            childArySI,
            numEdgesSI,
            childSI,
            primHeapST
         );
         extractEdge_heap_prim(primHeapST);

         numEdgesSI = 0;
         for(siPos = 1; siPos < def_size; ++siPos)
         { /*Loop: get the edges*/
            if(primHeapST->indexArySI[siPos] < 0)
               continue; /*edge is a parent*/

            oneArySI[numEdgesSI] = rand() % modSI;
            childArySI[numEdgesSI] = siPos;

            if(zeroArySI[siPos -1] > oneArySI[numEdgesSI])
               zeroArySI[siPos -1] = oneArySI[numEdgesSI];
            ++numEdgesSI;
         } /*Loop: get the edges*/
      } /*Loop: add all children edges*/

      /*_________check_for_mistakes_____________________*/
         /*I need the -1 to account for my tree treating
         `  the first node as a root, so I have one less
         `  node then def_size
         */
      for(siPos = 1; siPos < def_size - 1; ++siPos)
      { /*Loop: remove top entries and find next pivot*/
         childSI = primHeapST->childArySI[siPos];
         checkSI = zeroArySI[childSI - 1];

         if(checkSI != primHeapST->edgeArySI[siPos])
         { /*If: the tree is not correct*/
            printf(
               "%i index: %i pos; %i child no match\n",
               siRand,
               siPos,
               childSI
            );
            goto err_main;
         } /*If: the tree is not correct*/
      } /*Loop: remove top entries and find next pivot*/
   } /*Loop: test my array*/

   
   freeHeap_heap_prim(primHeapST);
   return 0;

   err_main:;
      freeHeap_heap_prim(primHeapST);
      return 1;
} /*main*/
