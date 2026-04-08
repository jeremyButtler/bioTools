/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' runPrim.c SOF: Start Of File
'   - holds the functions to run prim.c
'   o header:
'     - included libraries
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
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "runPrim.h"

#include <stdio.h>
#include "../genLib/ulCp.h"
#include "../genLib/fileFun.h"
#include "prim.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries
!   - .c  #include "../genSort/siBinSearch.h"
!   - .c  #include "../genLib/genMath.h"
!   - .h  #include "../genLib/endLine.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
){
   if(! mstSTPtr)
      return;
   if(mstSTPtr->mstST)
      blank_heap_prim(mstSTPtr->mstST);
} /*blank_mst_runPrim*/

/*-------------------------------------------------------\
| Fun02: init_mst_runPrim
|   - initialize values in a mst_runPrim struct
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct pionter with variables to free
| Output:
|   - Modifies:
|     o all variables in mst_runPrim to be 0/null
\-------------------------------------------------------*/
void
init_mst_runPrim(
   struct mst_runPrim *mstSTPtr
){
   if(! mstSTPtr)
      return;

   mstSTPtr->mstST = 0;
   mstSTPtr->nameAryStr = 0;
   mstSTPtr->distArySI = 0;
   mstSTPtr->nodeSizeSL = 0;
   mstSTPtr->nodeLenSL = 0;

   mstSTPtr->indexArySL = 0;
   mstSTPtr->maxLineLenSL = 0;

   blank_mst_runPrim(mstSTPtr);
} /*init_mst_runPrim*/

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
){
  signed long slPos = 0;

  if(! mstSTPtr)
     return;

   if(mstSTPtr->mstST)
      freeHeap_heap_prim(mstSTPtr->mstST);
   if(mstSTPtr->distArySI)
      free(mstSTPtr->distArySI);
   if(mstSTPtr->indexArySL)
      free(mstSTPtr->indexArySL);

   if(mstSTPtr->nameAryStr)
   { /*If: need to free the stored names*/
      for(slPos=0; slPos < mstSTPtr->nodeSizeSL; ++slPos)
      { /*Loop: free all names*/
         if(mstSTPtr->nameAryStr[slPos])
            free(mstSTPtr->nameAryStr[slPos]);
         mstSTPtr->nameAryStr[slPos] = 0;
      } /*Loop: free all names*/

      free(mstSTPtr->nameAryStr);
   } /*If: need to free the stored names*/

   init_mst_runPrim(mstSTPtr);
} /*freeStack_mst_runPrim*/

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
){
   if(! mstSTPtr)
      return;
   freeStack_mst_runPrim(mstSTPtr);
   free(mstSTPtr);
} /*freeHeap_mst_runPrim*/

/*-------------------------------------------------------\
| Fun05: setupMem_mst_runPrim
|   - adds the initial memory to a runPrim structure
| Input:
|   - mstSTPtr:
|     o pointer to a mst_runPrim struct to add memory to
|   - numNodesSL:
|     o maximum nodes in the tree
| Output:
|   - Modifies:
|     o nameAryStr in mstSTPtr to have numNodesSL pointers
|     o distArySI in mstSTPtr to have numNodesSL ints
|     o all variables in mstST in mstSTPtr to be length
|       of numNodesSL
|     o nodeSizeSL in mstSTPtr to be set to numNodesSL
|   - Return:
|     o 0 for no errors
|     o 1 for memory errors
| Note:
|   - this does not setup indexArySL because indexArySL
|     is the file index and needs a file to set
\-------------------------------------------------------*/
signed char
setupMem_mst_runPrim(
   struct mst_runPrim *mstSTPtr,
   signed long numNodesSL
){
   signed long slNode = 0;

   if(! mstSTPtr)
      return 0; /*no structure input*/

   /*_____________add_memory_for_the_heap_______________*/
   if(! mstSTPtr->mstST)
   { /*If: need a heap_prim structure for the tree*/
      mstSTPtr->mstST = malloc(sizeof(struct heap_prim));
      if(! mstSTPtr->mstST)
         goto memErr_fun05;
      init_heap_prim(mstSTPtr->mstST);
   } /*If: need a heap_prim structure for the tree*/

   if( setupMem_heap_prim(mstSTPtr->mstST, numNodesSL) )
      goto memErr_fun05;

   /*_____________add_memory_for_the_arrays_____________*/
   if(numNodesSL > mstSTPtr->nodeSizeSL)
   { /*If: need more memory*/
      /*_______________setup_the_distance_row___________*/
      if(mstSTPtr->distArySI)
         free(mstSTPtr->distArySI);
      mstSTPtr->distArySI = 0;

      mstSTPtr->distArySI =
         malloc(numNodesSL * sizeof(signed int));
      if(! mstSTPtr->distArySI)
         goto memErr_fun05;


      /*_______________setup_the_name_array_____________*/
      if(mstSTPtr->nameAryStr)
      { /*If: have a name array I need to free*/
         for(
            slNode = 0;
            slNode < mstSTPtr->nodeLenSL;
            ++slNode
         ){ /*Loop: free all name pointers*/
            if(mstSTPtr->nameAryStr[slNode])
               free(mstSTPtr->nameAryStr[slNode]);
            mstSTPtr->nameAryStr[slNode] = 0;
         }  /*Loop: free all name pointers*/

         mstSTPtr->nameAryStr = 0;
      } /*If: have a name array I need to free*/

      mstSTPtr->nameAryStr =
         malloc(numNodesSL * sizeof(signed char *));
      if(! mstSTPtr->nameAryStr)
         goto memErr_fun05;

      for(slNode = 0; slNode < numNodesSL; ++slNode)
      { /*Loop: initialize all values to 0*/
         mstSTPtr->nameAryStr[slNode] = 0;
         mstSTPtr->distArySI[slNode] = 0;
      } /*Loop: initialize all values to 0*/


      mstSTPtr->nodeSizeSL = numNodesSL;
   } /*If: need more memory*/

   /*________________retur_the_results__________________*/
   mstSTPtr->nodeLenSL = numNodesSL;
   return 0;

   memErr_fun05:;
      return 1;
} /*setupMem_mst_runPrim*/

/*-------------------------------------------------------\
| Fun06: mk_mst_runPrim
|   - make a mst_runPrim structure and allocate memory
| Input:
|   - numNodesSL:
|     o number nodes in the tree
| Output:
|   - Returns:
|     o pionter to a mst_runPrim structure that is
|       initialize and has memory
|     o 0 for memory errors
\-------------------------------------------------------*/
struct mst_runPrim *
mk_mst_runPrim(
   signed long numNodesSL
){
   struct mst_runPrim *mstHeapSTPtr = 0;

   mstHeapSTPtr = malloc(sizeof(struct mst_runPrim));
   if(! mstHeapSTPtr)
      goto memErr_fun06;
   init_mst_runPrim(mstHeapSTPtr);

   if( setupMem_mst_runPrim(mstHeapSTPtr, numNodesSL) )
      goto memErr_fun06;

   return mstHeapSTPtr;

   memErr_fun06:;
      if(mstHeapSTPtr)
         freeHeap_mst_runPrim(mstHeapSTPtr);
      mstHeapSTPtr = 0;
      return 0;
} /*mk_mst_runPrim*/

/*-------------------------------------------------------\
| Fun07: addName_mst_runPrim
|   - adds name to the name array in a mst_runPrim struct
| Input:
|   - nameStr:
|     o cstring with the name to add
|   - indexSI:
|     o index to add nameStr to
|   - mstSTPtr:
|     o mst_runPrim struct pointer to add names to
| Output:
|   - Modifies:
|     o nameAryStr in mstSTPtr to have nameStr at indexSI
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
|     o 2 for out of bounds error
\-------------------------------------------------------*/
signed char
addName_mst_runPrim(
   signed char *nameStr,
   signed int indexSI,
   struct mst_runPrim *mstSTPtr
){
   signed long lenSL = endWhite_ulCp(nameStr);

   if(indexSI >= mstSTPtr->nodeLenSL)
      return 2;

   if(mstSTPtr->nameAryStr[indexSI])
      free(mstSTPtr->nameAryStr[indexSI]);
   mstSTPtr->nameAryStr[indexSI] =
      malloc((lenSL + 8) * sizeof(signed char));
   if( ! mstSTPtr->nameAryStr[indexSI] )
      return 1;

   cpLen_ulCp(
      mstSTPtr->nameAryStr[indexSI],
      nameStr,
      lenSL
   );

   return 0;
} /*addName_mst_runPrim*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - index a feature file for prims to get each nodes
   '     location
   '   o fun08 sec01:
   '     - variable declarations
   '   o fun08 sec02:
   '     - index the file and allocate memory
   '   o fun08 sec03:
   '     - add the entry names to the mst
   '   o fun08 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *buffHeapStr = 0;
   signed long lenSL = 0;          /*length of cstring*/
   struct mst_runPrim *mstHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - index the file and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*this method is inefficent since I have to read the
   `  file three times. However, it is simpler
   `   1. get number lines
   `   2. index each line
   `   3. add name for each entry
   */
   mstHeapST = malloc(sizeof(struct mst_runPrim));
   if(! mstHeapST)
      goto memErr_fun08_sec04;
   init_mst_runPrim(mstHeapST);

   mstHeapST->indexArySL =
      lineIndex_fileFun(
         featureFILE,
         &mstHeapST->nodeLenSL,
         &mstHeapST->maxLineLenSL
      ); /*reads file twice; get # lines & index lines*/

   if(mstHeapST->nodeLenSL <2 && mstHeapST->nodeLenSL >=0)
      goto emptyFile_fun08_sec04;
   else if(mstHeapST->indexArySL)
      ; /*no errors*/
   else if(mstHeapST->nodeLenSL < 0)
      goto memErr_fun08_sec04; /*memory error*/

   if(
      setupMem_mst_runPrim(mstHeapST,mstHeapST->nodeLenSL)
   ) goto memErr_fun08_sec04;

   buffHeapStr =
      malloc(
          (mstHeapST->maxLineLenSL + 8)
        * sizeof(signed char)
      );
   if(! buffHeapStr)
      goto memErr_fun08_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - add the entry names to the mst
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(lenSL = 1; lenSL < mstHeapST->nodeLenSL; ++lenSL)
   { /*Loop: get the file names*/
      getLineByIndex_fileFun(
         buffHeapStr,
         lenSL,
         mstHeapST->indexArySL,
         mstHeapST->nodeLenSL,
         featureFILE
      );

      /*3. add name (third file read through*/
      if(
         addName_mst_runPrim(
            buffHeapStr,
            lenSL - 1, /*-1 to skip the header*/
            mstHeapST
         )
      ) goto memErr_fun08_sec04;
   } /*Loop: get the file names*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun08_sec04;

   memErr_fun08_sec04:;
      *errSCPtr = -1;
       goto errClean_fun08_sec04;

   emptyFile_fun08_sec04:;
      *errSCPtr = 1;
       goto errClean_fun08_sec04;

   errClean_fun08_sec04:;
      if(mstHeapST)
         freeHeap_mst_runPrim(mstHeapST);
      mstHeapST = 0;
      goto ret_fun08_sec04;

   ret_fun08_sec04:;
      if(featureFILE)
         fseek((FILE *) featureFILE, 0, SEEK_SET);

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      return mstHeapST;
} /*indexFeatureFile_runPrim*/

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
|     o 1 if firstNodeSI is not in the tree
|     o 2 for memory errors
|     o (1 + lineSL) * -1 for file errors
\-------------------------------------------------------*/
signed long
build_mst_runPrim(
   void *featureFILE,
   signed int firstNodeSI,      /*entry to build tree*/
   struct mst_runPrim *mstSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - run prim using a file without keeping a distance
   '     matrix (less memory, but one tree build)
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - allocate memory for buffers
   '   o fun09 sec03:
   '     - build the mst tree
   '   o fun09 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *qryRowHeapStr = 0; /*has query features*/
   signed char *qryTmpStr = 0;     /*used in comparing*/

   signed char *refRowHeapStr = 0; /*has ref features*/
   signed char *refTmpStr = 0;     /*used in comparing*/

   signed long slEntry = 0;     /*entry on for query*/
   signed long linesReadSL = 0; /*number lines processed*/
   signed long slPos = 0;       /*entry on in the file*/

   signed int *childHeapArySI = 0; /*gets children nodes*/
   signed int distLenSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - allocate memory for buffers
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(firstNodeSI > mstSTPtr->nodeLenSL - 2)
      goto invalidNode_fun09_sec05;
   else if(firstNodeSI < 0)
      goto invalidNode_fun09_sec05;

   childHeapArySI =
      malloc(mstSTPtr->nodeLenSL * sizeof(signed int));
   if(! childHeapArySI)
      goto memErr_fun09_sec05;

   qryRowHeapStr =
      malloc(
           (mstSTPtr->maxLineLenSL + 8)
         * sizeof(signed char)
      );
   if(! qryRowHeapStr)
      goto memErr_fun09_sec05;

   refRowHeapStr =
      malloc(
           (mstSTPtr->maxLineLenSL + 8)
         * sizeof(signed char)
      );
   if(! refRowHeapStr)
      goto memErr_fun09_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - build the mst tree
   ^   o fun09 sec03 sub01:
   ^     - start loop and get edges for the new parent
   ^   o fun09 sec03 sub02:
   ^     - get the row for each edge from the file
   ^   o fun09 sec03 sub03:
   ^     - get the distance for an edge
   ^   o fun09 sec03 sub04:
   ^     - add new edges to the tree and get next child
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - start loop and get edges for the new parent
   \*****************************************************/

   linesReadSL = 0;
   slEntry = firstNodeSI;

   for(
      linesReadSL = 0;
      linesReadSL < mstSTPtr->nodeLenSL - 1;
              /*-1 because nodeLenSL includes the header*/
      ++linesReadSL
   ){ /*Loop: get distance for a single row*/
      /*mark new parenet as added*/
      mstSTPtr->distArySI[slEntry] = -1;

      getLineByIndex_fileFun(
         qryRowHeapStr,
         slEntry + 1,   /*+1 to skip the header*/
         mstSTPtr->indexArySL,
         mstSTPtr->nodeLenSL,
         featureFILE
      );

      /**************************************************\
      * Fun09 Sec03 Sub02:
      *   - get the row for each edge from the file
      \**************************************************/

      distLenSI = 0;
      for(
         slPos = 0;        /*0th node is always a parent*/
         slPos < mstSTPtr->nodeLenSL - 1;
              /*-1 because nodeLenSL includes the header*/
         ++slPos
      ){ /*Loop: get the distances for one row*/
         if(mstSTPtr->mstST->indexArySI[slPos] < 0)
             continue; /*this node is a parent*/
         else if(slPos == firstNodeSI)
             continue; /*original node*/
         else
            mstSTPtr->distArySI[distLenSI] = 0;

         childHeapArySI[distLenSI] = slPos;

         getLineByIndex_fileFun(
            refRowHeapStr,
            slPos + 1,   /*+1 to skip the header*/
            mstSTPtr->indexArySL,
            mstSTPtr->nodeLenSL,
            featureFILE
         );

         /***********************************************\
         * Fun09 Sec03 Sub03:
         *   - get the distance for an edge
         \***********************************************/

         /*________set_points_and_get_past_the_id_______*/
         qryTmpStr = qryRowHeapStr;
         refTmpStr = refRowHeapStr;

         qryTmpStr += endWhite_ulCp(qryTmpStr);
         while(*qryTmpStr && *qryTmpStr < 33)
            ++qryTmpStr;

         refTmpStr += endWhite_ulCp(refTmpStr);
         while(*refTmpStr && *refTmpStr < 33)
            ++refTmpStr;


         while(*qryTmpStr && *refTmpStr)
         { /*Loop: find the distance*/
            /*____check_if_have_known_value_for_loci____*/
            if(qryTmpStr[0] == '*')
               goto missingEntry_fun08_sec03_sub04;

            else if(refTmpStr[0] == '*')
               goto missingEntry_fun08_sec03_sub04;

            else if(
                  (qryTmpStr[0] | 32) == 'n'
               && (qryTmpStr[1] | 32) == 'a'
               && qryTmpStr[2] < 33
            ) goto missingEntry_fun08_sec03_sub04;

            else if(
                  (refTmpStr[0] | 32) == 'n'
               && (refTmpStr[1] | 32) == 'a'
               && refTmpStr[2] < 33
            ){ /*Else If: unkown reference lineage*/
               missingEntry_fun08_sec03_sub04:;
                  while(*qryTmpStr > 32)
                     ++qryTmpStr;
                  while(*refTmpStr > 32)
                     ++refTmpStr;
            }  /*Else If: unkown reference lineage*/

            else if( eqlWhite_ulCp(qryTmpStr, refTmpStr) )
               ++mstSTPtr->distArySI[distLenSI];
               /*different*/

            /*___________find_the_next_loci_____________*/
            qryTmpStr += endWhite_ulCp(qryTmpStr);
            while(*qryTmpStr && *qryTmpStr < 33)
               ++qryTmpStr;

            refTmpStr += endWhite_ulCp(refTmpStr);
            while(*refTmpStr && *refTmpStr < 33)
               ++refTmpStr;
         } /*Loop: find the distance*/


         ++distLenSI;
         if(*qryTmpStr || *refTmpStr)
            goto fileErr_fun09_sec05;
            /*still have features that were not compared*/
      } /*Loop: get the distances for one row*/

      /**************************************************\
      * Fun09 Sec03 Sub04:
      *   - add new edges to the tree and get next child
      \**************************************************/

      if(distLenSI)
      { /*If: have edges to add*/
         addEdges_heap_prim(
            mstSTPtr->distArySI,
            childHeapArySI,
            distLenSI,
            slEntry,           /*parent node adding in*/
            mstSTPtr->mstST
         ); /*add the new edges in*/

         slEntry = extractEdge_heap_prim(mstSTPtr->mstST);
      } /*If: have edges to add*/
   }  /*Loop: get distance for a single row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   slPos = 0;
   goto ret_fun09_sec05;

   invalidNode_fun09_sec05:;
      slPos = 1;
      goto errClean_fun09_sec05;

   memErr_fun09_sec05:;
      slPos = 2;
      goto errClean_fun09_sec05;

   fileErr_fun09_sec05:;
      slPos = (slPos + 1) * -1;
      goto errClean_fun09_sec05;

   errClean_fun09_sec05:;
      goto ret_fun09_sec05;

   ret_fun09_sec05:;
      if(childHeapArySI)
         free(childHeapArySI);
      childHeapArySI = 0;

      if(qryRowHeapStr)
         free(qryRowHeapStr);
      qryRowHeapStr = 0;

      if(refRowHeapStr)
         free(refRowHeapStr);
      refRowHeapStr = 0;

      return slPos;
} /*build_mst_runPrim*/

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
){
   /*this is a wrapper function*/
   root0MstToNewick_heap_prim(
      mstSTPtr->mstST,
      mstSTPtr->nameAryStr,
      mstSTPtr->distArySI,
      outFILE
   );
} /*mstToNewick_mst_runPrim*/

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
