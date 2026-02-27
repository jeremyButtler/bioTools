/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' prim SOF: Start Of File
'   - uses prims algorithm to find a minimum spanning tree
'   o header:
'     - guards for people who call .h files in .h files
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

/*-------------------------------------------------------\
| Header:
|   - guards for people who call .h files in .h files
\-------------------------------------------------------*/

#ifndef PRIMS_MINIMUM_SPANNING_TREE_H
#define PRIMS_MINIMUM_SPANNING_TREE_H

#define def_memErr_prim -1
#define def_overlfowErr_prim -2

#define def_maxSI_prim ( (signed int) (((unsigned int) -1) >> 1) )
   /*maximum value for a signed int*/

/*-------------------------------------------------------\
| ST01: heap_prim
|   - has variables to run a quick heap for prims MST
\-------------------------------------------------------*/

typedef struct
heap_prim
{
   /*_______________heap_variables_size_n______________*/
   signed int *edgeArySI;/*has quick heap*/
   signed int *childArySI; /*child nodes for each edge*/

   signed int indexSI;       /*index at in quick heap*/
   signed int heapLenSI; /*number of nodes in the heaps*/

   /*______________pivots_for_heap_size_log2(n)_________*/
   signed int *pivotArySI;   /*index of each pivot*/
      /*0 is first index, while pivotLenSI is last index*/
   signed int pivotLenSI;    /*number of pivots*/
   signed int pivotSizeSI;   /*max pivots*/

   /*______________non-heap_tracking size_n_____________*/
   signed int *indexArySI:
      /*heas the index of each child node (not in MST) in
      `  the heap
      */
  signed int *parArySI;
     /*has the parent nodes (already in MST) id for each
     `  edge
     */

   signed int sizeSI; /*max nodes*/
   signed int scoreSI; /*score of minimum spanning tree*/
}heap_prim;

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif

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
