# Goal:

Holds functions to easily run my prims algorithm in prim.h

Files: runPrim.c and runPrim.h

- Depends on:
  - prim
  - fileFun from genLib
  - ulCp from genLib
- Hidden dependencies
  - siBinSearch from genSort
  - genMath from genLib
  - endLine from genLib

# Flow:

The flow of runPrim is to first index a file and create
  the main structure with the `indexFeatureFile_runPrim`
  function. After the file is indexed you can build a
  minimum spanning tree with the `build_mst_runPrim`
  function. After building the tree you can print the
  tree to a newick file with `mstToNewick_mst_runPrim`.

When finished you can free your mst\_runPrim structure
  with the `freeHeap_mst_runPrim` function. You can also
  reset the mst tree with the `blank_mst_runPrim`
  function. After calling the blank function you can
  recall the `build_mst_runPrim` function to rebuild the
  tree.

# The feature file

The feature file is a tsv (tab or space deliminated) used
  to find the distance between the different lineages. The
  first line is the header. Each line after the header has
  each genome. There can be no blank cells in the feature
  file. If you are missing a feature/lineage, then use a
  `*`, `NA`, or `na` to mark a missing entry
  (feature/lineage).

**An example of a feature file (this one uses spaces)**

```
id        locus_1  locus_2  locus_3	             ...
US_Bax    1        3        odd_ball             ...
CA_Bax    3        *        California           ...
Mars_Bax  na       10       not_from_this_planet ...
AZ_Bax    1        3        odd_ball             ...
```

As a rule; do not use DNA or amino acid sequences. The
  minimum spanning tree is not a good tool for either.
  It is missing models to account for different evolution
  rates. Instead use a maximum likelihood or Bayesian
  method.

Use a minimum spanning tree for data that is hard to
  model, such as lineages. This may not be the best
  version, especially since it does not boot strap.

# Structure

The mst_runPrim structure has the minimum spanning tree,
  the node names, and the file index.

- Variables
  - mstST: is a heap\_prim structure pointer (see
    using_prim.h) that gets the minimum spanning tree
  - nameAryStr: c-string pointer array with all the node
    names (index 0 is the first node)
  - indexArySL: signed long array with the index of every
    line in the file (index 0 is the header, index 1 is
    the first node)
  - nodeLenSL: number of lines in the file (index 0 is the
    header; index 1 is the first node)
  - maxLineLenSL: is the number of bytes in the longest
    line in the file
  - distArySI: this is a temporary int array with the
    length of nodeLenSL. This is used by
    `mstToNewick_mst_runPrim` and `build_mst_runPrim`

# Functions:

## indexFeatureFile\_runPrim

The first step in making a MST tree is to get the index
  of the file. You can index the file using the
  `indexFeatureFile_runPrim` function.

- Input:
  1. FILE pointer to the feature table to index and get
    distances from (later step)
  2. signed char pointer to get the error
- Output:
  - Modifies:
    - input file (input 1) to be at the start (uses fseek)
    - input 2 to be set to 0 for no errors, -1 for a
      memory error, and 1 for an empty file
  - Returns:
    - mst\_runPrim structure with the file index

## build\_mst\_runPrim

The second step is to make the MST tree. You can use the
  `build_mst_runPrim` function to do this.

- Input:
  1. FILE pointer to the file with the features to get the
     distances from
  2. The node to start building the tree with (use 0 if
     unsure)
  3. A mst\_runPrim structure pointer with the index's for
     the feature file (input 1; use
     `indexFeatureFile_runPrim`)
- Output:
  - Modifies:
    - The mst\_runPrim structure (input 3) to have the
      minimum spanning tree
  - Return:
    - 0 for no errors
    - 1 for memory errors
    - (1 + line_on) * -1 for file errors

## mstToNewick\_mst\_runPrim

To print the MST tree as a newick file use the
  `mstToNewick_mst_runPrim` function.

- Input:
  1. mst\_runPrim structure pointer with the tree to print
     out (use `build_mst_runPrim` to set this up)
  2. FILE pointer to the file to save the newick tree to
- Output:
  - Prints: the newick tree to the output file (input 2)

## freeing or blanking

When you are done with building the tree you can free the
  mst\_runPrim structure with `freeHeap_mst_runPrim`. The
  input is the mst\_runPrim tree pointer to free. You must
  set the pointer to null or 0 after calling
  `freeHeap_mst_runPrim(mstSTPtr)`.

If you want to rebuild another tree, say with a different
  starting node. You can blank the minimum spanning tree
  with the `blank_mst_runPrim` function. The blank
  function will still keep the file index's and the name
  arrays. The input is the mst\_runPrim structure pointer
  to free (`blank_mst_runPrim(mstSTPtr)`).

# Non-tested example code

Here is an example of me using runPrim:

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "runPrim.h"

int
main(
){
   signed int retSI = 0;
   signed char errSC = 0;
   signed long errSL = 0;

   struct mst_runPrim *mstHeapST = 0;

   FILE *featureFILE = 0;
   FILE *outFILE = stdout;

   /*___________setup_the_structures____________________*/
   featureFILE = fopen("features.tsv", "r");
   if(! freatureFILE)
      goto err_main;

   indexFeatureFile_runPrim(featureFILE, &errSC);
   if(! errSC)
      ; /*no errors
   else if(errSC)
      goto err_main;
      /*errSC: 1 empty file; -1 memory error*/

   /*___________build_the_mst_tree______________________*/
   errSL = build_mst_runPrim(featureFILE, 0, mstHeapST);
      /*using the 0th (first) node to build the tree*/
   if(! errSL)
      ; /*no errors*/
   else
      goto err_main;
      /*errSL:
      `  1 is a memory error
      `  < 0 is a file error
      `    you can do (errSL + 1) * -1 to get the line the
      `    file error was on
      */

   /*print the tree to a newick file*/
   mstToNewick_mst_runPrim(mstHeapST, outFILE);

   /*_____You_would_not_want_to_do_this_but_to_rebuild____
   `________________the_tree_for_each_node_do_____________
   `  for(
   `     errSL = 1;
   `     errSL < mstHeapST->nodeLenSL - 1;
   `     ++errSL
   `  ){ /*Loop: rebuild the tree for all nodes*/
   `     blank_mst_runPrim(mstHeapST); /*remove old tree*/
   `
   `     build_mst_runPrim(featureFILE, errSL, mstHeapST);
   `        /*at this piont there is no chance of a memory
   `        `  or file error. Everything is already set up
   `        */
   `
   `      fprintf(outFILE, "\n\ntree %li\n", errSL);
   `      mstToNewick_mst_runPrim(mstHeapST, outFILE);
   `  } /*Loop: rebuild the tree for all nodes*/
   */

   /*_____________clean_up______________________________*/
   retSI = 0;
   goto ret_main;

   err_main:;
      retSI = 1;
      goto ret_main;

   ret_main:;
      if(featureFILE)
         fclose(featureFILE);
      fclose(featureFILE);

      if(! outFILE)
         ;
      else if(outFILE != stdout)
         fclose(outFILE);
      outFILE = 0;

      if(mstHeapST)
         freeHeap_mst_runPrim(mstHeapST);
      mstHeapST = 0;

      return retSI;
} /*main*/
```
