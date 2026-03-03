/*has headers for the hidden functions I am testing in
`  prim.c
*/

struct heap_prim;

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
);

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
);

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
);

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
);
