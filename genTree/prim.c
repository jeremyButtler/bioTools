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
'   o .c fun13: 3ArySort_prim
'     - sorts a signed int array from least to greatest
'       and keeps two other arrays in sync
'   o .c fun14: 3AryUnsort_prim
'     - unsorts three arrays using the first input array
'   o .c fun15: mstToNewickRecusive_heap_prim
'     - the recurisve part to save a mst as a newick file
'     - this function should only be called by
'       mstToNewick_heap_prim; do not call it
'   o fun16: mstToNewick_heap_prim
'     - saves a minimum spanning tree as a newick file
'   o license
'     - licensing for this code (public domain / mit)
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

/*-------------------------------------------------------\
| Fun01: blank_heap_prim
|   - blanks a heap_prim structure to null (somewhat
|     lazy)
| Input:
|   - primPtr:
|     o pointer to a heap_prim struct to blank
| Output:
|   - sets indexSI, pivotLenSI, heapLenSI, and scoreSI
|     (lazy blank)
\-------------------------------------------------------*/
blank_heap_prim(
   struct heap_prim *primPtr
){
   if(! primPtr)
      return;
   primPtr->indexSI = 0; /*at first index in heap*/
   primPtr->pivotLenSI = 0;
   primPtr->heapLenSI = 0;
   primSTPtr->scoreSI = 0;
} /*blank_heap_prim*/

/*-------------------------------------------------------\
| Fun02: init_heap_prim
|   - initializes a heap_prim structure
| Input:
|   - primPtr:
|     o pointer to a heap_prim struct to initialize
| Output:
|   - sets all pointers and values to 0
\-------------------------------------------------------*/
init_heap_prim(
   struct heap_prim *primPtr
){
   if(! primPtr)
      return;
   primPtr->edgeArySI = 0;
   primPtr->heapNodeArySI = 0;

   primPtr->pivotArySI = 0;
   primPtr->pivotSizeSI = 0;

   primPtr->indexArySI = 0;
   primPtr->parArySI = 0;

   primPtr->sizeSI = 0;

   blank_heap_prim(primPtr);
} /*init_heap_prim*/

/*-------------------------------------------------------\
| Fun03: freeStack_heap_prim
|   - frees all variables in a heap_prim structure
| Input:
|   - primPtr
|     o pointer to a heap_prim struct with variables to
|       free
| Output:
|   - Modifies:
|     o primPtr to have everything freeded and to be
|       initialized
\-------------------------------------------------------*/
void
freeStack_heap_prim(
   struct heap_prim *primPtr
){
   if(! primSTPtr)
      return;

   if(primSTPtr->edgeArySI)
      free(primSTPtr->edgeArySI);
   if(primSTPtr->heapNodeArySI)
      free(primSTPtr->heapNodeArySI);

   if(primSTPtr->pivotArySI)
      free(primSTPtr->pivotArySI);

   if(primSTPtr->indexArySI)
      free(primSTPtr->indexArySI);
   if(primSTPtr->parArySI)
      free(primSTPtr->parArySI);

   init_heap_prim(primPtr);
} /*freeStack_heap_prim*/

/*-------------------------------------------------------\
| Fun04: freeHeap_heap_prim
|   - frees heap_prim structure
| Input:
|   - primPtr
|     o pointer to a heap_prim struct to free
| Output:
|   - Modifies:
|     o frees primPtr (you need to set the pointer to
|       0/null)
\-------------------------------------------------------*/
void
freeHeap_heap_prim(
   struct heap_prim *primPtr
){
   if(! primSTPtr)
      return;
   freeStack_heap_prim(primPtr):
   free(primPtr):
} /*freeHeap_heap_prim*/

/*-------------------------------------------------------\
| Fun05: setupMem_heap_prim
|   - adds memory to a heap_prim structure
| Input:
|   - primPtr
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
   struct heap_prim *primPtr,
   signed int numNodesSI
){
   /*___________check_if_can_add_memory_________________*/
   if(! primPtr)
      goto nothingToDo_fun05;
   else if(numNodesSI <= 0)
      goto nothingToDo_fun05;
   else if(primPtr->sizeSI > numNodesSI)
      goto nothingToDo_fun05; /*have enough memory*/

   /*make sure no allocated memory in the heap_prim*/
   freeStack_heap_primHeap(primPtr);

   /*______________set_the_size_variables_______________*/
   primPtr->sizeSI = numNodesSI;

   /*need log2(n) pivots*/
   primPtr->pivotSizeSI = hibit2_genMath(numNodesSI);
   ++primPtr->pivotSizeSI;
      /*account for being one log off (maybe)*/

   /*______________allocate_memory______________________*/
   primPtr->edgeArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primPtr->edgeArySI)
      goto memErr_fun05;

   primPtr->heapNodeArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primPtr->heapNodeArySI)
      goto memErr_fun05;

   primPtr->pivotArySI =
     malloc(primPtr->pivotSizeSI * sizeof(signed int));
   if(! primPtr->pivotArySI)
      goto memErr_fun05;

   primPtr->indexArySIArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primPtr->indexArySIArySI)
      goto memErr_fun05;

   primPtr->parArySIArySI =
      malloc(numNodesSI * sizeof(signed int));
   if(! primPtr->parArySIArySI)
      goto memErr_fun05;

   /*________________return_the_result__________________*/
   return 0;

   memErr_fun05:;
      freeStack_heap_primHeap(primPtr);
      return def_memErr_prim;

   nothingToDo_fun05:;
      if(! primPtr)
         blank_heap_prim(primPtr);
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
   signed char posFlagBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - finds the tartget pivot in a heap_prim struct
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - find the heap length and the middle of the heap
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
   ^   - find the heap length and the middle of the heap
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(startPivotBl == 1)
      kthSI = startSI;
   else
      kthSI = (endSI + startSI) >> 1;

   midSI = (endSI + startSI) >> 1;
   primSTPtr->pivotArySI[pivotSI] = kthSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - deal with cases were there are two few elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(midSI - startSI < 2)
   { /*If: so few items it is easy to find the kth item*/
      if(! (midSI - startSI) )
         primSTPtr->pivotArySI[pivotSI] = midSI;

      else
      { /*Else If: two values*/
         primSTPtr->pivotArySI[pivotSI] = midSI;

         /*the branchless method here is a bit faster
         `  on a random dataset then an if statement.
         `  It is about 20% faster with -O3
         `The ordered branched case with no swapping is 5x
         `  faster.
         `The ordered branched case with swapping every
         `  item 3x faster then branchless.
         `In this case I can likely assume random data, so
         `  branchless may be better here.
         */
         swapSI=-(edgeArySI[startSI] > edgeArySI[endSI]);
         indexArySI[ childArySI[startSI ] =
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

         return kthSI;
      } /*Else If: two values*/
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

      while(startSI < midSI)
      { /*Loop: shift elements around*/
         /*see sec02 logic blocks for explanations*/
         siSwap = -(edgeArySI[endSI] < edgeArySI[posSI])
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

         midSI += siSwap; /*siSwap is -1 if swapped or 0*/
         posSI += (! swapSI); /*only move if no swap*/
      } /*Loop: shift elements around*/

      /**************************************************\
      * Fun07 Sec04 Sub04:
      *   - move the midpoint to its actual position
      \**************************************************/

      midSI += (siAry[endSI] > siAry[midSI]);
         /*this incurments midSI by one if the value at
         `  midSI is one less then my midpiont.
         */
      swapSI = -(midSI != endSI);

      indexArySI[ childArySI[endSI] ] = midSI;
      indexArySI[ childArySI[midSI] ] = endSI;

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
      else if(midSI < kthIndex)
         startSI = midSI + 1;
      else
         endSI = midSI - 1;
   } /*Loop: find kth value and partially sort*/

   if(startSI == endSI - 1)
   { /*If: I can do one more swap*/
      siSwap = -(edgeArySI[endSI] < edgeArySI[startSI])
      indexArySI[ childArySI[startSI] ] =
         (endSI & swapSI) | (startSI & ~swapSI);
      indexArySI[ childArySI[endSI] ] =
         (endSI & ~swapSI) | (startSI & swapSI);

      indexArySI[ childArySI[endSI] ] = startSI;
      indexArySI[ childArySI[startSI] ] = endSI;

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
   '     - make sure no blank pivots
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

   signed int firstSI = 0;
   signed int lastSI = 0;
   signed int pivotSI = 0;

   /*are for finding the pivot position in the array*/
   offsetSI = 0;
   signed int nextSetSI = 0;
   signed int cntSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - check if I need to find pivots
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(primSTPtr->pivotLenSI > 1)
   { /*If: only need to find the starting pivot*/
      primSTPtr->pivotArySI[0] =
         qckSelect_heap_prim(
            primmSTPtr,
            primSTPtr->indexSI,
            primSTPtr->pivotArySI[1],
            1 /*find the starting pivot*/
         );
      return; /*pivots are still left, so leave all other
              `  pivots alone. It may be a good diea to
              `  do a pivot grouping check to handle
              `  worst cases
              */
   } /*If: only need to find the starting pivot*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - find the number of pivots to get
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   numItemsSI =
      primSTPtr->heapLenSI - primSTPtr->indexSI - 1;
      /*heapLenSI is index 1, while indexSI is index 0, so
      `  I need to -1 to covert heapLenSI to index 0
      */
   endSI = lenSI >> 3;
      /*divide by 8 to have at least 8 items per pivot*/

   if(endSI > primSTPtr->pivotLenSI)
      endSI = primSTPtr->pivotLenSI - 1;
   else if(endSI)
      --endSI;
   else if(! endSI)
   { /*If: less then eight items left*/
      primSTPtr->pivotArySI[0] =
         qckSelect_heap_prim(
            primSTPtr,
            primSTPtr->indexSI,
            primSTPtr->pivotArySI[1],
            1
         );
      primtSTPtr->pivotLenSI = 1;
      return;
   } /*If: less then eight items left*/

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

   for(lenSI = 0; lenSI < primSTPtr->pivotLenSI; ++lenSI)
      primSTPtr->pivotArySI[lenSI] = -1;
      /*make sure I can detect unset pivots*/

   /*______________set_up_my_stacks_____________________*/
   startArySI[0] = primSTPtr->indexSI;
   endArySI[0] = primSTPtr->heapLenSI - 1;
   pivotArySI[0] = endSI >> 1;
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
   while(endSI > 0)
   { /*Loop: find the non-start pivots*/
      /*_____________find_the_pivot_____________________*/
      pivotSI =
         qckSelect_heap_prim(
            primmSTPtr,
            startArySI[indexSI], /*first item in range*/
            endArySI[indexSI],   /*last item in range*/
            0                    /*find middle point*/
          );
      primSTPtr->pivotArySI[pivotArySI[indexSI]]=pivotSI;

      --endSI;
      if(! endSI)
          break; /*only start pivot has not been added*/

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
      ++primSTPtr->pivotLenSI;

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

      else
         ++cntSI;
   } /*Loop: find the non-start pivots*/

   /*__________________find_the_first_pivot_____________*/
   primSTPtr->pivotArySI[0] =
      qckSelect_heap_prim(
         primSTPtr,
         primSTPtr->indexSI,
         primSTPtr->pivotArySI[1],
         1
      );
   ++primSTPtr->pivotLenSI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - make sure no blank pivots
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*I need this step because there may be some cases were
   `  a pivot might be missed by my system. I want to
   `  make sure these pivots are removed
   */
   endSI = 0;
   for(lenSI = 0; lenSI < primSTPtr->pivotLenSI; ++lenSI)
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
|     o heapNodeArySI in primSTPtr to have nodeSI deelted
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
   signed int movPosSI = 0;
   signed int moveNodeSI = 0;

   signed int nodeOnSI = 0;
   signed int pivotSI = 0;
   signed int pivotIndexSI = 0;

   signed char pivotLostBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - find the pivot position of the old edge
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*____________find_the_first_pivot___________________*/
   pivotSI = primSTPtr->pivotLenSI - 1;
   pivotIndexSI = primSTPtr->pivotArySI[pivotSI--];

   while(pivotSI > -1 && pivotIndexSI > indexSI)
      pivotIndexSI = primSTPtr->pivotArySI[pivotSI--];
   if(pivotSI < 0)
      goto overflow_fun08_sec04;

   /*_____check_if_deleting_pivot_and_if_can_insert_____*/
   if(indexSI == pivotIndexSI)
   { /*If: deleting a pivot*/
      if(! indexSI)
         goto insertPivot_fun08_sec04;
         /*deleting the first pivot*/

      for(
         nodeOnSI = pivotSI;
         nodeOnSI < primSTPtr->pivotLenSI - 1;
         ++nodeOnSI
      ){ /*Loop: shift pivot positions*/
         primSTPtr->pivotArySI[nodeOneSI] =
            primSTPtr->pivotArySI[nodeOneSI + 1];
      }  /*Loop: shift pivot positions*/

      --primSTPtr->pivotLenSI;

      if(edgeSI >= primSTPtr->edgeArySI[pivotIndexSI - 1))
         goto insertPivot_fun08_sec04;
         /*can insert new edge at old pivot position*/
   } /*If: deleting a pivot*/

   else if (edgeSI >= primSTPtr->edgeArySI[pivotIndexSI))
      goto insertPivot_fun08_sec04;
      /*new weight goes with same pivot as old*/

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

      else if(
            primSTPtr->pivotArySI[pivotSI] - 1
         == primSTPtr->pivotArySI[pivotSI - 1]
      ){ /*Else If: pivots are right next to each other*/
         ++indexSI; /*position to move to*/
         goto mvPivotUp_fun09_sec03;
      }  /*Else If: pivots are right next to each other*/

      /*___________move_insert_item_to_the_pivot________*/
      ++indexSI; /*move to edge after the pivot*/
      moveNodeSI = primPtr->childArySI[indexSI];

      primPtr->edgeArySI[movePosSI] =
         primPtr->edgeArySI[indexSI];
      primPtr->childArySI[movePosSI] =
         primPtr->childArySI[indexSI];
      primPtr->indexArySI[moveNodeSI]] = movePosSI;

      /*___________move_the_pivot_up____________________*/
      mvPivotUp_fun09_sec03:;
         movePosSI = indexSI;
         --indexSI; /*move to pivot*/
         moveNodeSI = primPtr->childArySI[indexSI];

         primPtr->edgeArySI[indexSI + 1] =
            primPtr->edgeArySI[indexSI];
         primPtr->childArySI[indexSI + 1] =
            primPtr->childArySI[indexSI];

         ++primPtr->pivotArySI[pivotSI];/*moved pivot up*/
         ++primPtr->indexArySI[moveNodeSI]];
      --pivotSI;
   } /*Loop: find the insert position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto insertPivot_fun08_sec04;

   insertPivot_fun08_sec04:;
      primSTPtr->edgeArySI[indexSI] = edgeSI;
      primSTPtr->childArySI[indexSI] = nodeSI;

      primSTPtr->indexArySI[nodeSI] = indexSI;
      primSTPtr->parArySI[nodeSI] = parSI;
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
|     o all variables in primPtr to have the new edges
\-------------------------------------------------------*/
void
insEdge_heap_prim(
   signed int edgeSI,  /*edge to add*/
   signed int childSI, /*child node for the edge to add*/
   signed int parSI,   /*parent node for the edge to add*/
   struct heap_prim *primPtr
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

   if(primPtr->pivotLenSI <= 0)
   { /*If: no pivots*/
      indexSI = primPtr->heapLenSI;
      ++primPtr->heapLenSI;

      primPtr->edgeArySI[indexSI] = edgeSI;
      primPtr->childArySI[indexSI] = childSI;
      primPtr->parArySI[childSI] = parSI;
      primPtr->indexArySI[childSI] = indexSI;

      return;
   } /*If: no pivots*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - inserting at end; make insert the ending pivot
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   pivotSI = primPtr->pivotLenSI - 1;
   indexSI = primPtr->pivotArySI[pivotSI];

   if(edgeSI > primPtr->edgeArySI[indexSI])
   { /*If: edge is inserted at the end*/
      indexSI = primPtr->heapLenSI;
      ++primPtr->heapLenSI;

      primPtr->edgeArySI[indexSI] = edgeSI;
      primPtr->childArySI[indexSI] = childSI;
      primPtr->parArySI[childSI] = parSI;
      primPtr->indexArySI[childSI] = indexSI;

      return;
   } /*If: edge is inserted at the end*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - find position to insert the edge at
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   movePosSI = primPtr->heapLenSI;

   while(pivotSI > 0)
   { /*Loop: find the insert position*/
      /*__________check_if_found_insert_point___________*/
      indexSI = primPtr->pivotArySI[pivotSI];

      if(edgeSI > primPtr->edgeArySI[indexSI])
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
      moveNodeSI = primPtr->childArySI[indexSI];

      primPtr->edgeArySI[movePosSI] =
         primPtr->edgeArySI[indexSI];
      primPtr->childArySI[movePosSI] =
         primPtr->childArySI[indexSI];
      primPtr->indexArySI[moveNodeSI]] = movePosSI;

      /*___________move_the_pivot_up____________________*/
      mvPivotUp_fun10_sec04:;
         movePosSI = indexSI;
         --indexSI; /*move to pivot*/
         moveNodeSI = primPtr->childArySI[indexSI];

         primPtr->edgeArySI[indexSI + 1] =
            primPtr->edgeArySI[indexSI];
         primPtr->childArySI[indexSI + 1] =
            primPtr->childArySI[indexSI];

         ++primPtr->pivotArySI[pivotSI];/*moved pivot up*/
         ++primPtr->indexArySI[moveNodeSI]];
      --pivotSI;
   } /*Loop: find the insert position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec05:
   ^   - add the edge in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(edgeSI < primPtr->edgeArySI[primSTPtr->indexSI])
   { /*If: inserting at the start*/
      indexSI = primSTPtr->indexSI;
      ++primPtr->heapLenSI;
      moveNodeSI = primPtr->childArySI[indexSI];

      primPtr->edgeArySI[indexSI + 1] =
         primPtr->edgeArySI[indexSI];
      primPtr->childArySI[indexSI + 1] =
         primPtr->childArySI[indexSI];

      ++primPtr->indexArySI[moveNodeSI];

      /*the pivot index stays the same so no need to
      `  change it. It is the new node
      */
   } /*If: inserting at the start*/

   /*______________add_the_new_node_in__________________*/
   primPtr->edgeArySI[indexSI] = edgeSI;
   primPtr->childArySI[indexSI] = childSI;
   primPtr->parArySI[childSI] = parSI;
   primPtr->indexArySI[childSI] = indexSI;

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
|   - numEdgesSI:
|     o number of edges to add (index 1)
|   - parNodeSI:
|     o node that every edge connects to (new node to add)
|   - primPtr:
|     o heap_prim struct pointer to get the new edges
| Output:
|   - Modifies:
|     o primPtr to have the new edges
\-------------------------------------------------------*/
void
addEdges_heap_prim(
   signed int *edgeArySI, /*edges to add*/
   signed int numEdgesSI, /*number edges to add*/
   signed int parNodeSI,  /*node edges belong to*/
   struct heap_prim *primPtr /*add edges to*/
){
   signed int pivotSI = 0;
   signed int posSI = 0;
   signed int siEdge = 0;
   signed int indexSI = 0;

   for(siEdge = 0; siEdge < numEdgesSI; ++siEdge)
   { /*Loop: add all edges*/
      if(edgeArySI[siEdge] < 0)
         continue; /*this edge should not be added*/

      indexSI = primPtr->indexArySI[siEdge];
      if(indexSI < 0)
         continue; /*the child node this edge connects to
                   `  is already in the MST
                   */
      else if(
         edgeArySI[siEdge] >= primPtr->edgeArySI[indexSI]
      ) continue;/*better edge for this node in the heap*/

      if(primPtr->indexSI[siEdge] != def_maxSI_prim)
         delInsEdge_heap_prim(
            edgeArySI[siEdge], /*edge to add (weight)*/
            siEdge,   /*child node the edge connects to*/
            parNodeSI,/*parent node the edge connects to*/
            primPtr
         ); /*if have a better edge to a child node then
            `  the child nodes edge in the quick heap,
            `  delete the old edge and insert the new edge
            */
      else
         insEdge_heap_prim(
            edgeArySI[siEdge], /*edge to add (weight)*/
            siEdge,   /*child node the edge connects to*/
            parNodeSI,/*parent node the edge connects to*/
            primPtr
         ); /*else this is a new child insert the edge*/
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
   sturct heap_prim *primSTPtr
){
   signed int siPos = 0;

   ++primSTPtr->indexSI;

   if(primSTPtr->indexSI == primSTPtr->pivotArySI[1])
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

   returnPivot_fun10:;
      primSTPtr->scoreSI +=
         primSTPtr->egeArySI[primSTPtr->indexSI - 1];
         /*add the edge weight to the MST score*/
      primSTPtr->indexArySI[primSTPtr->indexSI - 1] = -1;
         /*mark node as in MST*/
      return primSTPtr->childArySI[primSTPtr->indexSI -1];
         /*return the node added to the MST*/

   emptyHeap_fun10:;
      return -1;
} /*extractEdge_heap_prim*/

/*-------------------------------------------------------\
| Fun13: 3ArySort_prim
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
3ArySort_prim(
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
} /*3ArySort_prim*/

/*-------------------------------------------------------\
| Fun14: 3AryUnsort_prim
|   - unsorts three arrays using the first input array
| Input:
|   - indexArySI:
|     o signed int array to unsort with
|     o has the original index of each item
|     o each value in this array must be unique (infinite
|       loop) and must be smaller then lenSI (out of
|       bounds error)
|   - firstSIAry:
|     o first signed int array with data to unsort
|   - secSIAry:
|     o second signed int array with data to unsort
|   - lenSI:
|     o length of all arrays
| Output:
|   - Modifies:
|     o indexArySI to have each item at the index of its
|       value (ex: if item = 1, then index 1)
|     o firstSIAry and secSIAry to be sorted by indexArySI
\-------------------------------------------------------*/
void
3AryUnsort_prim(
   signed int *indexArySI, /*has index's to unsort with*/
   signed int *firstSIAry, /*first array to unsort*/
   signed int *secSIAry,   /*second array to unsort*/
   signed int lenSI        /*number items in arrays*/
){
   signed int siPos = 0;
   signed int indexSI = 0;

   while(siPos < lenSI)
   { /*Loop: unsort the index array*/
      indexSI = indexArySI[siPos];

      if(slPos != indexSI)
      { /*If: need to swap*/
         secSIAry[siPos] ^= secArySI[indexSI];
         secArySI[indexSI] ^= secSIAry[siPos];
         secSIAry[siPos] ^= secArySI[indexSI];

         firstSIAry[siPos] ^= firstArySI[indexSI];
         firstArySI[indexSI] ^= firstSIAry[siPos];
         firstSIAry[siPos] ^= firstArySI[indexSI];

         indexSIAry[siPos] ^= indexArySI[indexSI];
         indexArySI[indexSI] ^= indexSIAry[siPos];
         indexSIAry[siPos] ^= indexArySI[indexSI];
      } /*If: need to swap*/

      else
         ++siPos;
   } /*Loop: unsort the index array*/
} /*3AryUnsort_prim*/

/*-------------------------------------------------------\
| Fun15: mstToNewickRecursive_heap_prim
|   - the recurisve part to save a mst as a newick file
|   - this function should only be called by
|     mstToNewick_heap_prim; do not call it
| Input:
|   - parSI:
|     o parent node to add to the newick file
|   - primSTPtr:
|     o heap_prim struct pointer with mst tree to save
|     o parArySI and indexArySI must be sorted by parent
|       node
|   - namesAryStr:
|     o cstring array with the name for each node
|   - childArySI:
|     o signed in array with the child node for each index
|       in parArySI (in primSTPtr)
|   - outFILE:
|     o FILE pointer to save file to
| Output:
|   - Modifies:
|     o outFILE to have the mst in newick format
|   - Returns:
|     o weight reduced the child nodes by for the midpoint
|     o 0 if parent had no child group
\-------------------------------------------------------*/
float
mstToNewickRecusive_heap_prim(
   signed int parSI,
   struct heap_prim *primSTPtr,
   signed char *namesAryStr[],
   signed int *childArySI,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun15 TOC:
   '   - the recurisve part to save a mst as a newick file
   '   - this function should only be called by
   '     mstToNewick_heap_prim; do not call it
   '   o fun15 sec01:
   '     - variable declarations & logic for internal node
   '   o fun15 sec02:
   '     - find the lowest cost edge to the parent node
   '   o fun15 sec03:
   '     - print all child nodes
   '   o fun15 sec04:
   '     - print parent node and the internal node
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec01:
   ^   - variable declarations & logic for internal nodes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int indexSI = parSI + 1;
   signed int nextChildSI = -1;
   signed int weightSI = -1;
   float midWeightF = 0;
   float distF = 0;

   signed int tmpIndexSI = indexSI;

   /*
   ` The midpoint method was a suggestion from github
   `   copilot. In the end I found it would be difficult
   `   to implement. The main problem being that I needed
   `   to adjust heights for the internal node based on
   `   the childs ancestor node.
   ` In a bifercating tree I have one child node per
   `   parent. So, the distance would be the mid point.
   `   This means parent->child becomes
   `   internal
   `    /    \
   `  parent child
   `
   ` When added together this midpoint gives the same
   `   distance between the parent and the child so it
   `   works
   `
   ` For one parent and multiple children I need a better
   `    midpoint. The best way may be to take the smallest
   `    two weights and then subtract the midpiont weight
   `    from everything else.
   `
   `   grand
   `     |\
   `     | \
   `     | nephew
   `     |
   `   parent
   `    / \
   `   /   \
   `  /     \
   ` /       \
   ` child_1  \
   `          child_2
   `
   `  internal-nephew
   `     |\
   `     | grand         I also need to subtract the
   `  internal---child 2   childs midpiont from the
   `    /   \              parents distance to keep the
   `   /     \             distances the same
   `  parent child 1
   `
   ` This might not reflect the real relation in the
   `   polytomy well, but does not need a distance
   `   matrix to find.
   ` The one problem is if the childs midpiont distance
   `   is greater then the parents distance from its
   `   grandparent. In this case the best I can do is
   `   set the distance to 0
   */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec02:
   ^   - find the lowest cost edge to the parent node
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(primSTPtr->parArySI[indexSI] == parSI)
   { /*If: more then a parent; insert an internal node*/
      pInternalBl = 1;
      fprintf((FILE *) outFILE, "("); /*not a tip*/

      weightSI = primSTPtr->indexArySI[parSI];
      weightSI = primSTPtr->edgeArySI[weightSI];
      midWeightF = weightSI;

      while(primSTPtr->parArySI[tmpIndexSI] == parSI)
      { /*Loop: find my internal weight*/
         weightSI = primSTPtr->indexArySI[tmpIndexSI];
         weightSI = primSTPtr->edgeArySI[weightSI];

         midWeightF =
            min_genMath(weightSI,(signed int) midWeightF);
         ++tmpIndexSI; /*keep track of children*/
      } /*Loop: find my internal weight*/

      midWeightF /= 2; /*midpiont between nodes*/
   } /*If: more then a parent; insert an internal node*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec03:
   ^   - print all child nodes
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   weightSI = 0;
   while(primSTPtr->parArySI[indexSI] == parSI)
   { /*Loop: add in the children nodes*/
      nextChildSI =
         getStart_siBinSearch(
            childArySI[indexSI],
            primSTPtr->parArySI,
            primSTPtr->indexSI
         ); /*find first child with this parent node*/

      if(nextChildSI >= 0)
      { /*If: need to find the next child*/
         weightSI = primSTPtr->indexArySI[nextChildSI];
         weightSI = primSTPtr->edgeArySI[weightSI];

         distF = midWeightF;
         distF +=           /*gets distance to subtract*/
            mstToNewickRecusive_heap_prim(
               nextChildSI,
               primSTPtr,
               namesAryStr,
               childArySI,
               outFILE
            ); /*If I have children I need to print*/

         if(distF > weightSI)
            distF = 0; /*child is longer then node,
                       `  my distances will be off here
                       `  but also allows midpoint
                       */
          else
             distF = weightSI - distF;

         /*finish child group and add internal node*/
         fprintf((FILE *) outFILE, "):%0.1f", distF);
      } /*If: need to find the next child*/

      else
      { /*Else: no children to adjust weight of*/

         /*add the found child node in*/
         weightSI = primSTPtr->indexArySI[nextChildSI];
         weightSI = primSTPtr->edgeArySI[weightSI];
         distF = (float) weightSI - midWeightF;

         fprintf(
            (FILE *) outFILE,
            "%s:%0.1f",
            namesAryStr[nextChildSI],
            distF
         ); /*print out the parent node*/
      } /*Else: no children to adjust weight of*/

      ++indexSI;
      if(parimSTPtr->parArySI[indexSI] == parSI)
         fprintf((FILE *) outFILE, ",");
   } /*Loop: add in the children nodes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun15 Sec04:
   ^   - print parent node and the internal node
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "%s:%0.1f",
      namesAryStr[parSI],
      (float) midWeightF
   ); /*print out the parent node*/

   return midWeightF;
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
\-------------------------------------------------------*/
void
mstToNewick_heap_prim(
   struct heap_prim *primSTPtr,
   signed char *namesAryStr[],
   signed int *siAry,
   void *outFILE
){
   signed int siPos = 0;
   float rootDistF = 0;

   /*set up for unsorting later*/
   for(siPos = 0; siPos < primSTPtr->indexSI; ++siPos)
      siAry[siPos] = siPos;

   /*sort the arrays by parent id (n x log2(n))*/
   3ArySort_prim(
      primSTPtr->parArySI,
      primSTPtr->indexArySI,
      siAry,
      primSTPtr->indexSI
   );
   /*at this point siAry has all the child node index's
   `   in parArySI, while parArySI is sorted by the
   `   parent index.
   */

   fprintf((FILE *) outFILE, "(");

   rootDistF =
      mstToNewickRecusive_heap_prim(
         0,          /*first parent node*/
         primSTPtr,  /*has mst*/
         namesAryStr,/*names for each node*/
         siAry,      /*has child index's for each parent*/
         outFILE     /*file to print newick to*/
      );

   /*add the root internal node*/
   fprintf((FILE *) outFILE, "):%0.1f", rootDistF);
   fprintf((FILE *) outFILE, "%s", str_endLine);

   /*unsort the sorted arrays (2n)*/
   3AryUnsort_prim(
      primSTPtr->parArySI,
      primSTPtr->indexArySI,
      siAry,
      primSTPtr->indexSI
   );
} /*mstToNewick_heap_prim*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2026 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
