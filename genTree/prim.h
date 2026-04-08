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

/*-------------------------------------------------------\
| Header:
|   - guards for people who call .h files in .h files
\-------------------------------------------------------*/

#ifndef PRIMS_MINIMUM_SPANNING_TREE_H
#define PRIMS_MINIMUM_SPANNING_TREE_H

#define def_memErr_prim -1
#define def_overflowErr_prim -2

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
   signed int *edgeArySI;  /*has quick heap*/
   signed int *childArySI; /*child nodes for each edge*/

   signed int indexSI;       /*index at in quick heap*/
   signed int heapLenSI; /*number of nodes in the heaps*/

   /*______________pivots_for_heap_size_log2(n)_________*/
   signed int *pivotArySI;   /*index of each pivot*/
      /*0 is first index, while pivotLenSI is last index*/
   signed int pivotLenSI;    /*number of pivots*/
   signed int pivotSizeSI;   /*max pivots*/

   /*______________non-heap_tracking size_n_____________*/
   signed int *indexArySI;
      /*heas the index of each child node (not in MST) in
      `  the heap. If the edge has been assigned to its
      `  final index the value is set to (index + 1) * -1
      `The +1 is to convert 0 to -1
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
|   - sets all values used in indexArySI to def_maxSI_prim
\-------------------------------------------------------*/
void
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
void
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
   struct heap_prim *primSTPtr
);

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
);

#endif

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
