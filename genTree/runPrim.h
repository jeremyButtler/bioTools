/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' runPrim.c SOF: Start Of File
'   - holds the functions to run prim.c
'   o header:
'     - guards for people how include .h files in .h files
'   o fun01: blank_mst_runPrim
'     - lazely blanks a mst_runPrim structure
'   o .c fun02: init_mst_runPrim
'     - initialize values in a mst_runPrim struct
'   o fun03: freeStack_mst_runPrim
'     - frees the variables in a mst_runPrim struct
'   o fun04: freeHeap_mst_runPrim
'     - free a mst_runPrim struct that is on the heap
'   o .c fun05: setupMem_mst_runPrim
'     - adds the initial memory to a runPrim structure
'   o .c fun06: mk_mst_runPrim
'     - make a mst_runPrim structure and allocate memory
'   o .c fun07: addName_mst_runPrim
'     - adds name to name array in a mst_runPrim struct
'   o fun08: indexFeatureFile_runPrim
'     - index a feature file for prims to get each nodes
'       location, also get the feature names
'   o fun09: build_mst_runPrim
'     - run prim using a file without keeping a distance
'       matrix (less memory, but one tree build)
'   o fun10: mstToNewick_mst_runPrim
'     - print the tree in a mst_runPrim struct as a newick
'       file
'   o license
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards for people how include .h files in .h files
\-------------------------------------------------------*/

#ifndef RUN_PRIMS_ALGORITHM_FROM_PRIM_C_H
#define RUN_PRIMS_ALGORITHM_FROM_PRIM_C_H

struct heap_prim;

/*-------------------------------------------------------\
| ST01: mst_runPrim
|   - holds the mst tree built by the run functions
\-------------------------------------------------------*/
typedef struct
mst_runPrim
{
   struct heap_prim *mstST;   /*has mst tree*/
   signed char **nameAryStr;  /*node names for newick*/
   singed int *distArySI;  /*distance row or spare array*/
   signed long *indexArySL;   /*index of all lines*/

   singed long nodeLenSL;    /*number nodex in tree*/
   singed long nodeSizeSL;   /*nodes in structure*/
   signed long maxLineLenSL; /*max line length in file*/
}mst_runPrim;

/*-------------------------------------------------------\
| Fun01: blank_mst_runPrim
|   - lazely blanks a mst_runPrim structure
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct pointer to blank
| Output:
|   - Modifies:
|     o mstST in mstSTPtr to be blanked
\-------------------------------------------------------*/
void
blank_mst_runPrim(
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_mst_runPrim
|   - frees the variables in a mst_runPrim struct
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct pionter with variables to free
| Output:
|   - Frees:
|     o all variables in mst_runPrim and sets to 0/null
\-------------------------------------------------------*/
void
freeStack_mst_runPrim(
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_mst_runPrim
|   - free a mst_runPrim struct that is on the heap
| Input:
|   - mstSTPtr:
|     o struct mst_runPrim pointer to free
| Output:
|   - Frees:
|     o frees the mst_runPrim structure, but you must set
|       to null
\-------------------------------------------------------*/
void
freeHeap_mst_runPrim(
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun08: indexFeatureFile_runPrim
|   - index a feature file for prims to get each nodes
|     location, also get the feature names
| Input:
|   - featureFILE:
|     o FILE pointer to feature file to index
|   - errSCPtr:
|     o signed char pointer to get errors
| Output:
|   - Modifies:
|     o featureFILE to move to end and then be set to
|       index 0
|     o errSCPtr to have the number of lines or errors
|       * 0 no errors
|       * -1: memory error
|       * 1: empty file or to few features
|   - Returns:
|     o mst_runPrim struct pointer with file index's and
|       the names for each entry
|       * nameAryStr skips the header
|       * nodeLenSL includes the header
|     o 0 for an error
\-------------------------------------------------------*/
struct mst_runPrim *
indexFeatureFile_runPrim(
   void *featureFILE,
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun09: build_mst_runPrim
|   - run prim using a file without keeping a distance
|     matrix (less memory, but one tree build)
| Input:
|   - featureFILE:
|     o FILE pointer to file with features
|   - firstNodeSI:
|     o node number to start building the tree with
|       * value 0 is the first node in the tree
|       * max value is mstSTPtr->nodeLenSL - 2
|   - mstSTPtr
|     o mst_runPrim struct pointer with the index for
|       the file (returned from indexFeatureFile_runPrim)
| Output:
|   - Modifies;
|     o mstST in mstSTPtr to have minimum spanning tree
|     o distArySI in mstSTPtr to have distances from the
|       last read row
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
|     o (1 + lineSL) * -1 for file errors
\-------------------------------------------------------*/
signed long
build_mst_runPrim(
   void *featureFILE,
   signed int firstNodeSI,      /*entry to build tree*/
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun10: mstToNewick_mst_runPrim
|   - print the tree in a mst_runPrim struct as a newick
|     file
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct with the tree to print
|   - outFILE:
|     o FILE pointer to file to print to
| Output:
|   - Prints:
|     o minimum spanning tree in mstSTPtr to outFILE
|   - Modifies:
|     o distArySI in mstSTPtr to have values 0 to number
|       of nodes (this array is a temporary array, so not
|       a big change)
\-------------------------------------------------------*/
void
mstToNewick_mst_runPrim(
   struct mst_runPrim *mstSTPtr,
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
