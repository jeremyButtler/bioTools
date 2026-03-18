# Use:

Holds the functions to run prims algorithm on input
  distances.

Files: prim.c prim.h

- Depends on:
  - siBinSearch from genSort
  - genMath from genLib.

- Structures:
  - `heap_prim`: structure for the quick heap used
    in the prim MST algorithm

# The heap\_prim structure

The `heap_prim` structure has the variables for making
  a quick heap. For the heap, the parent node is the node
  that is already used. The child node is the node that
  is currently not in the minimum spanning tree.

The prim version of the quick heap is designed to only
  keep nodes that could be used in the MST in the quick
  heap. In this case the size of the heap is always the
  number of nodes times a multiplier
  (`5 * sizeof(signed int)`). To keep the heap size down
  only the smallest edge mapping to a single node is kept.

- Variables:
  - scoreSI: score for the current minimum spanning tree
  - quick heap variables:
    - edgeArySI: signed int array with the quick heap
      edge weights
    - childArySI: signed int array with the child
      nodes that each edge connects to
    - indexSI: index at in the quick heap
      - edges that have been added to the tree (put in
        their final position) are set to `(index +1) * -1`
      - nodes with no connecting edges are set to
        def\_maxSI\_prim
    - heapLenSI: number of nodes in the heap
  - heap pivots variables:
    - pivotArySI: signed int array with the index of each
      pivot (last pivot comes first)
    - pivotLenSI: number of pivots used
  - my custom quick heap support variables:
    - indexArySI: has the index of every edge in the heap
      - each child node is an index in the array
        (`heap->heapIndexArySI[child_node]`)
      - Set to -1 when the node is added to the MST
    - parArySI: signed int array with the parent nodes id
      - each child node is an index in the array
        (`heap->heapIndexArySI[child_node]`)

As a rule let the functions handle the quick heap.

# using a heap\_prim structure

To use a `heap_prim` structure you first need to
  initialize it and then add memory (number of nodes).
  After initialization you can start to add in the edges.

1. Initialize and setup memory
2. Add edges for the new node in
3. Find the minimum edge to find the new node to add
4. Repeat steps 1 to 3 until no more nodes left

When finished you can extract the tree and then free the
  `heap_prim` structure.
  
## Setup a heap\_prim structure

### Setup functions:

You can initialize a `heap_prim` structure using the
  `init_heap_prim` function. Then add memory with the
  `setupMem_heap_prim` function. However, the easier
  way is to call the `mk_heap_prim` function.

The `mk_heap_prim` function takes in the number of
  nodes in the tree. It returns a setup `heap_prim`
  structure pointer. You should free this structure using
  the `freeHeap_heap_prim` function.

### using the mk\_heap\_prim function

The input for the `mk_heap_prim` is the number of nodes
  in you minimum spanning tree. The return value is a
  pointer to the new `heap_prim` structure or 0 for a
  memory error or if you input 0 or 1 for the number of
  nodes.

Example:

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "prim.h"

int
main(
){
   struct heap_prim *primHeapMST = mk_heap_prim(20);
      /*make a heap_primST structure with 20 nodes*/

   if(! primHeapMST)
      return 1; /*memory error*/

   /*your code goes here*/

   /*clean up*/
   freeHeap_heap_prim(primHeapMST);
   primHeapMST = 0;
   return 0;
} /*main*/
```

## building the MST

You can add a node to the heap\_prim structure by using
  the `addEdges_heap_prim` function. You can then extract
  the minimum edge using the `extractEdge_heap_prim`
  fuction.

### addingEdges\_heap\_prim function

The `addEdges_heap_prim` function allows you to add edges
  to your heap.

- Input:
  1. signed int array of edges (weights) to add
     - a weight of -1 means do not add the edge
  2. singed int array with the child each edge is assigned
     to
     - a weight of -1 means do not add the edge
     - this is here so that non-complete graphs will not
       take O(n^^2^^) time. Without this I would have to
       assume each index in the edge array (input 1) is
       the id of the child. This would require me to
       scan through the input array n times.
  3. number of edges to add (length of input 1; index 1)
  4. parent node of all edges
  5. heap\_prim to add the edges to
     - edges are only added if they provide a better
       weight for their child node
- Output:
  - No return value

**Example:**

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "prim.h"

int
main(
){
   signed int zeroEdges[4] = {1, 1, 3, 2}
      /*0-1    each -, \, _, and | is weight of 1
      ` |\__3
      ` | 2
      ` 4
       */
   signed int zeroChildren[4] = {1, 2, 3, 4}
   struct heap_prim *primHeapMST = mk_heap_prim(20);
      /*make a heap_primST structure with 20 nodes*/

   if(! primHeapMST)
      return 1; /*memory error*/

   addEdges_heap_prim(
      zeroEdges,
      zeroChildren,
      5,
      0,
      primheapMSTu
   );
      /*edges, number edges, parent, heap*/

   /*clean up*/
   freeHeap_heap_prim(primHeapMST);
   primHeapMST = 0;
   return 0;
} /*main*/
```

### extractEdge_heap\_prim function

The `extractEdge_heap_prim` function allows you to get the
  minimum edge in the heap. The returned node is the child
  node that was added. After extracting the minimum child
  node you then add the edges for the child edges nodes.

What it really does is move the heap index up one. When
  finished, the heap is the minimum spanning tree.

- Input:
  1. heap\_prim to extract the child node of the minimum
     edge from
- Output:
  - Returns
    - the child node of the next minimum edge
    - -1 if no more edges are in the heap to extract, this
      means your done

**Example:**

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "prim.h"

int
main(
){
   signed int edges[5][4];
   signed int children[5][4];
   signed int nodeSI = 5;
   signed int nodeOnSI = 0;

   signed int childNodeSI = 0;
   signed int weigthSI = 0;
   signed int parentSI = 0;

   struct heap_prim *primHeapMST = mk_heap_prim(5);
      /*make a heap_primST structure with 5 nodes*/

   if(! primHeapMST)
      return 1; /*memory error*/

   /*_______add_the_edges_to_the_tree___________________*/
   /*edges for the parent node (zero)*/
                       /*0-1
   edges[0][0] = 1;    ` |\__3
   edges[0][1] = 1;    ` | 2
   edges[0][2] = 3;    ` 4
   edges[0][3] = 2;     */
   children[0][0] = 1;
   children[0][1] = 2;
   children[0][2] = 3;
   children[0][3] = 4;

   /*edges for the first child node (one)*/
   edges[1][0] = 1;    /*0-1\
                       `  /| 3
   edges[1][1] = 1;    ` | 2  
   edges[1][2] = 1;    ` 4
   edges[1][3] = 2;    */

   children[1][0] = 0;
   children[1][1] = 2;
   children[1][2] = 3;
   children[1][3] = 4;

   /*edges for the second child node (two)*/
   edges[2][0] = 1;   /*0 1
   edges[2][1] = 1;   `  \| 3
                      `   2/ 
   edges[2][2] = 1;   ` 4/
   edges[2][3] = 1;   */

   children[2][0] = 0;
   children[2][1] = 1;
   children[2][2] = 3;
   children[2][3] = 4;

   /*edges for the third child node (three)*/
   edges[3][0] = 3;   /*0 1\
   edges[3][1] = 1;   `  \--3
   edges[3][2] = 1;   `   2/  
                      ` 4-+ 
   edges[3][3] = 3;   */

   children[3][0] = 0;
   children[3][1] = 1;
   children[3][2] = 2;
   children[3][3] = 4;

   /*edges for the fourth child node (four)*/
   edges[4][0] = 2;   /*0 1
   edges[4][1] = 1;   ` | +-3
   edges[4][2] = 3;   ` |/2/  
   edges[4][3] = 3;   ` 4/-+ 
                      */
   children[4][0] = 0;
   children[4][1] = 1;
   children[4][2] = 2;
   children[4][3] = 3;

   /*_______find_the_minimum_spanning_tree______________*/
   childNodeSI = 0;
   for(nodeOnSI = 0; nodeOnSI < nodeSI; ++nodeOnSI)
   { /*Loop: add all edges to the tree*/
         addEdges_heap_prim(
            edges[childNodeSI], /*edges to add in*/
            children[childNodeSI], /*children for edges*/
            4,                  /*4 edges in the arrays*/
            childNodeSi,        /*new parent node*/
            primHeapMST
         );

      /*____________get_the_next_parent_node_to_add_____*/
      childNodeSI = extractEdge_heap_prim(primHeapMST);

      if(childNodeSI < 0)
         break; /*done*/
   } /*Loop: add all edges to the tree*/


   /*_______print_the_minimum_spanning_tree_____________*/
   /*this is more of edges in the MST, A full MST would
   `  be much harder to print since one parent may have
   `  multiple children.
   */
   printf("MST score: %i\n", primHeapMST->scoreSI);
   for(
      nodeOnSI = 0;
      nodeOnSI < primHeapMST->indexSI;
      ++nodeOnSI
    ){ /*Loop: print the minimum spanning tree*/
        childNodeSI = primHeapMST->childArySI[nodeOnSI];
        parentSI = primHeapMST->parArySI[childNodeSI];
        weightSI = primHeapMST->edgeArySI[nodeOnSI];

        printf(
           "%i->%i with weight of %i\n",
           childNodeSI,
           parentSI,
           weightSI
        );
    }  /*Loop: print the minimum spanning tree*/
      
   /*___________________clean_up________________________*/
   freeHeap_heap_prim(primHeapMST);
   primHeapMST = 0;
   return 0;
} /*main*/
```

## printing a heap\_prim structure

You can print a `heap_prim` structure to a newick file
  using the `mstToNewick_heap_prim` function. The newick
  function will add the internal nodes to the MST.

Each internal node is found by taking the midpoint between
  the parent and child edge. If there is more then one
  child for a parent, then the smallest edge is used. The
  midpoint from the smallest edge is then subtracted from
  all edges.

```
         parent
         /  | \
        /   |  \
    child_1 |   \
            |  child_3
          child_2

Child one has the smallest edge (distance of 2). So, I
  divide child 1's edge by two (2/2) to get one. I then
  subtract 1 from every child and add one to the parent.

       internal-parent
       /     | \
    child_1  |  \
             |  child_3
           child_2
```

To deal with the distance bewtween the grand parent and
  being off we return the midpiont distance back and
  subtract it from the parents internal node distance. If
  this would result in a negative number, the parents
  internal node distance is set to 0. This could result
  in some distances looking smaller then they really are,
  but is the best I can do without setting the parents
  distance to 0.

- Input:
  1. a pointer to a heap\_prim structure to print to a
     newick file
  2. cstring array with names to print for each node in
     input 1
  3. signed int array to use in sorting and unsorting
     - The length needs to be the number of nodes
     - Use the array you used with the
       `addEdges_heap_prim` function
  4. A file to print to (FILE pionter)
- Output:
  - Prints heap\_prim structure to the file in input 4
    as a newick file

A quick, untested example for:

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <time.h>
#include "../genSortAndHeap/prim.h"

int
main(
){
   signed char namesAryStr[20][2];
   signed int siIndex = 0;
   signed int childSI = 0;
   signed int siEdge = 0;
   signed int distanceArySI[20];
   signed int childArySI[20];
   signed int numEdgesSI = 0; /*number edges added*/

   FILE *outFILE = 0;
   struct heap_prim primStackMST;

   /*_______initalize_add_memory_open_output_file_______*/
   init_heap_prim(&primStackMST);

   if( setupMem_heap_prim(&primStackMST, 20) )
      goto err_main; /*memory error*/

   outFILE = fopen("delete.newick", "w");
   if(! outFILE)
      goto err_main; /*could not open the file*/

   /*___________build_a_random_20_node tree_____________*/
   for(siIndex = 0; siIndex < 20; ++siIndex)
   { /*Loop: create some names*/
      namesAryStr[siIndex][0] = siIndex + 65;
      namesAryStr[siIndex][1] = 0;
   } /*Loop: create some names*/

   srand(tmie(NULL)); /*set a random seed*/

   for(siIndex = 0; siIndex < 20; ++siIndex)
   { /*Loop: build a random tree*/

      numEdgesSI = 0;
      for(siEdge = 0; siEdge < 20; ++siEdge)
      { /*Loop: get new edges to add*/
         if(primStackMST.indexArySI[siEdge] < 0)
            continue; /*edge is in tree as a parent*/

         distanceArySI[numEdgesSI] = rand() % 100;
            /*add in random weights for each edge*/
         childArySI[numEdgesSI] = siEdge;
         ++numEdgesSI;
      } /*Loop: get new edges to add*/

      addEdges_heap_primt(
         distanceArySI, /*distance between nodes*/
         childArySI,    /*child node of each edge*/
         numEdgesSI,    /*number of new edges*/
         childSI,       /*parent node I am on; this is
                        ` the last rounds child
                        */
         &primStackMST
      );

      /*get the next minimum edge in the tree*/
      childSI = extractEdge_heap_prim(&primStackMST);

      if(childSI < 0)
         break;
         /*finished the MST; for this case it should not
         `  happen until all 20 nodes are added
         */
   } /*Loop: build a random tree*/

   /*________________save_tree_as_a_newick_file_________*/
   mstToNewick_heap_prim(
      &primStackMST,
      namesAryStr,
      distanceArySI[20],
      outFILE
   );

   /*________________clean_up_and_return________________*/
   siIndex = 0;
   goto cleanUp_main;

   err_main:;
      siIndex = 1;
      goto cleanUp_main;
   cleanUp_main:;
      freeStack_heap_prim(&primStackMST);

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return indexSI;
} /*main*/
```

## freeing a heap\_prim structure

When you are finished with your `heap_prim` you will
  need to free it. If the structure is on the stack you
  will use the `freeStack_heap_prim` function. If your
  structure is on the heap you will use
  the `freeHeap_heap_prim` function.

### the freeStack function:

The freeStack function will free all variables inside the
  structure. It will also initialize all values to 0.

A quick example for using freeStack:

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "../genSortAndHeap/prim.h"

int
main(
){
   struct heap_prim primHeapMST;

   /*initialize and add memory*/
   init_heap_prim(&primHeapMST);
   if( setupMem_heap_prim(&primHeapMST, 20) )
      return 1; /*memory error*/

   /*free the variables in the structure (stack free)*/
   freeStack_heap_prim(&primHeapMST);
   return 0;
} /*main*/
```

### the freeHeap function:

The freeHeap function will free the structure your pointer
  points to. However, it will not set the pointer to 0.
  After calling freeHeap, you should set the structure
  pointer to 0.

Quick example of doing a heap free:

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "../genSortAndHeap/prim.h"

int
main(
){
   struct heap_prim *primHeapMST = mk_heap_prim(20);

   if(! primHeapMST)
      return 1; /*memory error*/

   freeHeap_heap_prim(primHeapMST);
   primHeapMST = 0;
   return 0;
} /*main*/
```

# How works

## Intro to how works

This Prim algorithm uses a slightly modified quick heap
  from:

Gonzalo Navarro and Rodrigo Paredes. 2010. On Sorting,
  Heaps, and Minimum Spanning Trees.
  Algorithmica 57, 4 (August 2010), 585–620.
  Accessed from:
  [https://users.dcc.uchile.cl/~raparede/publ/09algorIQS.pdf](
   https://users.dcc.uchile.cl/~raparede/publ/09algorIQS.pdf)

I wanted to keep memory down to the number of nodes (n).
  So, I modified the quick heap to include a delete
  function when inserting a new edge (e). The delete
  function only fires when the new edge has a lower weight
  then the edge in the heap connecting the same node. The
  one problem about this system is the pivots can get out
  of balance really easily.

## Step by step list of how works

1. Copy the edges from the first node
2. Add in missing pivots
   - If no pivots, add in pivots O(n^2 x log2(n))
     - since phylogenetic trees have n^2 edges, the cost
       is O(number edges * log2(n)), which is about cost
       of sorting
       - quick select n + n - 1 + n - 2 + ... = n^2 times
   - pivots are inserted at equal spaced distances. I have
     debated about increasing the distance by 2x between
     each pivot
     - ex: pivot:1 8 edges, pivot:2 16 edges, pivot:3 32
           edges, ...
   - Else: only insert starting pivot O(n^2)
     - quick select n + n - 1 + n - 2 + ... = n^2 times
3. Remove the first (minimum) pivot (edge) by incrementing
   the heap index
   - The array having the index of the edge in the heap is
     set to -1
4. Add the edges from the minimum pivots child node into
   the heap
   a. Check if new edge has a lower weight to its child
      node then the edge in the heap
      - if the child node is already in MST, move to the
        next edge
      - if new edge has a higher weight move to the next
        edge
      - if yes, but the child node is new, move to step c
      - if yes and already have an edge to the child node,
        move to step b
      - Thanks to the heap index array this is a cost of
        O(1) [two to three look ups] per edge
   b. Delete the heap edge that shares the same child node
      and insert the new weight
      - Delete always assumes the new edge costs less then
        the old edge
      - I first find the pivot before the delete edge.
        Then I use the insert insert method to insert the
        new edge (using the deleted edge as the
        replacement)
        - If delete keeps edge at or near same pivot,
          the cost is O(1) (replace old cost with new)
        - Else the cost is O(2 x log2(n))
      - If the deleted edge was a pivot
        - If first pivot, replace weight (better) and keep
          the firts pivot cost O(1)
        - If other pivot, delete the other pivot
          - If weight of edge is greater then previous
            pivot replace with the new edge O(1)
          - else find insert point O(log2(n))
   c. Insert the new edge
      - Move pivots greater then edge one up
        - Update the heap index and pivot array for the
          new pivots position
      - Move the first value before the moved pivot
        - Update the heap index for the moved edges
          position
      - Insert new edge into moved edge spot
      - Repeat until the next pivot is less then the new
        edge
      - cost of O(2 x log2(n))
5. Repeat steps 2 to 4 until the heap index is equal to n
6. Extract the MST tree (is the heap)

## Flow diagram of how works

```
                 Copy nodes [n]
                     |
                     v
+----------->Add in missing pivots
|                    |           |
|             plenty of pivots   no pivots left
|                    |           |
|             add start pivot    add all pivots in
|                    |           | The worst overall case
|                    +-----------+ is if all pivots were
|                    |             deleted each round,
|                    |             which would make me
|                    |             find all pivots. Cost
|                    |             log2(n)*(n + n-1 + ...)
|                    |             = n^2 * log2(n)
|                    |             for phyogentic trees I
|                    v             always have n^2 edges
|   Extract the minimum edge and get its child node [1]
|           (index of edge is marked with a -1)
|                    |
|                    v
|         increment the heap position [1]
|                    |
|                    v
|  Add an edge from the child node to the heap
|  ^                 | number edges * (6 * log2[n])
|  |                 v
|  |   check if new child node is in graph [1]
|  |     |                      |
|  |    no                     yes
|  |     |                      |
|  |   insert new edge    new edge has a lower weight?
|  |     |  [2 * log2(n)]  | [1]               |
|  |     |                no                  yes
|  |     |                 |                   |
|  |     |                 |             delete and insert
|  |     |                 |                   | 2*log2[n]
|  |     +->child node still has edges to add<-+
|  |            |    |
|  +-----------yes   no
|                    |
|                    v
|             edges left in heap
|                |   |
|               yes  no
|                |   |
+----------------+   |
                     |
                     v
          Extract the minimum spanning tree [n]
```

## Example showing the insert operation

```
A [number] is a pivot position

Starting heap:
Heap:          [1]-3-2-[5]-6-[7]-11-8-10
node in index: {1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11}
Index:         {0, 2, 1, -1, 3, 4, 5, 7, -1,  8,  6}

Inserting 4; 4 < 7 so move 11 to the end
Heap:          [1]-3-2-[5]-6-[7]-_-8-10-11
node in index: {1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11}
Index:         {0, 2, 1, -1, 3, 4, 5, 7, -1,  8,  9}

Move pivot 7 to 11's old spot to make room for 4
Heap:          [1]-3-2-[5]-6-_-[7]-8-10-11
node in index: {1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11}
Index:         {0, 2, 1, -1, 3, 4, 6, 7, -1,  8,  9}

4 < 5; move 7 to 7's old spot to make room for 4
Heap:          [1]-3-2-[5]-_-6-[7]-8-10-11
node in index: {1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11}
Index:         {0, 2, 1, -1, 3, 5, 6, 7, -1,  8,  9}

move 5 to 6's old spot to make room for 4
Heap:          [1]-3-2-_-[5]-6-[7]-8-10-11
node in index: {1, 2, 3, 4,  5, 6, 7, 8,  9, 10, 11}
Index:         {0, 2, 1, -1, 4, 5, 6, 7, -1,  8,  9}

4 > 1; move 4 to 5's old spot
Heap:          [1]-3-2-4-[5]-6-[7]-8-10-11
node in index: {1, 2, 3, 4, 5, 6, 7, 8,  9, 10, 11}
Index:         {0, 2, 1, 3, 4, 5, 6, 7, -1,  8,  9}
```

## Example showing the delete operation

For deletes, I always know the index of the node. I can
  find the nearest pivot using a loop to search the pivot
  list. After finding the pivot before the new edge I
  can inser the edge. For delete I always assume thew
  new edge is better (lower cost/smaller weight).

```
A [number] is a pivot position

Goal: Replace child 5's edge with a new that edge costs 3
  instead of 6.

Starting heap:
Heap (weight): [1]-2-[4]-7-6-[8]-11-10
child node:    {1, 2, 3, 4,5, 6,  7, 8,  9}
Index:         {0, 1, 2, 3,4, 5,  6, 7, -1}
length: 9                  ^---edge to replace

Find pivot of child 4:
Heap (weight): [1]-2-[4]-7-6-[8]-11-10
child node:    {1, 2, 3, 4,5, 6,  7, 8,  9}
Index:         {0, 1, 2, 3,4, 5,  6, 7, -1}
Pivot:                ^---pivot before child 5
length: 9

Check if pivot is less then new edge cost:
4 > 3; new edge costs less

Replace child's 5's edge with child 4's edge (cost 7)
Heap weight:   [1]-2-[4]-7-7-[8]-11-10
child node:    {1, 2, 3, 4,4, 6,  7, 8,  9}
Index:         {0, 1, 2, 3,4, 5,  6, 7, -1}
Pivot:                ^---pivot before child 5
length: 9

Move pivot one edge up:
Heap (weight): [1]-2-4-[4]-7-[8]-11-10
child node:    {1, 2,3, 3, 4, 6,  7, 8,  9}
Index:         {0, 1,2, 3, 4, 5,  6, 7, -1}
Pivot:                  ^---pivot before child 5
length: 9

Check if next pivot is less then edge
   1 < 3; new edge cost more then the next pivot

Overwirte the old pivot with the new edge
Move pivot one edge up:
Heap (weight): [1]-2-3-[4]-7-[8]-11-10
child node:    {1, 2,5, 3, 4, 6,  7, 8,  9}
Index:         {0, 1,2, 3, 4, 5,  6, 7, -1}
Pivot:               ^---child insereted in new position
length: 9
```

## Example of prims for a simple complete graph

For example a 4 node complete graph. In a complete graph
  every node shares an edge with every other node. This
  is a very simple example.

```
  3
 /|\
1-+-4
 \|/
  2
```

Weights of each edge (n^2/2) matrix:

```
{1->4=3, 1->2=1, 1->3=1, 2->3=3, 2->4=1, 3->4=1}
```

1. Copy the edges from the first node
   - add each child node to the node heap
   - add each edge weight to the weight heap
   - add the parent node to the heap
   - add each edges index in the head to the heap index
     array (at index of the child node)
   - cost: `4n * sizeof(signed int)`

```
- Heap node numbers:  {1, 2, 3, 4}
- Heap edge weights:  {0, 1, 1, 3}
- Heap parent node:   {0, 1, 1, 1}
- Index of each heap: {0, 1, 2, 3}
```

2. Add in missing pivots
   - use quick select to find the pivots
   - cost
     - first round is `log2(n) * n`
     - next rounds will vary, but are either 1 (all pivots
       are set) or at least log2(n)

In this example the array is already sorted, so quick
  select only selects the pivots.

Number of pivots = log2(4) = 2 pivots

```
- Heap node numbers:  {1, 2, 3, 4}
- Heap edge weights:  {0, 1, 1, 3}
- Heap parent node:   {0, 1, 1, 1}
- Index of each heap: {0, 1, 2, 3}
- pivots: {1, 3}
- first edge index: 1
```

3. Remove the first (minimum) pivot by moving one item
   up in the index
   - use quick select to replace the pivot
     - if lost, it will be added back in in step 2

```
- Heap node numbers:  {1, 2, 3, 4}
- Heap edge weights:  {0, 1, 1, 3}
- Heap parent node:   {0, 1, 1, 1}
- Index of each heap: {0, 1, 2, 3}
- pivots: {2, 3}
- first edge index: 2
```

4. Add in the nodes edges
   - if edge to existing node has a lesser weight, add it
     in and delete the old edge
   - else do nothing

a. Check if I can insert an edge (lower weight to a node)
   - If I can insert an edge, delete the old parent edge

Node 2 edges: {2->4=1, 2->3=3}
I need to delete node 4.

I ignore edge node 3, because 2->3 has a weight of three,
  while 1->3 has a weight of 1


```
- Heap node numbers:  {1, 2, 3}
- Heap edge weights:  {0, 1, 1}
- Heap parent node:   {0, 1, 1}
- Index of each heap: {0, 1, 2}
- pivots: {2, na}
- first edge index: 2
```

b. Insert the new edges
   - if no pivots, put the new edge at the end
   - if new edge is less then the smallest pivot, set
     the smallest pivot to the new edge

```
- Heap node numbers:  {1, 2, 3, 4}
- Heap edge weights:  {0, 1, 1, 1}
- Heap parent node:   {0, 1, 1, 2}
- Index of each heap: {0, 1, 2, 3}
- pivots: {2, na}
- first edge index: 2
```

c. Extract the first pivot and If removed all pivots add
   in a new set of pivots with quick select, otherwise
   find the next pivot value for the first pivot. In this
   case there are to few nodes to support two pivots.
   

```
- Heap node numbers:  {1, 2, 3, 4}
- Heap edge weights:  {0, 1, 1, 1}
- Heap parent node:   {0, 1, 1, 2}
- Index of each heap: {0, 1, 2, 3}
- pivots: {3, na}
- first edge index: 3
```

5. Repeat steps 3 and 4 again until no pivots remain

In this case the tree is already built, so no new edges
  are added in. The final result looks like:

```
- Heap node numbers:  {1, 2, 3, 4}
- Heap edge weights:  {0, 1, 1, 1}
- Heap parent node:   {0, 1, 1, 2}
- Index of each heap: {0, 1, 2, 3}
- pivots: {na, na}
- first edge index: 4
```

6. Extract the found MST from the quick heap
   - In this case we just move down the heap

```
MST:    1->2->3->4
Weight:  1  1   1
Total weight: 3
```
