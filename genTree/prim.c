/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' prim SOF: Start Of File
'   - uses prims algorithm to find a minimum spanning tree
'   o header:
'     - included libraries
'   o .h st01: heap_prim
'     - has variables to run a quick heap for prims MST
'   o fun01: blank_heap_prim
'     - blanks a heap_prim structure to null (somewhat
'       lazy)
'   o fun02: init_heap_prim
'     - initializes a heap_prim structure
'   o fun03: freeStack_heap_prim
'     - frees all variables in a heap_prim structure
'   o fun04: freeHeap_heap_prim
'     - frees heap_prim structure
'   o fun05: setupMem_heap_prim
'     - adds memory to a heap_prim structure
'   o fun06: mk_heap_prim
'     - makes and sets up a heap allocated heap_prim
'   o .c fun07: qckSelect_heap_prim
'     - finds the tartget pivot in a heap_prim struct
'   o .c fun08: findPivots_heap_prim
'     - find the pivots in the quick heap
'   o .c fun09: delInsEdge_heap_prim
'     - deletes edge and then insert the better edge for
'       the same child node into a heap_prim
'   o .c fun10: insEdge_heap_prim
'     - inset an edge into my prim quick heap
'   o fun11: addEdges_heap_prim
'     - add edges for a node to a heap_prim struct
'   o fun12: extractEdge_heap_prim
'     - extract the minimum edge from a heap_prim struct
'   o .c fun13: threeArySort_prim
'     - sorts a signed int array from least to greatest
'       and keeps two other arrays in sync
'   o .c fun14: threeAryUnsort_prim
'     - unsorts three arrays using the first input array
'   o .c fun15: mstToNewickRecusive_heap_prim
'     - the recurisve part to save a mst as a newick file
'     - this function should only be called by
'       mstToNewick_heap_prim; do not call it
'   o fun16: mstToNewick_heap_prim
'     - saves a minimum spanning tree as a newick file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\_
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h> /*reading in files*/

#include "prim.h"
#include "../genSort/siBinSearch.h"
#include "../genLib/genMath.h"

/*.h files*/
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*I originally wanted a midpoint method. However, it got
`  to complex to quickly and so I never got it working.
`  I am leaving the code here in case I want to revive it
*/
#ifdef MIDPIONT_NEWICK_PRIM
/*-------------------------------------------------------\
| ST02: dist_prim
|   - stores the distance to a cluster
\-------------------------------------------------------*/
typedef struct
dist_prim{
   float distF;               /*distance to child node*/
   signed int childSI;
      /*child node to add or -1 if is an internal node*/
   signed int firstChildIndexSI;
      /*index of the first child node*/

   struct dist_prim *nextPtr;
      /*idea is to keep the first struct with the lowest
      `  value, and do not bother sorting the rest. This
      `  allows me to find the lowest midpiont
      */
}dist_prim;
#endif

/*-------------------------------------------------------\
| Fun01: blank_heap_prim
|   - blanks a heap_prim structure to null (somewhat
|     lazy)
| Input:
|   - primSTPtr:
|     o pointer to a heap_prim struct to blank
| Output:
|   - sets indexSI, pivotLenSI, heapLenSI, and scoreSI
|     (lazy blank)
|   - sets all values used in indexArySI to def_maxSI_prim
\-------------------------------------------------------*/
void
blank_heap_prim(
   struct heap_prim *primSTPtr
){
   signed int siPos = 0;

   if(! primSTPtr)
      return;
   primSTPtr->pivotLenSI = 0;
   primSTPtr->heapLenSI = 0;
   primSTPtr->scoreSI = 0;

   if(primSTPtr->indexArySI)
   { /*If: I need to reset all index's to none*/
      for(siPos = 0; siPos < primSTPtr->indexSI; ++siPos)
      { /*Loop: initialize index and parent nodes*/
         primSTPtr->indexArySI[siPos] = def_maxSI_prim;
         primSTPtr->parArySI[siPos] = -1;
      } /*Loop: initialize index and parent nodes*/

      if(primSTPtr->indexSI < primSTPtr->sizeSI)
      { /*If: need to blank the last possible value*/
         primSTPtr->indexArySI[ primSTPtr->indexSI ] =
            def_maxSI_prim;
         primSTPtr->parArySI[ primSTPtr->indexSI ] = -1;
      } /*If: need to blank the last possible value*/
   } /*If: I need to reset all index's to none*/

   primSTPtr->indexSI = 0; /*at first index in heap*/
} /*blank_heap_prim*/

/*-------------------------------------------------------\
| Fun02: init_heap_prim
|   - initializes a heap_prim structure
| Input:
|   - primSTPtr:
|     o pointer to a heap_prim struct to initialize
| Output:
|   - sets all pointers and values to 0
\-------------------------------------------------------*/
void
init_heap_prim(
   struct heap_prim *primSTPtr
){
   if(! primSTPtr)
      return;
   primSTPtr->edgeArySI = 0;
   primSTPtr->childArySI = 0;

   primSTPtr->pivotArySI = 0;
   primSTPtr->pivotSizeSI = 0;

   primSTPtr->indexArySI = 0;
   primSTPtr->parArySI = 0;

   primSTPtr->sizeSI = 0;

   blank_heap_prim(primSTPtr);
} /*init_heap_prim*/

/*-------------------------------------------------------\
| Fun03: freeStack_heap_prim
|   - frees all variables in a heap_prim structure
| Input:
|   - primSTPtr
|     o pointer to a heap_prim struct with variables to
|       free
| Output:
|   - Modifies:
|     o primSTPtr to have everything freeded and to be
|       initialized
\-------------------------------------------------------*/
void
freeStack_heap_prim(
   struct heap_prim *primSTPtr
){
   if(! primSTPtr)
      return;

   if(primSTPtr->edgeArySI)
      free(primSTPtr->edgeArySI);
   if(primSTPtr->childArySI)
      free(primSTPtr->childArySI);

   if(primSTPtr->pivotArySI)
      free(primSTPtr->pivotArySI);

   if(primSTPtr->indexArySI)
      free(primSTPtr->indexArySI);
   if(primSTPtr->parArySI)
      free(primSTPtr->parArySI);

   init_heap_prim(primSTPtr);
} /*freeStack_heap_prim*/

/*-------------------------------------------------------\
| Fun04: freeHeap_heap_prim
|   - frees heap_prim structure
| Input:
|   - primSTPtr
|     o pointer to a heap_prim struct to free
| Output:
|   - Modifies:
|     o frees primSTPtr (you need to set the pointer to
|       0/null)
\-------------------------------------------------------*/
void
freeHeap_heap_prim(
   struct heap_prim *primSTPtr
){
   if(! primSTPtr)
      return;
   freeStack_heap_prim(primSTPtr);
   free(primSTPtr);
} /*freeHeap_heap_prim*/

/*-------------------------------------------------------\
| Fun05: setupMem_heap_prim
|   - adds memory to a heap_prim structure
| Input:
|   - primSTPtr
|     o pointer to a heap_prim struct to add memory to
|   - numNodesSI:
|     o number of nodes in the tree
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_memErr_prim for memory errors
\-------------------------------------------------------*/
signed char
setupMem_heap_prim(
   struct heap_prim *primSTPtr,
   signed int numNodesSI
){
   signed int siPos = 0;

   /*___________check_if_can_add_memory_________________*/
   if(! primSTPtr)
      goto nothingToDo_fun05;
   else if(numNodesSI <= 0)
      goto nothingToDo_fun05;
   else if(primSTPtr->sizeSI > numNodesSI)
      goto nothingToDo_fun05; /*have enough memory*/

   /*make sure no allocated memory in the heap_prim*/
   freeStack_heap_prim(primSTPtr);

   /*______________set_the_size_variables_______________*/
   primSTPtr->sizeSI = numNodesSI;

   /*need log2(n) pivots*/
   primSTPtr->pivotSizeSI = hibit2_genMath(numNodesSI);
   ++primSTPtr->pivotSizeSI;
      /*account for being one log off (maybe)*/

   /*______________allocate_memory______________________*/
   primSTPtr->edgeArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primSTPtr->edgeArySI)
      goto memErr_fun05;

   primSTPtr->childArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primSTPtr->childArySI)
      goto memErr_fun05;

   primSTPtr->pivotArySI =
     malloc(primSTPtr->pivotSizeSI * sizeof(signed int));
   if(! primSTPtr->pivotArySI)
      goto memErr_fun05;

   primSTPtr->parArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primSTPtr->parArySI)
      goto memErr_fun05;

   primSTPtr->indexArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primSTPtr->indexArySI)
      goto memErr_fun05;

   for(siPos = 0; siPos < numNodesSI; ++siPos)
   { /*Loop: set parent and index to no node*/
      primSTPtr->indexArySI[siPos] = def_maxSI_prim;
      primSTPtr->parArySI[siPos] = def_maxSI_prim;
   } /*Loop: set parent and index to no node*/

   /*________________return_the_result__________________*/
   return 0;

   memErr_fun05:;
      freeStack_heap_prim(primSTPtr);
      return def_memErr_prim;

   nothingToDo_fun05:;
      if(! primSTPtr)
         blank_heap_prim(primSTPtr);
      return 0;
} /*setupMem_heap_prim*/

/*-------------------------------------------------------\
| Fun06: mk_heap_prim
|   - makes and sets up a heap allocated heap_prim
| Input:
|   - numNodesSI:
|     o number of nodes in your graph (and MST)
| Output:
|   - Returns:
|     o pointer to a setup heap_prim structure
|     o 0 for memory errors
\-------------------------------------------------------*/
struct heap_prim *
mk_heap_prim(
   signed int numNodesSI
){
   struct heap_prim *retHeapPtr = 0;

   retHeapPtr = malloc(sizeof(struct heap_prim));
   if(! retHeapPtr)
      goto memErr_fun06;
   init_heap_prim(retHeapPtr);

   if( setupMem_heap_prim(retHeapPtr, numNodesSI) )
      goto memErr_fun06;

   goto return_fun06;

   memErr_fun06:;
      if(retHeapPtr)
         freeHeap_heap_prim(retHeapPtr);
      retHeapPtr = 0;
      goto return_fun06;

   return_fun06:;
      return retHeapPtr;
} /*mk_heap_prim*/

/*-------------------------------------------------------\
| Fun07: qckSelect_heap_prim
|   - finds the tartget pivot in a heap_prim struct
| Input:
|   - primSTPtr:
|     o heap_prim struct to find the pivot for
|   - startSI:
|     o first index (index 0) to search for the pivot at
|   - endSI:
|     o last index (index 0) to search for the pivot at
|   - startPivotBl:
|     o 1: find the starting pivot (lowest value)
|     o 0: find the midpoint in the range
| Output:
|   - Modifies:
|     o edgeArySI and childArySI arrays in primSTPtr
|       to be paritallys sorted for the first pivot
|   - Returns:
|     o index of the pivot position
\-------------------------------------------------------*/
signed int
qckSelect_heap_prim(
   struct heap_prim *primSTPtr,
   signed int startSI,
   signed int endSI,
   signed char startPivotBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - finds the tartget pivot in a heap_prim struct
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - check minimum value case and find heap midpoint
   '   o fun07 sec03:
   '     - deal with cases were there are two few elements
   '   o fun07 sec04:
   '     - find the kth value
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int midSI = 0;
   signed int swapSI = 0;
   signed int kthSI = 0;
   signed int posSI = 0; /*for the sort loop*/

   /*for my sanity; it should be compiled out*/
   signed int *edgeArySI = primSTPtr->edgeArySI;
   signed int *childArySI = primSTPtr->childArySI;
   signed int *indexArySI = primSTPtr->indexArySI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - check minimum value case and find heap midpoint
   ^   o fun07 sec02 sub01:
   ^     - find minimum value case
   ^   o fun07 sec02 sub02:
   ^     - find midpiont in heap
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec02 Sub01:
   *   - deal with find minimum value case
   \*****************************************************/

   if(startPivotBl == 1)
   { /*If: finding the minimum value*/
      /*I do not have to jump around to find the first
      `  pivot, this should be faster
      */
      for(midSI = startSI; midSI <= endSI; ++midSI)
      { /*Loop: find the minimum value*/
         /*unlike swapping, finding the minimum value
         `  is faster branched in a random case
         `  (see tmp_minFindTimes.c in genSort/test
         */
         if(edgeArySI[midSI] < edgeArySI[startSI])
         { /*If: need to swap*/
            indexArySI[ childArySI[startSI] ] = midSI;
            indexArySI[ childArySI[midSI] ] = startSI;

            edgeArySI[startSI] ^= edgeArySI[midSI];
            edgeArySI[midSI] ^= edgeArySI[startSI];
            edgeArySI[startSI] ^= edgeArySI[midSI];

            childArySI[startSI] ^= childArySI[midSI];
            childArySI[midSI] ^= childArySI[startSI];
            childArySI[startSI] ^= childArySI[midSI];
         } /*If: need to swap*/
      } /*Loop: find the minimum value*/

      return startSI;
   } /*If: finding the minimum value*/

   /*****************************************************\
   * Fun07 Sec02 Sub02:
   *   - find midpiont in heap
   \*****************************************************/

   else
      kthSI = (endSI + startSI) >> 1;
      /*else find the midpiont*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - deal with cases were there are two few elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(endSI - startSI < 2)
   { /*If: so few items it is easy to find the kth item*/
      if(midSI - startSI <= 0)
         ;

      else
      { /*Else If: two values*/
         swapSI=-(edgeArySI[startSI] > edgeArySI[endSI]);
         indexArySI[ childArySI[startSI] ] =
            (startSI & ~swapSI) | (endSI & swapSI);
         indexArySI[ childArySI[endSI] ] =
            (startSI & swapSI) | (endSI & ~swapSI);
            /*Sec02 Logic01:
            `   - startSI & swapSI:
            `     o goes to 0 if not swaping index's
            `     o goes to 1 if swaping index's
            `   - endSI & ~swapSI:
            `     o goes to 1 if not swaping index's
            `     o goes to 0 if swaping index's
            `   - (startSI & swapSI) | (endSI & ~swapSI)
            `     o goes to (0 | endSI) = endSI if not
            `       swaping index's
            `     o goes to (startSI | 0) = startSI if
            `       swaping index's
            */

         edgeArySI[startSI] ^= (edgeArySI[endSI] &swapSI);
         edgeArySI[endSI] ^= (edgeArySI[startSI] &swapSI);
         edgeArySI[startSI] ^= (edgeArySI[endSI] &swapSI);

         childArySI[startSI]^=(childArySI[endSI] &swapSI);
         childArySI[endSI]^=(childArySI[startSI] &swapSI);
         childArySI[startSI]^=(childArySI[endSI] &swapSI);
            /*Sec02 Logic02:
            `   - swapSI = -(edgeArySI[x] > edgeArySI[y])
            `     o becomes -(1) = -1 if
            `       edgeArySI[x] > edgeArySI[y]
            `       * all bits set to 1
            `     o becomes -(0) = 0 if
            `       edgeArySI[x] < edgeArySI[y]
            `       * all bits set to 0
            `   - edgeArySI[y or x] & swapSI:
            `     o value is kept if swapSI = -1
            `     o set to 0 if swapSI = 0
            `   - the rest of the logic is a regular swap
            `     using xors from the standford
            `     bithacking guide
            */
      } /*Else If: two values*/

      return kthSI;
   } /*If: so few items it is easy to find the kth item*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - find the kth value
   ^   o fun07 sec04 sub01:
   ^     - find the median value using the three median
   ^       method + start loop
   ^   o fun07 sec04 sub02:
   ^     - mvoe midpoint to the last item in the array
   ^   o fun07 sec04 sub03:
   ^     - paritally sort array to find midpoint position
   ^   o fun07 sec04 sub04:
   ^     - move the midpoint to its actual position
   ^   o fun07 sec04 sub05:
   ^     - check if found the kth value
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec04 Sub01:
   *   - find the median value using the three median
   *     method + start loop
   \*****************************************************/

   while(startSI < endSI - 1)
   { /*Loop: find kth value and partially sort*/
      midSI = (endSI + startSI) >> 1;

      /*_________find_the_midpoint_using_3_medians______*/
      /*find the logic for this see sec02 logic01 and
      `   sec02 logic02
      */

      /*_________compare_start_and_end__________________*/
      swapSI = -(edgeArySI[startSI] > edgeArySI[endSI]);
      indexArySI[ childArySI[startSI] ] =
         (startSI & ~swapSI) | (endSI & swapSI);
      indexArySI[ childArySI[endSI] ] =
         (startSI & swapSI) | (endSI & ~swapSI);

      edgeArySI[startSI] ^= (edgeArySI[endSI] & swapSI);
      edgeArySI[endSI] ^= (edgeArySI[startSI] & swapSI);
      edgeArySI[startSI] ^= (edgeArySI[endSI] & swapSI);

      childArySI[startSI] ^= (childArySI[endSI] & swapSI);
      childArySI[endSI] ^= (childArySI[startSI] & swapSI);
      childArySI[startSI] ^= (childArySI[endSI] & swapSI);

      /*_________compare_mid_and_start__________________*/
      swapSI = -(edgeArySI[startSI] > edgeArySI[midSI]);
      indexArySI[ childArySI[startSI] ] =
         (startSI & ~swapSI) | (midSI & swapSI);
      indexArySI[ childArySI[midSI] ] =
         (startSI & swapSI) | (midSI & ~swapSI);

      edgeArySI[startSI] ^= (edgeArySI[midSI] & swapSI);
      edgeArySI[midSI] ^= (edgeArySI[startSI] & swapSI);
      edgeArySI[startSI] ^= (edgeArySI[midSI] & swapSI);

      childArySI[startSI] ^= (childArySI[midSI] & swapSI);
      childArySI[midSI] ^= (childArySI[startSI] & swapSI);
      childArySI[startSI] ^= (childArySI[midSI] & swapSI);

      /*_________compare_mid_and_end____________________*/
      swapSI = -(edgeArySI[midSI] > edgeArySI[endSI]);
      indexArySI[ childArySI[midSI] ] =
         (midSI & ~swapSI) | (endSI & swapSI);
      indexArySI[ childArySI[endSI] ] =
         (midSI & swapSI) | (endSI & ~swapSI);

      edgeArySI[midSI] ^= (edgeArySI[endSI] & swapSI);
      edgeArySI[endSI] ^= (edgeArySI[midSI] & swapSI);
      edgeArySI[midSI] ^= (edgeArySI[endSI] & swapSI);

      childArySI[midSI] ^= (childArySI[endSI] & swapSI);
      childArySI[endSI] ^= (childArySI[midSI] & swapSI);
      childArySI[midSI] ^= (childArySI[endSI] & swapSI);

      /**************************************************\
      * Fun07 Sec04 Sub02:
      *   - move midpoint to the last item in the array
      \**************************************************/

      /*_________position_the_midpoint_at_the_end_______*/
      indexArySI[ childArySI[midSI] ] = endSI;
      indexArySI[ childArySI[endSI] ] = midSI;

      edgeArySI[midSI] ^= edgeArySI[endSI];
      edgeArySI[endSI] ^= edgeArySI[midSI];
      edgeArySI[midSI] ^= edgeArySI[endSI];

      childArySI[midSI] ^= childArySI[endSI];
      childArySI[endSI] ^= childArySI[midSI];
      childArySI[midSI] ^= childArySI[endSI];

      midSI = endSI - 1;
      posSI = startSI;

      /**************************************************\
      * Fun07 Sec04 Sub03:
      *   - paritally sort array to find midpoint position
      \**************************************************/

      while(posSI < midSI)
      { /*Loop: shift elements around*/
         /*see sec02 logic blocks for explanations*/
         swapSI = -(edgeArySI[endSI] <= edgeArySI[posSI]);
         indexArySI[ childArySI[posSI] ] =
            (midSI & swapSI) | (posSI & ~swapSI);
         indexArySI[ childArySI[midSI] ] =
            (midSI & ~swapSI) | (posSI & swapSI);

         edgeArySI[posSI] ^= (edgeArySI[midSI] &swapSI);
         edgeArySI[midSI] ^= (edgeArySI[posSI] &swapSI);
         edgeArySI[posSI] ^= (edgeArySI[midSI] &swapSI);

         childArySI[posSI] ^= (childArySI[midSI] &swapSI);
         childArySI[midSI] ^= (childArySI[posSI] &swapSI);
         childArySI[posSI] ^= (childArySI[midSI] &swapSI);
            /*see sec02 logic blocks for explanations*/

         midSI += swapSI; /*swapSI is -1 if swapped or 0*/
         posSI += (! swapSI); /*only move if no swap*/
      } /*Loop: shift elements around*/

      /**************************************************\
      * Fun07 Sec04 Sub04:
      *   - move the midpoint to its actual position
      \**************************************************/

      midSI += (edgeArySI[endSI] > edgeArySI[midSI]);
         /*this incurments midSI by one if the value at
         `  midSI is one less then my midpiont.
         */
      swapSI = -(midSI != endSI);

      indexArySI[ childArySI[midSI] ] =
         (midSI & ~swapSI) | (endSI & swapSI);
      indexArySI[ childArySI[endSI] ] =
         (midSI & swapSI) | (endSI & ~swapSI);

      edgeArySI[midSI] ^= (edgeArySI[endSI] & swapSI);
      edgeArySI[endSI] ^= (edgeArySI[midSI] & swapSI);
      edgeArySI[midSI] ^= (edgeArySI[endSI] & swapSI);

      childArySI[midSI] ^= (childArySI[endSI] & swapSI);
      childArySI[endSI] ^= (childArySI[midSI] & swapSI);
      childArySI[midSI] ^= (childArySI[endSI] & swapSI);

      /**************************************************\
      * Fun07 Sec04 Sub05:
      *   - check if the midpoint was the kth value
      \**************************************************/

      if(midSI == kthSI)
         break;
      else if(midSI < kthSI)
         startSI = midSI + 1;
      else
         endSI = midSI - 1;
   } /*Loop: find kth value and partially sort*/

   if(startSI == endSI - 1)
   { /*If: I can do one more swap*/
      swapSI = -(edgeArySI[endSI] < edgeArySI[startSI]);
      indexArySI[ childArySI[startSI] ] =
         (endSI & swapSI) | (startSI & ~swapSI);
      indexArySI[ childArySI[endSI] ] =
         (endSI & ~swapSI) | (startSI & swapSI);

      edgeArySI[startSI] ^= (edgeArySI[endSI] & swapSI);
      edgeArySI[endSI] ^= (edgeArySI[startSI] & swapSI);
      edgeArySI[startSI] ^= (edgeArySI[endSI] & swapSI);

      childArySI[startSI] ^= (childArySI[endSI] & swapSI);
      childArySI[endSI] ^= (childArySI[startSI] & swapSI);
      childArySI[startSI] ^= (childArySI[endSI] & swapSI);
   } /*If: I can do one more swap*/

   return kthSI;
} /*qckSelect_heap_prim*/

/*-------------------------------------------------------\
| Fun08: findPivots_heap_prim
|   - find the pivots in the quick heap
| Input:
|   - primSTPtr:
|     o heap_prim struct pointer to find the pivots for
| Output:
|   - Modifies:
|     o pivotArySI in primSTPtr to have at least the first
|       pivot and if no pivots finds a new set of pivots
\-------------------------------------------------------*/
void
findPivots_heap_prim(
   struct heap_prim *primSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - find the pivots in the quick heap
   '   o fun08 sec01:
   '     - variable declrations
   '   o fun08 sec02:
   '     - check if I need to find pivots
   '   o fun08 sec03:
   '     - find the number of pivots to get
   '   o fun08 sec04:
   '     - find the pivots
   '   o fun08 sec05:
   '     - make sure no blank pivots and find first pivot
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declrations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int endSI = 0;
   signed int startArySI[64];
   signed int endArySI[64];
   signed int pivotArySI[64];
      /*I should never have more then 2^32 pivots, so
      `  2^64 is more then enough space.
      */
   signed int lenSI = 0;  /*length of my stack*/
   signed int indexSI = 0;/*index I am at in my stack*/

   signed int pivotSI = 0;

   /*are for finding the pivot position in the array*/
   signed int offsetSI = 0;
   signed int nextSetSI = 0;
   signed int cntSI = 0;
   signed int lastPivotSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - check if I need to find pivots
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(primSTPtr->pivotLenSI > 1)
   { /*If: only need to find the starting pivot*/
      if(primSTPtr->pivotArySI[1] - primSTPtr->indexSI >1)
      { /*If: have to find the first pivot*/
         primSTPtr->pivotArySI[0] =
            qckSelect_heap_prim(
               primSTPtr,
               primSTPtr->indexSI,
               primSTPtr->pivotArySI[1] - 1,
               1 /*find the starting pivot*/
            );
      } /*If: have to find the first pivot*/

      else if(
         primSTPtr->pivotArySI[1] == primSTPtr->indexSI
      ){ /*Else If: the next pivot becomes the first*/
         for(
            lenSI = 0;
            lenSI < primSTPtr->pivotLenSI - 1;
            ++lenSI
         ){ /*Loop: remove blank pivots*/
            primSTPtr->pivotArySI[lenSI] =
                  primSTPtr->pivotArySI[lenSI + 1];
         } /*Loop: remove blank pivots*/

         --primSTPtr->pivotLenSI;
      } /*Else If: the next pivot becomes the first*/

      else
         primSTPtr->pivotArySI[0] = primSTPtr->indexSI;
         /*only one item, so can assign pivot safely*/

      return; /*pivots are still left, so leave all other
              `  pivots alone. It may be a good idea to
              `  do a pivot grouping check to handle
              `  worst cases
              */
   } /*If: only need to find the starting pivot*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - find the number of pivots to get
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lenSI = primSTPtr->heapLenSI - primSTPtr->indexSI;
   endSI = lenSI >> 3;
      /*divide by 8 to have at least 8 items per pivot*/

   if(endSI > primSTPtr->pivotSizeSI)
      endSI = primSTPtr->pivotSizeSI - 1;
   else if(endSI > 1)
      --endSI; /*convert pivot length to index 0*/
   else
   { /*Else: less then eight items left*/
      primSTPtr->pivotArySI[0] =
         qckSelect_heap_prim(
            primSTPtr,
            primSTPtr->indexSI,
            primSTPtr->heapLenSI - 1,
            1
         );
      primSTPtr->pivotLenSI = 1;
      return;
   } /*Else: less then eight items left*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - find the pivots
   ^   o fun08 sec04 sub01:
   ^     - setup for the pivot loop (initialze)
   ^   o fun08 sec04 sub02:
   ^     - find the pivots
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec04 Sub01:
   *   - setup for the pivot loop (initialze)
   \*****************************************************/

   for(lenSI = 0; lenSI <= endSI; ++lenSI)
      primSTPtr->pivotArySI[lenSI] = -1;
      /*make sure I can detect unset pivots*/

   /*______________set_up_my_stacks_____________________*/
   startArySI[0] = primSTPtr->indexSI;
   endArySI[0] = primSTPtr->heapLenSI - 1;

   if(endSI > 1)
      pivotArySI[0] = endSI >> 1;
   else
      pivotArySI[0] = 1;

   lenSI = 1;
   indexSI = 0;
   primSTPtr->pivotLenSI = 0;
   offsetSI = endSI >> 2;
   nextSetSI = 1;

   /*****************************************************\
   * Fun08 Sec04 Sub02:
   *   - find the pivots
   \*****************************************************/

   /*This loops ensures that the non-start pivots are
   `  evenly placed throughout the heap
   */
   while(0 == 0)
   { /*Loop: find the non-start pivots*/
      /*_____________find_the_pivot_____________________*/
      pivotSI =
         qckSelect_heap_prim(
            primSTPtr,
            startArySI[indexSI], /*first item in range*/
            endArySI[indexSI],   /*last item in range*/
            0                    /*find middle point*/
          );
      primSTPtr->pivotArySI[pivotArySI[indexSI]]=pivotSI;
      lastPivotSI =
         max_genMath(lastPivotSI, pivotArySI[indexSI]);
      --endSI;
      ++primSTPtr->pivotLenSI;

      if(! endSI)
         break; /*all pivots have been filled*/
      else if(indexSI >= lenSI)
         break; /*added all pivots*/
      else if(! offsetSI)
      { /*Else If: at end of pivot list*/
         ++indexSI;

         if(indexSI >= lenSI)
            break; /*added all pivots*/
         else
            continue; /*no more pivots to add*/
      } /*Else If: at end of pivot list*/

      /*___________add_next_pivot_ranges_to_stack_______*/
      startArySI[lenSI] = pivotSI + 1;
      endArySI[lenSI] = endArySI[indexSI];
      pivotArySI[lenSI] = pivotArySI[indexSI] + offsetSI;
      ++lenSI;

      startArySI[lenSI] = startArySI[indexSI];
      endArySI[lenSI] = pivotSI - 1;
      pivotArySI[lenSI] = pivotArySI[indexSI] - offsetSI;
      ++lenSI;

      /*___________move_to_the_next_pivot_to_find_______*/
      ++indexSI;
      ++cntSI;

      if(cntSI == nextSetSI)
      { /*If: moving to next set of pivot values*/
         offsetSI >>= 1;
         cntSI = 0;
         nextSetSI <<= 1;
         /*I need this step because I am working from the
         `   midpoint to the ends. It may not be perfect,
         `   but it should get most of the pivot positions
         */
      } /*If: moving to next set of pivot values*/
   } /*Loop: find the non-start pivots*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - make sure no blank pivots and find first pivot
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*I need this step because there may be some cases were
   `  a pivot might be missed by my system. I want to
   `  make sure these pivots are removed
   */
   endSI = 1;
   for(lenSI = 1; lenSI <= lastPivotSI; ++lenSI)
   { /*Loop: remove blank pivots*/
      if(primSTPtr->pivotArySI[lenSI] < 0)
         ;
      else
      { /*Else: pivot was set*/
         primSTPtr->pivotArySI[endSI] =
            primSTPtr->pivotArySI[lenSI];
         ++endSI;
      } /*Else: pivot was set*/
   } /*Loop: remove blank pivots*/
   primSTPtr->pivotLenSI = endSI;

   /*__________________find_the_first_pivot_____________*/
   if(primSTPtr->pivotArySI[0] < 0)
      ++primSTPtr->pivotLenSI;
      /*if not overwriting a pivot*/

   primSTPtr->pivotArySI[0] =
      qckSelect_heap_prim(
         primSTPtr,
         primSTPtr->indexSI,
         primSTPtr->pivotArySI[1] - 1,
         1
      );

   primSTPtr->pivotLenSI = endSI;
} /*findPivots_heap_prim*/

/*-------------------------------------------------------\
| Fun09: delInsEdge_heap_prim
|   - deletes edge and then insert the better edge for the
|     same child node into a heap_prim
| Input:
|   - edgeSI:
|     o the weight of the new edge, it should always
|       be less then the weight of the old edge
|   - nodeSI:
|     o child node to delInsete the edge for
|   - parSI:
|     o the parent node the edge connect to
|   - primSTPtr:
|     o heap_prim struct pointer to delete the node from
| Output:
|   - Modifies:
|     o edgeArySI in primSTPtr to have nodeSI delete
|     o childArySI in primSTPtr to have nodeSI deelted
|     o other variables in primSTPtr as needed
|   - Returns:
|     o 1 if could quickly insert the new edge
|       * this means the edges cost is still puts it by
|         the same pivot
|     o 0 for no errors
|     o def_overflowErr_prim if deleted element is outside
|       of the stack
\-------------------------------------------------------*/
signed char
delInsEdge_heap_prim(
   signed int edgeSI,
   signed int nodeSI,
   signed int parSI,
   struct heap_prim *primSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - find the pivot position of the old edge
   '   o fun09 sec03:
   '     - delInsete old edge and insert the new edge
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int indexSI = primSTPtr->indexArySI[nodeSI];
   signed int movePosSI = 0;
   signed int moveNodeSI = 0;

   signed int nodeOnSI = 0;
   signed int pivotSI = 0;
   signed int pivotIndexSI = 0;

   /*this is for pivot shifting*/
   signed int lastPivotSI = 0;
   signed int firstPivotSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - find the pivot position of the old edge
   ^   o fun09 sec02 sub01:
   ^     - get ending pivot and check if have pivots to
   ^       look through
   ^   o fun09 sec02 sub02:
   ^     - find the pivot to insert at
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec02 Sub01:
   *   - get ending pivot and check if have pivots to
   *     look through
   \*****************************************************/

   /*____________find_the_first_pivot___________________*/
   pivotSI = primSTPtr->pivotLenSI - 1;

   if(pivotSI <= 0)
   { /*If: one or no pivots*/
      firstPivotSI = primSTPtr->pivotArySI[0];
      
      if(pivotSI < 0)
         ;
      else if(edgeSI < primSTPtr->edgeArySI[firstPivotSI])
      { /*Else If: I need to set the new pivot*/
         moveNodeSI = primSTPtr->childArySI[firstPivotSI];

         primSTPtr->edgeArySI[indexSI] =
            primSTPtr->edgeArySI[firstPivotSI];
         primSTPtr->childArySI[indexSI] = moveNodeSI;
         primSTPtr->indexArySI[moveNodeSI] = indexSI;

         indexSI = firstPivotSI;
      } /*Else If: I need to set the new pivot*/

      primSTPtr->edgeArySI[indexSI] = edgeSI;
      primSTPtr->childArySI[indexSI] = nodeSI;
      primSTPtr->indexArySI[nodeSI] = indexSI;
      primSTPtr->parArySI[nodeSI] = parSI;

      goto retNoErr_fun08_sec04;
   } /*If: one or no pivots*/

   /*****************************************************\
   * Fun09 Sec02 Sub02:
   *   - find the pivot to insert at
   \*****************************************************/

   pivotIndexSI = primSTPtr->pivotArySI[pivotSI];

   while(pivotSI > -1 && pivotIndexSI > indexSI)
   { /*Loop: find the pivot before the new edge*/
      --pivotSI;
      pivotIndexSI = primSTPtr->pivotArySI[pivotSI];
   } /*Loop: find the pivot before the new edge*/

   if(pivotSI < 0)
      goto overflow_fun08_sec04;

   /*_____check_if_deleting_pivot_and_if_can_insert_____*/
   if(indexSI == pivotIndexSI)
   { /*If: deleting a pivot*/
      if(indexSI == primSTPtr->indexSI)
         goto insertPivot_fun08_sec04;
         /*deleting the first pivot*/

      for(
         nodeOnSI = pivotSI;
         nodeOnSI < primSTPtr->pivotLenSI - 1;
         ++nodeOnSI
      ){ /*Loop: shift pivot positions*/
         primSTPtr->pivotArySI[nodeOnSI] =
            primSTPtr->pivotArySI[nodeOnSI + 1];
      }  /*Loop: shift pivot positions*/

      --primSTPtr->pivotLenSI;

      if(edgeSI >= primSTPtr->edgeArySI[pivotIndexSI] - 1)
         goto insertPivot_fun08_sec04;
         /*can insert new edge at old pivot position;
         `  in this case I lost the pivot, but I do not
         `  need to find the insert postion
         */
      else if(pivotSI)
         --pivotSI; /*removed the old pivot*/
      else
      { /*Else: I am removing the first pivot*/
         indexSI = primSTPtr->indexSI;
         goto insertPivot_fun08_sec04;
      } /*Else: I am removing the first pivot*/
   } /*If: deleting a pivot*/

   else if (edgeSI >= primSTPtr->edgeArySI[pivotIndexSI])
      goto insertPivot_fun08_sec04;
      /*new weight stays with same pivot; so no need to
      `  change the position
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - delete old edge and insert the new edge
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   movePosSI = indexSI;

   while(pivotSI > 0)
   { /*Loop: find the insert position*/
      /*__________check_if_found_insert_point___________*/
      indexSI = primSTPtr->pivotArySI[pivotSI];

      if(edgeSI > primSTPtr->edgeArySI[indexSI])
      { /*If: found the insert point*/
         indexSI = movePosSI;
         break;
      } /*If: found the insert point*/

      /*___________move_insert_item_to_the_pivot________*/
      ++indexSI; /*move to edge after the pivot*/
      moveNodeSI = primSTPtr->childArySI[indexSI];

      primSTPtr->edgeArySI[movePosSI] =
         primSTPtr->edgeArySI[indexSI];
      primSTPtr->childArySI[movePosSI] = moveNodeSI;
      primSTPtr->indexArySI[moveNodeSI] = movePosSI;

      /*___________move_the_pivot_up____________________*/
      --indexSI; /*move to pivot*/
      movePosSI = indexSI;
      moveNodeSI = primSTPtr->childArySI[indexSI];

      primSTPtr->edgeArySI[indexSI + 1] =
         primSTPtr->edgeArySI[indexSI];
      primSTPtr->childArySI[indexSI + 1] = moveNodeSI;

      ++primSTPtr->pivotArySI[pivotSI];
      ++primSTPtr->indexArySI[moveNodeSI];

      /*______delete_neighboring_pivots_________________*/
      if(
         pivotSI
         &&    primSTPtr->pivotArySI[pivotSI] - 2
            == primSTPtr->pivotArySI[pivotSI - 1]
      ){ /*If: two pivots are next to each other*/
         /*the -2 in the if check is because I moved the
         `  pivot one item up
         */
         if(pivotSI < primSTPtr->pivotLenSI - 1)
            lastPivotSI = pivotSI + 1;
         else
            lastPivotSI = primSTPtr->pivotLenSI;

         lastPivotSI = pivotSI;
         --pivotSI;

         while(
               pivotSI > 0
            &&    primSTPtr->pivotArySI[pivotSI] - 1
               == primSTPtr->pivotArySI[pivotSI - 1]
         ) --pivotSI;/*find 1st non-overlapping pivot*/

         firstPivotSI = pivotSI + 1;
         for(
            ;
            lastPivotSI < primSTPtr->pivotLenSI;
            ++lastPivotSI
         ){ /*Loop: shift pivot positions*/
            primSTPtr->pivotArySI[firstPivotSI++] =
               primSTPtr->pivotArySI[lastPivotSI];
         }  /*Loop: shift pivot positions*/

         primSTPtr->pivotLenSI = firstPivotSI;
      }  /*If: two pivots are next to each other*/

      else
         --pivotSI;
   } /*Loop: find the insert position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto insertPivot_fun08_sec04;

   insertPivot_fun08_sec04:;
      pivotSI = primSTPtr->indexSI;
      if(edgeSI < primSTPtr->edgeArySI[pivotSI])
      { /*If: replacing the first pivot*/
         primSTPtr->edgeArySI[indexSI] =
            primSTPtr->edgeArySI[pivotSI];
         primSTPtr->childArySI[indexSI] =
            primSTPtr->childArySI[pivotSI];

         pivotSI = primSTPtr->childArySI[indexSI];
         primSTPtr->indexArySI[pivotSI] = indexSI;
         indexSI = primSTPtr->indexSI;
      } /*If: replacing the first pivot*/

      primSTPtr->edgeArySI[indexSI] = edgeSI;
      primSTPtr->childArySI[indexSI] = nodeSI;

      primSTPtr->indexArySI[nodeSI] = indexSI;
      primSTPtr->parArySI[nodeSI] = parSI;

      goto retNoErr_fun08_sec04;

   retNoErr_fun08_sec04:;
      return 0;

   overflow_fun08_sec04:;
      return def_overflowErr_prim;
} /*delInsEdge_heap_prim*/

/*-------------------------------------------------------\
| Fun10: insEdge_heap_prim
|   - inset an edge into my prim quick heap
| Input:
|   - edgeSI:
|     o weight of edge to insert
|   - childSI:
|     o child node the edge connects to
|   - parSI:
|     o parent node connecting to the child
|   - primSTPtr:
|     o heap_prim struct pointer to add the node to
| Output:
|   - Modifies:
|     o all variables in primSTPtr to have the new edges
\-------------------------------------------------------*/
void
insEdge_heap_prim(
   signed int edgeSI,  /*edge to add*/
   signed int childSI, /*child node for the edge to add*/
   signed int parSI,   /*parent node for the edge to add*/
   struct heap_prim *primSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10: TOC
   '   - inset an edge into my prim quick heap
   '   o fun10 sec01:
   '     - variable declarations
   '   o fun10 sec02:
   '     - no pivots insert
   '   o fun10 sec03:
   '     - inserting at end; make insert the ending pivot
   '   o fun10 sec04:
   '     - find position to insert the edge at
   '   o fun10 sec05:
   '     - add the edge in
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
 
   signed int indexSI = 0;
   signed int pivotSI = 0;
   signed int moveNodeSI = 0;
   signed int movePosSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - no pivots insert
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(primSTPtr->pivotLenSI <= 0)
   { /*If: no pivots*/
      indexSI = primSTPtr->heapLenSI;
      ++primSTPtr->heapLenSI;

      primSTPtr->edgeArySI[indexSI] = edgeSI;
      primSTPtr->childArySI[indexSI] = childSI;
      primSTPtr->parArySI[childSI] = parSI;
      primSTPtr->indexArySI[childSI] = indexSI;

      return;
   } /*If: no pivots*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - inserting at end; make insert the ending pivot
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   pivotSI = primSTPtr->pivotLenSI - 1;
   indexSI = primSTPtr->pivotArySI[pivotSI];

   if(edgeSI > primSTPtr->edgeArySI[indexSI])
   { /*If: edge is inserted at the end*/
      indexSI = primSTPtr->heapLenSI;
      ++primSTPtr->heapLenSI;

      primSTPtr->edgeArySI[indexSI] = edgeSI;
      primSTPtr->childArySI[indexSI] = childSI;
      primSTPtr->parArySI[childSI] = parSI;
      primSTPtr->indexArySI[childSI] = indexSI;

      return;
   } /*If: edge is inserted at the end*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - find position to insert the edge at
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   movePosSI = primSTPtr->heapLenSI;

   while(pivotSI > 0)
   { /*Loop: find the insert position*/
      /*__________check_if_found_insert_point___________*/
      indexSI = primSTPtr->pivotArySI[pivotSI];

      if(edgeSI > primSTPtr->edgeArySI[indexSI])
      { /*If: found the insert point*/
         indexSI = movePosSI;
         break;
      } /*If: found the insert point*/

      else if(
            primSTPtr->pivotArySI[pivotSI] - 1
         == primSTPtr->pivotArySI[pivotSI - 1]
      ){ /*Else If: pivots are right next to each other*/
         ++indexSI; /*position to move to*/
         goto mvPivotUp_fun10_sec04;
      }  /*Else If: pivots are right next to each other*/

      /*___________move_insert_item_to_the_pivot________*/
      ++indexSI; /*move to edge after the pivot*/
      moveNodeSI = primSTPtr->childArySI[indexSI];

      primSTPtr->edgeArySI[movePosSI] =
         primSTPtr->edgeArySI[indexSI];
      primSTPtr->childArySI[movePosSI] =
         primSTPtr->childArySI[indexSI];
      primSTPtr->indexArySI[moveNodeSI] = movePosSI;

      /*___________move_the_pivot_up____________________*/
      mvPivotUp_fun10_sec04:;
         --indexSI; /*move to pivot*/
         movePosSI = indexSI;
         moveNodeSI = primSTPtr->childArySI[indexSI];

         primSTPtr->edgeArySI[indexSI + 1] =
            primSTPtr->edgeArySI[indexSI];
         primSTPtr->childArySI[indexSI + 1] =
            primSTPtr->childArySI[indexSI];

         ++primSTPtr->pivotArySI[pivotSI];
         ++primSTPtr->indexArySI[moveNodeSI];
         --pivotSI;
   } /*Loop: find the insert position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec05:
   ^   - add the edge in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(edgeSI < primSTPtr->edgeArySI[primSTPtr->indexSI])
   { /*If: inserting at the start*/
      indexSI = primSTPtr->indexSI;
      moveNodeSI = primSTPtr->childArySI[indexSI];

      primSTPtr->edgeArySI[movePosSI] =
         primSTPtr->edgeArySI[indexSI];
      primSTPtr->childArySI[movePosSI] =
         primSTPtr->childArySI[indexSI];

      ++primSTPtr->indexArySI[moveNodeSI];

      /*the pivot index stays the same so no need to
      `  change it. It is the new node
      */
   } /*If: inserting at the start*/

   /*______________add_the_new_node_in__________________*/
   primSTPtr->edgeArySI[indexSI] = edgeSI;
   primSTPtr->childArySI[indexSI] = childSI;
   primSTPtr->parArySI[childSI] = parSI;
   primSTPtr->indexArySI[childSI] = indexSI;

   ++primSTPtr->heapLenSI;
} /*insEdge_heap_prim*/

/*-------------------------------------------------------\
| Fun11: addEdges_heap_prim
|   - add edges for a node to a heap_prim struct
| Input:
|   - edgeArySI:
|     o signed int array with the edge weights to add
|     o each index is the child node the edge connects to
|     o to skip an edge set the edges weight to -1
|   - childArySI:
|     o signed int array with child to assign each edge
|       to
|     o this array prevents a n^2 loop when the graph
|       is not complete (everything connected). Without
|       it I would have to have all edges input and skip
|       all edges that are set to -1
|   - numEdgesSI:
|     o number of edges to add (index 1)
|   - parNodeSI:
|     o node that every edge connects to (new node to add)
|   - primSTPtr:
|     o heap_prim struct pointer to get the new edges
| Output:
|   - Modifies:
|     o primSTPtr to have the new edges
\-------------------------------------------------------*/
void
addEdges_heap_prim(
   signed int *edgeArySI,  /*edges to add*/
   signed int *childArySI, /*child edges to add*/
   signed int numEdgesSI,  /*number edges to add*/
   signed int parNodeSI,   /*node edges belong to*/
   struct heap_prim *primSTPtr /*add edges to*/
){
   signed int siEdge = 0;
   signed int indexSI = 0;
   signed int childSI = 0;

   for(siEdge = 0; siEdge < numEdgesSI; ++siEdge)
   { /*Loop: add all edges*/
      childSI = childArySI[siEdge];
      if(edgeArySI[siEdge] < 0 || childSI < 0)
         continue; /*this edge should not be added*/

      indexSI = primSTPtr->indexArySI[childSI];
      if(indexSI < 0)
         continue; /*the child node this edge connects to
                   `  is already in the MST
                   */

      else if(indexSI == def_maxSI_prim)
      { /*Else If: this is a new edge*/
         insEdge_heap_prim(
            edgeArySI[siEdge], /*edge to add (weight)*/
            childSI,   /*child node the edge connects to*/
            parNodeSI,/*parent node the edge connects to*/
            primSTPtr
         ); /*else this is a new child insert the edge*/
      }  /*Else If: this is a new edge*/

      else if(
        edgeArySI[siEdge] >= primSTPtr->edgeArySI[indexSI]
      ) continue;/*better edge for this node in the heap*/

      else
      { /*Else: this is a better edge*/
         delInsEdge_heap_prim(
            edgeArySI[siEdge], /*edge to add (weight)*/
            childSI,   /*child node the edge connects to*/
            parNodeSI,/*parent node the edge connects to*/
            primSTPtr
         ); /*if have a better edge to a child node then
            `  the child nodes edge in the quick heap,
            `  delete the old edge and insert the new edge
            */
      } /*Else: this is a better edge*/
   } /*Loop: add all edges*/
} /*addEdges_heap_prim*/

/*-------------------------------------------------------\
| Fun12: extractEdge_heap_prim
|   - extract the minimum edge from the heap_prim struct
| Input:
|   - primSTPtr
|     o heap_prim struct pointer to extact the minimum
|       edge from
| Output:
|   - Modifies:
|     o pivotarySI in primSTPtr to have the index 0 pivot
|       set to the next minimum edge
|     o indexSI in primSTPtr to be incurmented one, saving
|       the minimum edge in the tree
|     o edgeArySI and childArySI in primSTPtr to have the
|       next pivot (quick select)
|     o scoreArySI in primSTPtr to have the weight of
|       the edge added in
|     o indexArySI in primSTPtr to be set to -1 (node
|       is in the MST)
|   - Returns:
|     o child node for the next minimum edge
|     o -1 if no edges left to extract
\-------------------------------------------------------*/
signed int
extractEdge_heap_prim(
   struct heap_prim *primSTPtr
){
   signed int siPos = 0;

   if(! primSTPtr->indexSI)
   { /*If: on the first child*/
      findPivots_heap_prim(primSTPtr); /*first round*/
      ++primSTPtr->indexSI;
      findPivots_heap_prim(primSTPtr);
         /*I need to refinde the first pivot*/
      goto getNode_fun12;
   } /*If: on the first child*/

   ++primSTPtr->indexSI;

   if(primSTPtr->pivotLenSI < 2)
   { /*If: have one or fewer pivots*/
      if(primSTPtr->indexSI > primSTPtr->heapLenSI)
         goto emptyHeap_fun10;
      else
         findPivots_heap_prim(primSTPtr);
         /*only one pivot*/
   } /*If: have one or fewer pivots*/

   else if(primSTPtr->indexSI == primSTPtr->pivotArySI[1])
   { /*If: the next item is the next pivot*/
      for(
         siPos = 0;
         siPos < primSTPtr->pivotLenSI - 1;
         ++siPos
       ) primSTPtr->pivotArySI[siPos] =
            primSTPtr->pivotArySI[siPos + 1];

       --primSTPtr->pivotLenSI;
       if(primSTPtr->indexSI == primSTPtr->heapLenSI)
          goto emptyHeap_fun10;
   } /*If: the next item is the next pivot*/

   else
      findPivots_heap_prim(primSTPtr);
      /*this function deals with the new length and
      `  always assumes the first pivot is gone
      */

   getNode_fun12:;
      primSTPtr->scoreSI +=
         primSTPtr->edgeArySI[primSTPtr->indexSI - 1];
         /*add the edge weight to the MST score*/

      siPos = primSTPtr->indexSI - 1;
      siPos = primSTPtr->childArySI[siPos];
      primSTPtr->indexArySI[siPos] =
         (primSTPtr->indexArySI[siPos] + 1) * -1;
         /*mark node as in MST, but still keep track of
         `  the index's
         */

      return siPos;
         /*return the node added to the MST*/

   emptyHeap_fun10:;
      return -1;
} /*extractEdge_heap_prim*/

/*-------------------------------------------------------\
| Fun13: threeArySort_prim
|   - sorts a signed int array from least to greatest and
|     keeps two other arrays in sync
| Input:
|   - siAry:
|     o signed int array to sort
|   - secArySI:
|     o second signed int array to keep in sync with siAry
|   - thirdArySI:
|     o third signed int array to keep in sync with siAry
|   - lenSI:
|     o length of the array
| Output:
|   - Modifies:
|     o siAry to be sorted form least to greatest
\-------------------------------------------------------*/
void
threeArySort_prim(
   signed int *siAry,
   signed int *secArySI,
   signed int *thirdArySI,
   signed int lenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   - sorts a signed int array from least to greatest
   '     and keeps two other arrays in sync
   '   o fun13 sec01:
   '     - variable declerations
   '   o fun13 sec02:
   '     - find the number of rounds to sort for
   '   o fun13 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   /*Variables to incurment loops*/
   signed int siIndex = 0;
   signed int siElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   --lenSI;
   if(! lenSI)
      return;

   /*Recursion formsia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < lenSI - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(siIndex = 0; siIndex <= subSI; ++siIndex)
      { /*Loop: though sub array*/
         siElm = siIndex;

         for(
            siElm = siIndex;
            siElm + subSI <= lenSI;
            siElm += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = siElm + subSI;

            if(siAry[siElm] > siAry[nextSI])
            { /*If I need to swap an element*/
               siAry[siElm] ^= siAry[nextSI];
               siAry[nextSI] ^= siAry[siElm];
               siAry[siElm] ^= siAry[nextSI];

               secArySI[siElm] ^= secArySI[nextSI];
               secArySI[nextSI] ^= secArySI[siElm];
               secArySI[siElm] ^= secArySI[nextSI];

               thirdArySI[siElm] ^= thirdArySI[nextSI];
               thirdArySI[nextSI] ^= thirdArySI[siElm];
               thirdArySI[siElm] ^= thirdArySI[nextSI];

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  if(siAry[onSI] > siAry[lastSI])
                     break; /*Positioned the element*/
   
                  siAry[onSI] ^= siAry[lastSI];
                  siAry[lastSI] ^= siAry[onSI];
                  siAry[onSI] ^= siAry[lastSI];

                  secArySI[onSI] ^= secArySI[lastSI];
                  secArySI[lastSI] ^= secArySI[onSI];
                  secArySI[onSI] ^= secArySI[lastSI];

                  thirdArySI[onSI] ^= thirdArySI[lastSI];
                  thirdArySI[lastSI] ^= thirdArySI[onSI];
                  thirdArySI[onSI] ^= thirdArySI[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*threeArySort_prim*/

/*-------------------------------------------------------\
| Fun14: threeAryUnsort_prim
|   - unsorts three arrays using the first input array
| Input:
|   - indexArySI:
|     o signed int array to unsort with
|     o has the original index of each item
|     o each value in this array must be unique (infinite
|       loop) and must be smaller then lenSI (out of
|       bounds error)
|   - firstArySI:
|     o first signed int array with data to unsort
|   - secArySI:
|     o second signed int array with data to unsort
|   - lenSI:
|     o length of all arrays
| Output:
|   - Modifies:
|     o indexArySI to have each item at the index of its
|       value (ex: if item = 1, then index 1)
|     o firstArySI and secArySI to be sorted by indexArySI
\-------------------------------------------------------*/
void
threeAryUnsort_prim(
   signed int *indexArySI, /*has index's to unsort with*/
   signed int *firstArySI, /*first array to unsort*/
   signed int *secArySI,   /*second array to unsort*/
   signed int lenSI        /*number items in arrays*/
){
   signed int siPos = 0;
   signed int indexSI = 0;

   while(siPos < lenSI)
   { /*Loop: unsort the index array*/
      indexSI = indexArySI[siPos];

      if(siPos != indexSI)
      { /*If: need to swap*/
         secArySI[siPos] ^= secArySI[indexSI];
         secArySI[indexSI] ^= secArySI[siPos];
         secArySI[siPos] ^= secArySI[indexSI];

         firstArySI[siPos] ^= firstArySI[indexSI];
         firstArySI[indexSI] ^= firstArySI[siPos];
         firstArySI[siPos] ^= firstArySI[indexSI];

         indexArySI[siPos] ^= indexArySI[indexSI];
         indexArySI[indexSI] ^= indexArySI[siPos];
         indexArySI[siPos] ^= indexArySI[indexSI];
      } /*If: need to swap*/

      else
         ++siPos;
   } /*Loop: unsort the index array*/
} /*threeAryUnsort_prim*/

/*I originally wanted a midpoint method. However, it got
`  to complex to quickly and so I never got it working.
`  I am leaving the code here in case I want to revive it
*/
#ifdef MIDPIONT_NEWICK_PRIM
/*-------------------------------------------------------\
| Fun15: mstToNewickRecursive_heap_prim
|   - the recurisve part to save a mst as a newick file
|   - this function should only be called by
|     mstToNewick_heap_prim; do not call it
| Input:
|   - parIndexSI:
|     o index of the edge (parent to child) to add to the
|       newick file
|   - parWeightF:
|     o weight of the parent to child edge
|   - primSTPtr:
|     o heap_prim struct pointer with mst tree to save
|     o parArySI and indexArySI must be sorted by parent
|       node
|   - namesAryStr:
|     o cstring array with the name for each node
|   - outFILE:
|     o FILE pointer to save file to
| Output:
|   - Modifies:
|     o outFILE to have the mst in newick format
|   - Returns:
|     o dist_prim structure pointer (on heap) with the
|       distance to the next node
|     o 0 if had a memory error
\-------------------------------------------------------*/
struct dist_prim *
mstToNewickRecusive_heap_prim(
   signed int parIndexSI,      /*index parent I am on*/
   float parWeightF,           /*cost of the parent node*/
   struct heap_prim *primSTPtr,/*tree to print*/
   signed char *namesAryStr[], /*names for each node*/
   signed char pBl,            /*1 print this group*/
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:
   '   - the recurisve part to save a mst as a newick file
   '   - this function should only be called by
   '     mstToNewick_heap_prim; do not call it
   '   o fun15 sec01:
   '     - variable declarations & logic for internal node
   '   o fun15 sec02:
   '     - initialize and detect if we have children
   '   o fun15 sec03:
   '     - find the lowest cost edge to the parent node
   '   o fun15 sec04:
   '     - print all child nodes
   '   o fun15 sec0y:
   '     - print parent node and the internal node
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declarations & logic for internal nodes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int parSI = 0;
   signed int indexSI = 0;
   signed int nextChildSI = -1;
   signed int weightSI = -1;
   signed int childSI = 0;
   float midWeightF = 0;

   struct dist_prim *distHeapST =
      malloc(sizeof(struct dist_prim));
   struct dist_prim *listDistHeapST = 0;
   struct dist_prim *nextDistSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - initialize and detect if we have children
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   parSI = primSTPtr->parArySI[parIndexSI];
   indexSI = parIndexSI;

   if(! distHeapST)
      goto memErr_fun15_sec05;
   distHeapST->distF = -1;
   distHeapST->childSI = -1;
   distHeapST->firstChildIndexSI = parIndexSI;
   distHeapST->nextPtr = 0;

   listDistHeapST = malloc(sizeof(struct dist_prim));
   listDistHeapST->distF = parWeightF;
   listDistHeapST->childSI = parSI;
   listDistHeapST->firstChildIndexSI = parIndexSI;
   listDistHeapST->nextPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - find the lowest cost edge to the parent node
   ^   o fun15 sec03 sub01:
   ^     - get all distances (including internal nodes)
   ^   o fun15 sec03 sub02:
   ^     - get midpiont distance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun15 Sec03 Sub01:
   *   - get all distances (including internal nodes)
   *   o fun15 sec03 sub01 cat01:
   *     - start loop, find if internal node, add weight
   *   o fun15 sec03 sub01 cat02:
   *     - add the new node/edge to the node/edge list
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun15 Sec03 Sub01 Cat01:
   +   - start loop, find if internal node, add weight
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(primSTPtr->parArySI[indexSI] == parSI)
   { /*Loop: get the children distances*/
      weightSI = primSTPtr->indexArySI[indexSI];
      weightSI = (weightSI * -1) - 1;

      childSI = primSTPtr->childArySI[weightSI];
      weightSI = primSTPtr->edgeArySI[weightSI];

      nextChildSI =
         getStart_siBinSearch(
            childSI,
            primSTPtr->parArySI,
            primSTPtr->indexSI
         ); /*find first child with this parent node*/

      if(nextChildSI >= 0)
      { /*If: have a next child*/
         nextDistSTPtr =
            mstToNewickRecusive_heap_prim(
               nextChildSI,
               weightSI,
               primSTPtr,
               namesAryStr,
               0,           /*do not print this yet*/
               outFILE
            ); /*setup the next internal node*/

        if(! nextDistSTPtr)
           goto memErr_fun15_sec05;
      } /*If: have a next child*/

      else
      { /*Else: have a tip*/
         nextDistSTPtr = malloc(sizeof(struct dist_prim));
         if(! nextDistSTPtr)
            goto memErr_fun15_sec05;

         nextDistSTPtr->distF = weightSI;
         nextDistSTPtr->childSI = childSI;
         nextDistSTPtr->firstChildIndexSI = -1;
         nextDistSTPtr->nextPtr = 0;
      } /*Else: have a tip*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun15 Sec03 Sub01 Cat02:
      +   - add the new node/edge to the node/edge list
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(! listDistHeapST)
         listDistHeapST = nextDistSTPtr;

      else if(
         nextDistSTPtr->distF < listDistHeapST->distF
      ){ /*Else If: new head of the list*/
         nextDistSTPtr->nextPtr = listDistHeapST;
         listDistHeapST = nextDistSTPtr;
      }  /*Else If: new head of the list*/

      else
      { /*Else: no a lowest value*/
         /*this does not need to be sorted, it only
         `  needs to have the lowest distance first
         */
         nextDistSTPtr->nextPtr =
            listDistHeapST->nextPtr;
         listDistHeapST->nextPtr = nextDistSTPtr;
      } /*Else: no a lowest value*/

      ++indexSI;
   } /*Loop: get the children distances*/

   /*****************************************************\
   * Fun15 Sec03 Sub02:
   *   - get midpiont distance
   \*****************************************************/

   midWeightF = listDistHeapST->distF / 2;
   distHeapST->distF = midWeightF;
   distHeapST->childSI = -1; /*mark as an internal node*/

   if(! pBl)
      goto ret_fun15_sec05;
      /*this was a non-print run; in this case I am trying
      `  to find the distances to get the midpoint
      */

   fprintf((FILE *) outFILE, "(");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec04:
   ^   - print child nodes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(listDistHeapST)
   { /*Loop: print this node*/
      if(listDistHeapST->childSI >= 0)
         fprintf(
            (FILE *) outFILE,
            "%s:%0.2f",
            namesAryStr[listDistHeapST->childSI],
            listDistHeapST->distF - midWeightF
         );

      else
      { /*Else: need to print an internal node*/
         nextDistSTPtr =
            mstToNewickRecusive_heap_prim(
               listDistHeapST->firstChildIndexSI,
               listDistHeapST->distF,
               primSTPtr,
               namesAryStr,
               1,           /*do not print this yet*/
               outFILE
            ); /*setup the next internal node*/

        if(! nextDistSTPtr)
           goto memErr_fun15_sec05;
        else
           free(nextDistSTPtr); /*already have*/

        fprintf(
           (FILE *) outFILE,
           ":%0.2f",
           listDistHeapST->distF - midWeightF
        ); /*add the weight to the internal node*/
      } /*Else: need to print an internal node*/

      nextDistSTPtr = listDistHeapST->nextPtr;
      free(listDistHeapST);
      listDistHeapST = nextDistSTPtr;

      if(listDistHeapST)
         fprintf((FILE *) outFILE, ",");
   } /*Loop: print this node*/

   fprintf((FILE *) outFILE, ")");

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun15_sec05;

   memErr_fun15_sec05:;
      if(distHeapST)
         free(distHeapST);
      distHeapST = 0;

   ret_fun15_sec05:;
      while(listDistHeapST)
      { /*Loop: free my distance list*/
         nextDistSTPtr = listDistHeapST->nextPtr;
         free(listDistHeapST);
         listDistHeapST = nextDistSTPtr;
      } /*Loop: free my distance list*/

      listDistHeapST = 0;

      return distHeapST;
} /*mstToNewickRecusrive_heap_prim*/

/*-------------------------------------------------------\
| Fun16: mstToNewick_heap_prim
|   - saves a minimum spanning tree as a newick file
| Input:
|   - primSTPtr:
|     o heap_prim struct pointer with mst tree to save
|   - namesAryStr:
|     o cstring array with the name for each node
|   - siAry:
|     o signed in array to use in unsorting, the size
|       should be at least the number of nodes
|     o logic behind using this instead of memory is
|       that as a user you will be using this anyways to
|       build the tree
|   - outFILE:
|     o FILE pointer to save file to
| Output:
|   - Modifies:
|     o outFILE to have the mst in newick format
|     o siAry to have 0 to the last child node
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
\-------------------------------------------------------*/
signed char
mstToNewick_heap_prim(
   struct heap_prim *primSTPtr,
   signed char *namesAryStr[],
   signed int *siAry,
   void *outFILE
){
   /*________varaible declarations______________________*/
   signed int siPos = 0;
   signed int indexSI = primSTPtr->childArySI[0];
   signed int weightSI = 0;
   struct dist_prim *distHeapST = 0;

   /*_________setup and sort nodes by parents___________*/
   indexSI = primSTPtr->parArySI[indexSI];/*first parent*/
   weightSI = primSTPtr->edgeArySI[0];

   /*set up for unsorting later*/
   for(siPos = 0; siPos < primSTPtr->indexSI; ++siPos)
      siAry[siPos] = siPos;

   /*sort the arrays by parent id (n x log2(n))*/
   threeArySort_prim(
      primSTPtr->parArySI,
      primSTPtr->indexArySI,
      siAry,
      primSTPtr->indexSI
   );
   /*at this point siAry has all the child node index's
   `   in parArySI, while parArySI is sorted by the
   `   parent index.
   */

   for(siPos = 0; siPos < primSTPtr->indexSI; ++siPos)
   { /*Loop: find the index of the first parent edge*/
      if(indexSI == primSTPtr->parArySI[siPos])
         break;
   } /*Loop: find the index of the first parent edge*/

   indexSI = siPos;

   /*________print the tree_____________________________*/
   distHeapST =
      mstToNewickRecusive_heap_prim(
         indexSI,    /*first parent edge in sorted array*/
         weightSI,   /*weight of the first edge*/
         primSTPtr,  /*has mst*/
         namesAryStr,/*names for each node*/
         1,          /*print the tree*/
         outFILE     /*file to print newick to*/
      );

   if(! distHeapST)
      goto memErr_fun16;

   /*add the root internal node (add its distance)*/
   fprintf((FILE *) outFILE,":%0.2f",distHeapST->distF);
   fprintf((FILE *) outFILE, "%s", str_endLine);

   free(distHeapST);
   distHeapST = 0;

   /*________unsort the nodes___________________________*/
   threeAryUnsort_prim(
      siAry,
      primSTPtr->parArySI,
      primSTPtr->indexArySI,
      primSTPtr->indexSI
   );

   /*________return_____________________________________*/
   siPos = 0;
   goto ret_fun16;

   memErr_fun16:;
      siPos = 1;
      goto ret_fun16;

   ret_fun16:;
      if(distHeapST)
        free(distHeapST);
      distHeapST = 0;

      return (signed char) siPos;
} /*mstToNewick_heap_prim*/
#endif

/*-------------------------------------------------------\
| Fun15: root0MstToNewickRecursive_heap_prim
|   - the recurisve part to save a mst as a newick file
|   - this function should only be called by
|     mstToNewick_heap_prim; do not call it
| Input:
|   - parIndexSI:
|     o index of the edge (parent to child) to add to the
|       newick file
|   - parWeightF:
|     o cost of traveling to the parent node
|   - primSTPtr:
|     o heap_prim struct pointer with mst tree to save
|     o parArySI and indexArySI must be sorted by parent
|       node
|   - namesAryStr:
|     o cstring array with the name for each node
|   - outFILE:
|     o FILE pointer to save file to
| Output:
|   - Modifies:
|     o outFILE to have the mst in newick format
\-------------------------------------------------------*/
void
root0MstToNewickRecusive_heap_prim(
   signed int parIndexSI,      /*index parent I am on*/
   signed int parWeightSI,     /*cost of the parent node*/
   struct heap_prim *primSTPtr,/*tree to print*/
   signed char *namesAryStr[], /*names for each node*/
   void *outFILE
){
   signed int parSI = 0;
   signed int indexSI = 0;
   signed int nextChildSI = -1;
   signed int weightSI = -1;
   signed int childSI = 0;
   signed char lastBl = 0;

   parSI = primSTPtr->parArySI[parIndexSI];

   if(parSI == def_maxSI_prim)
      return; /*this node has no parent node*/

   indexSI = parIndexSI;

   fprintf(
      (FILE *) outFILE,
      "(%s:0",
      namesAryStr[parSI]
   ); /*print the parent node as the root distance*/
   lastBl = 1;

   while(primSTPtr->parArySI[indexSI] == parSI)
   { /*Loop: get the children distances*/
      weightSI = primSTPtr->indexArySI[indexSI];

      if(weightSI >= 0)
         goto nextChild_fun15;
         /*this node has not been assigned yet*/

      weightSI = (weightSI * -1) - 1;

      childSI = primSTPtr->childArySI[weightSI];
      weightSI = primSTPtr->edgeArySI[weightSI];

      if(lastBl)
         fprintf((FILE *) outFILE, ",");

      nextChildSI =
         getStart_siBinSearch(
            childSI,
            primSTPtr->parArySI,
            primSTPtr->indexSI
         ); /*find first child with this parent node*/

      if(nextChildSI >= 0)
         root0MstToNewickRecusive_heap_prim(
            nextChildSI,
            weightSI,
            primSTPtr,
            namesAryStr,
            outFILE
         ); /*setup the next internal node*/
      else
         fprintf(
            (FILE *) outFILE,
            "%s:%i",
            namesAryStr[childSI],
            weightSI
         );

      nextChild_fun15:;
         lastBl = 0;
         ++indexSI;

         if(indexSI >= primSTPtr->indexSI)
            break; /*finished*/
         else if(primSTPtr->parArySI[indexSI] == parSI)
            lastBl = 1;
   } /*Loop: get the children distances*/

   /*print the interal node distance as the cost of
   `  traveling to the parent
   */
   fprintf((FILE *) outFILE, "):%i", parWeightSI);
} /*root0MstToNewickRecusrive_heap_prim*/

/*-------------------------------------------------------\
| Fun16: root0MstToNewick_heap_prim
|   - saves a minimum spanning tree as a newick file
| Input:
|   - primSTPtr:
|     o heap_prim struct pointer with mst tree to save
|   - namesAryStr:
|     o cstring array with the name for each node
|   - siAry:
|     o signed in array to use in unsorting, the size
|       should be at least the number of nodes
|     o logic behind using this instead of memory is
|       that as a user you will be using this anyways to
|       build the tree
|   - outFILE:
|     o FILE pointer to save file to
| Output:
|   - Modifies:
|     o outFILE to have the mst in newick format
|     o siAry to have 0 to the last child node
\-------------------------------------------------------*/
void
root0MstToNewick_heap_prim(
   struct heap_prim *primSTPtr,
   signed char *namesAryStr[],
   signed int *siAry,
   void *outFILE
){
   /*________varaible declarations______________________*/
   signed int siPos = 0;
   signed int indexSI = primSTPtr->childArySI[0];

   /*_________setup and sort nodes by parents___________*/
   indexSI = primSTPtr->parArySI[indexSI];/*first parent*/

   /*set up for unsorting later*/
   for(siPos = 0; siPos < primSTPtr->sizeSI; ++siPos)
      siAry[siPos] = siPos;

   /*sort the arrays by parent id (n x log2(n))*/
   threeArySort_prim(
      primSTPtr->parArySI,
      primSTPtr->indexArySI,
      siAry,
      primSTPtr->sizeSI
   );
   /*at this point siAry has all the child node index's
   `   in parArySI, while parArySI is sorted by the
   `   parent index.
   */

   for(siPos = 0; siPos < primSTPtr->indexSI; ++siPos)
   { /*Loop: find the index of the first parent edge*/
      if(indexSI == primSTPtr->parArySI[siPos])
         break;
   } /*Loop: find the index of the first parent edge*/

   indexSI = siPos;

   /*________print the tree_____________________________*/
   root0MstToNewickRecusive_heap_prim(
      indexSI,    /*first parent edge in sorted array*/
      0,          /*root to first edge is 0*/
      primSTPtr,  /*has mst*/
      namesAryStr,/*names for each node*/
      outFILE     /*file to print newick to*/
   );

   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*________unsort the nodes___________________________*/
   threeAryUnsort_prim(
      siAry,
      primSTPtr->parArySI,
      primSTPtr->indexArySI,
      primSTPtr->sizeSI
   );
} /*root0MstToNewick_heap_prim*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
