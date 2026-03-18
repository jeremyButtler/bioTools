/*here to test if I can output a newick file. I am doing
`  a signle small test here because this file needs to
`  be checked by hand.
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
   #define def_size 10
   struct heap_prim *primHeapST = mk_heap_prim(def_size);
   signed int edgeArySI[def_size]; /*edges*/
   signed int childArySI[def_size]; /*edges*/
   signed char **nameAryStr = 0;
   signed int modSI = def_size * 100;

   signed int siPos = 0;
   signed int numEdgesSI = 0;
   signed int childSI = 0;
   signed int parSI = 0;

   FILE *outFILE = fopen("delete.newick", "w");

   if(! primHeapST)
      return 1;
   nameAryStr = malloc(def_size * sizeof(signed char *));

   srand(1);
   blank_heap_prim(primHeapST);

   nameAryStr[0] = malloc(8 * sizeof(signed char *));
   nameAryStr[0][0] = '0';
   nameAryStr[0][1] = '\0';

   numEdgesSI = 0;
   for(siPos = 1; siPos < def_size; ++siPos)
   { /*Loop: build initial edges*/
      edgeArySI[numEdgesSI] = rand() % modSI;
      childArySI[numEdgesSI] = siPos;
      nameAryStr[siPos] =
         malloc(8 * sizeof(signed char *));
      sprintf((char *) nameAryStr[siPos], "%i", siPos);

      ++numEdgesSI;
   } /*Loop: build initial edges*/

   for(childSI = 0; childSI < def_size; ++childSI)
   { /*Loop: add all children edges*/
      addEdges_heap_prim(
         edgeArySI,
         childArySI,
         numEdgesSI,
         parSI,
         primHeapST
      );
      parSI = extractEdge_heap_prim(primHeapST);

      numEdgesSI = 0;
      for(siPos = 1; siPos < def_size; ++siPos)
      { /*Loop: get the edges*/
         if(primHeapST->indexArySI[siPos] < 0)
            continue; /*edge is a parent*/

         edgeArySI[numEdgesSI] = rand() % modSI;
         childArySI[numEdgesSI] = siPos;

         ++numEdgesSI;
      } /*Loop: get the edges*/
   } /*Loop: add all children edges*/

   root0MstToNewick_heap_prim(
      primHeapST, 
      nameAryStr,
      edgeArySI,
      outFILE
   ); /*print the newick file*/

   for(siPos = 0; siPos < def_size; ++siPos)
      free(nameAryStr[siPos]);
   free(nameAryStr);

   freeHeap_heap_prim(primHeapST);

   fclose(outFILE);
   outFILE = 0;
   return 0;
} /*main*/
