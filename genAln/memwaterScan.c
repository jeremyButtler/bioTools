/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' memwaterScan SOF: Start Of Functions
'   - has memory efficent Waterman Smith alignment scan
'     (best alignment for each reference/query base)
'   o header:
'     - included libraries
'   o .h st01: aln_memwaterScan
'     - stores the alignment results from a waterman scan
'   o fun01: blank_aln_memwaterScan
'     - blanks (sets non-array values to 0) an
'   o fun02: init_aln_memwaterScan
'     - initializes (sets arrays to 0 + blanks) an
'       aln_memwaterScan structure
'   o fun03: freeStack_aln_memwaterScan
'     - frees the variables in an aln_memwaterScan struct
'   o fun04: freeHeap_aln_memwaterScan
'     - frees an aln_memwaterScan struct
'   o fun05: getCoord_aln_memwaterScan
'     - gets the coordinates for input base index for an
'       aln_memwaterScan structure
'   o fun06: refCoordSort_aln_memwaterScan
'     - sorts best bases alignments by reference start,
'       query start, reference end (closest first), then
'       query end (closest first), and finally score.
'       all negatives (non-alignment found) are pushed to
'       the end
'   o fun07 memwaterScan:
'     - performs a memory efficent Smith Waterman scan
'       (keep best alignment for each query/reference base)
'       alignment on a pair of sequences
'   o license:
'     - licensing for this code (public domain / mit)
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

#include "memwaterScan.h"

#include "../genBio/seqST.h"

#include "alnSet.h"
#include "indexToCoord.h"

/*.h files only*/
#include "../genLib/genMath.h" /*only using .h portion*/
#include "alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o std #include <stdio.h>
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .h  #include "../genLib/endLine.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_aln_memwaterScan
|   - blanks (sets non-array values to 0) an
|     aln_memwaterScan struct
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to blank
| Output:
|   - Modifies:
|     o sets refLenSI, qryLenSI, refOffsetSI, qryOffsetSI
|       in alnsTPtr to 0
\-------------------------------------------------------*/
void
blank_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
){
   if(! alnSTPtr)
      return;

   alnSTPtr->refLenSI = 0;
   alnSTPtr->qryLenSI = 0;
   alnSTPtr->refOffsetSI = 0;
   alnSTPtr->qryOffsetSI = 0;

   alnSTPtr->outLenSL = 0;
} /*blank_aln_memwaterScan*/

/*-------------------------------------------------------\
| Fun02: init_aln_memwaterScan
|   - initializes (sets arrays to 0 + blanks) an
|     aln_memwaterScan structure
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to initialize
| Output:
|   - Modifies:
|     o sets all values to 0
\-------------------------------------------------------*/
void
init_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
){
   if(! alnSTPtr)
      return;

   alnSTPtr->startArySL = 0;
   alnSTPtr->endArySL = 0;
   alnSTPtr->scoreArySL = 0;
   alnSTPtr->outSizeSL = 0;

   alnSTPtr->scoreRowSL = 0;
   alnSTPtr->indexRowSL = 0;
   alnSTPtr->dirRowSC = 0;
   alnSTPtr->rowSizeSI = 0;

   blank_aln_memwaterScan(alnSTPtr);
} /*init_aln_memwaterScan*/

/*-------------------------------------------------------\
| Fun03: freeStack_aln_memwaterScan
|   - frees the variables in an aln_memwaterScan struct
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer with variable to
|       free
| Output:
|   - Modifies:
|     o frees all arrays and initializes
\-------------------------------------------------------*/
void
freeStack_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
){
   if(! alnSTPtr)
      return;

   if(alnSTPtr->startArySL)
      free(alnSTPtr->startArySL);
   if(alnSTPtr->endArySL)
      free(alnSTPtr->endArySL);
   if(alnSTPtr->scoreArySL)
      free(alnSTPtr->scoreArySL);

   if(alnSTPtr->scoreRowSL)
      free(alnSTPtr->scoreRowSL);
   if(alnSTPtr->indexRowSL)
      free(alnSTPtr->indexRowSL);
   if(alnSTPtr->dirRowSC)
      free(alnSTPtr->dirRowSC);

   init_aln_memwaterScan(alnSTPtr);
} /*freeStack_aln_memwaterScan*/

/*-------------------------------------------------------\
| Fun04: freeHeap_aln_memwaterScan
|   - frees an aln_memwaterScan struct
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer to free
| Output:
|   - Modifies:
|     o frees alnSTPtr; you must set the pointer to 0/null
\-------------------------------------------------------*/
void
freeHeap_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
){
   if(! alnSTPtr)
      return;
   freeStack_aln_memwaterScan(alnSTPtr);
   free(alnSTPtr);
} /*freeHeap_aln_memwaterScan*/

/*-------------------------------------------------------\
| Fun05: getCoord_aln_memwaterScan
|   - gets the coordinates for input base index for an
|     aln_memwaterScan structure
| Input:
|   - baseSI:
|     o base to get coordinates for
|   - refStartSIPtr:
|     o signed int pointer that gets the reference start
|   - refEndSIPtr:
|     o signed int pointer that gets the reference end
|   - qryStartSIPtr:
|     o signed int pointer that gets the query start
|   - qryEndSIPtr:
|     o signed int pointer that gets the query end
|   - alnSTPtr:
|     o aln_memwaterScan struct pointer with coordinates
| Output:
|   - Modifies:
|     o refStartSIPtr to have reference starting position
|     o refEndSIPtr to have reference ending position
|     o qryStartSIPtr to have query starting position
|     o qryEndSIPtr to have query ending position
|   - Returns:
|     o score for success
|     o 0 if no position at index
|     o -1 for out of bounds error
\-------------------------------------------------------*/
signed char
getCoord_aln_memwaterScan(
   signed long baseSL,        /*index to grab*/
   signed int *refStartSIPtr, /*gets reference start*/
   signed int *refEndSIPtr,   /*gets reference end*/
   signed int *qryStartSIPtr, /*gets query start*/
   signed int *qryEndSIPtr,   /*gets query end*/
   struct aln_memwaterScan *alnSTPtr
){
   unsigned long tmpRefUL = 0;
   unsigned long tmpQryUL = 0;

   if(baseSL > alnSTPtr->outLenSL)
      goto boundsErr_fun05_sec04;
   else if(alnSTPtr->scoreArySL[baseSL] < 0)
      goto noCoord_fun05_sec04;

   indexToCoord(
      alnSTPtr->refLenSI,
      alnSTPtr->startArySL[baseSL],
      &tmpRefUL,
      &tmpQryUL
   );

   *refStartSIPtr = tmpRefUL + alnSTPtr->refOffsetSI;
   *qryStartSIPtr = tmpQryUL + alnSTPtr->qryOffsetSI;

   indexToCoord(
      alnSTPtr->refLenSI,
      alnSTPtr->endArySL[baseSL],
      &tmpRefUL,
      &tmpQryUL
   );
   *refEndSIPtr = tmpRefUL + alnSTPtr->refOffsetSI;
   *qryEndSIPtr = tmpQryUL + alnSTPtr->qryOffsetSI;

   return alnSTPtr->scoreArySL[baseSL];

   noCoord_fun05_sec04:;
      return 0;
   boundsErr_fun05_sec04:;
      return -1;
} /*getCoord_aln_memwaterScan*/

/*-------------------------------------------------------\
| Fun06: refCoordSort_aln_memwaterScan
|   - sorts best bases alignments by reference start,
|     query start, reference end (closest first), then
|     query end (closest first), and finally score.
|     all negatives (non-alignment found) are pushed to
|     the end
| Input:
|   - alnSTPtr:
|     o aln_memwaterScan struct pionter to sort
| Output:
|   - Modifies:
|     o qryStartArySL in alnSTPtr to be sorted by
|       reference coodrinate
|     o qryEndArySL and qryScoreArySL in alnSTPtr to be
|       kept in sync with qryStartArySL
\-------------------------------------------------------*/
void
refCoordSort_aln_memwaterScan(
   struct aln_memwaterScan *alnSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - sorts best bases alignments by reference start,
   '     query start, reference end (closest first), then
   '     query end (closest first), and finally score.
   '     all negatives (non-alignment found) are pushed to
   '     the end
   '   o fun06 sec01:
   '     - variable declerations
   '   o fun06 sec02:
   '     - find the number of rounds to sort for
   '   o fun06 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   signed long numElmSL = alnSTPtr->outLenSL - 1;

   /*Number of sorting rounds*/
   signed int subSI = 0;
   signed int nextSI = 0;
   signed int lastSI = 0;
   signed int onSI = 0;

   unsigned long onRefStartUL;
   unsigned long onRefEndUL;
   unsigned long cmpRefStartUL;
   unsigned long cmpRefEndUL;

   unsigned long onQryStartUL;
   unsigned long onQryEndUL;
   unsigned long cmpQryStartUL;
   unsigned long cmpQryEndUL;

   /*Variables to incurment loops*/
   signed int siIndex = 0;
   signed int siElm = 0;

   /*sanity variables; using unsigned longs for start and
   `  end to push negatives (no alignment assigned) to the
   `  ends
   */
   unsigned long *startAryUL =
      (unsigned long *) alnSTPtr->startArySL;
   unsigned long *endAryUL =
      (unsigned long *) alnSTPtr->endArySL;
   signed long *scoreArySL = alnSTPtr->scoreArySL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numElmSL <= 0)
      return;

   /*Recursion formsla: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSI = 1; /*Initialzie first array*/

   while(subSI < numElmSL - 1)
      subSI = (3 * subSI) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - sort arrays
   ^   o fun06 sec03 sub01:
   ^     - outer loops for shell short
   ^   o fun06 sec03 sub02:
   ^     - compare and check if need to do first swap
   ^   o fun06 sec03 sub03:
   ^     - do inner (insertion) swaps
   ^   o fun06 sec03 sub04:
   ^     - move to the next element in the group
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec03 Sub01:
   *   - outer loops for shell short
   \*****************************************************/

   while(subSI > 0)
   { /*Loop: all rounds*/
      for(siIndex = 0; siIndex <= subSI; ++siIndex)
      { /*Loop: though sub array*/
         siElm = siIndex;

         for(
            siElm = siIndex;
            siElm + subSI <= numElmSL;
            siElm += subSI
         ){ /*Loop: swap elements in subarray*/
            nextSI = siElm + subSI;

            /********************************************\
            * Fun06 Sec03 Sub02:
            *   - compare & check if need to do first swap
            \********************************************/

            indexToCoord(
               alnSTPtr->refLenSI,
               startAryUL[siElm],
               &onRefStartUL,  
               &onQryStartUL
            );

            indexToCoord(
               alnSTPtr->refLenSI,
               startAryUL[nextSI],
               &cmpRefStartUL,  
               &cmpQryStartUL
            );

            indexToCoord(
               alnSTPtr->refLenSI,
               endAryUL[siElm],
               &onRefEndUL,  
               &onQryEndUL
            );

            indexToCoord(
               alnSTPtr->refLenSI,
               endAryUL[nextSI],
               &cmpRefEndUL,  
               &cmpQryEndUL
            );

            if(onRefStartUL > cmpRefStartUL)
               goto swapOuter_fun06_sec03_sub02;
            else if(onRefStartUL < cmpRefStartUL)
               goto nextElm_fun06_sec03_sub04;

            else if(onQryStartUL > cmpQryStartUL)
               goto swapOuter_fun06_sec03_sub02;
            else if(onQryStartUL < cmpQryStartUL)
               goto nextElm_fun06_sec03_sub04;

            else if(onRefEndUL > cmpRefEndUL)
               goto swapOuter_fun06_sec03_sub02;
            else if(onRefEndUL < cmpRefEndUL)
               goto nextElm_fun06_sec03_sub04;

            else if(onQryEndUL > cmpQryEndUL)
               goto swapOuter_fun06_sec03_sub02;
            else if(onQryEndUL < cmpQryEndUL)
               goto nextElm_fun06_sec03_sub04;

            else if(
                 scoreArySL[siElm] < scoreArySL[nextSI]
            ){ /*Else: need to swap based on scores*/
               swapOuter_fun06_sec03_sub02:;

               startAryUL[siElm] ^= startAryUL[nextSI];
               startAryUL[nextSI] ^= startAryUL[siElm];
               startAryUL[siElm] ^= startAryUL[nextSI];

               endAryUL[siElm] ^= endAryUL[nextSI];
               endAryUL[nextSI] ^= endAryUL[siElm];
               endAryUL[siElm] ^= endAryUL[nextSI];

               scoreArySL[siElm] ^= scoreArySL[nextSI];
               scoreArySL[nextSI] ^= scoreArySL[siElm];
               scoreArySL[siElm] ^= scoreArySL[nextSI];

               /*****************************************\
               * Fun06 Sec03 Sub03:
               *   - do inner (insertion) swaps
               \*****************************************/

               lastSI = siElm;
               onSI = siElm;

               while(lastSI >= subSI)
               { /*loop: move swapped element back*/
                  lastSI -= subSI;

                  indexToCoord(
                     alnSTPtr->refLenSI,
                     startAryUL[onSI],
                     &onRefStartUL,  
                     &onQryStartUL
                  );

                  indexToCoord(
                     alnSTPtr->refLenSI,
                     startAryUL[lastSI],
                     &cmpRefStartUL,  
                     &cmpQryStartUL
                  );

                  indexToCoord(
                     alnSTPtr->refLenSI,
                     endAryUL[onSI],
                     &onRefEndUL,  
                     &onQryEndUL
                  );

                  indexToCoord(
                     alnSTPtr->refLenSI,
                     endAryUL[lastSI],
                     &cmpRefEndUL,  
                     &cmpQryEndUL
                  );

                  if(onRefStartUL > cmpRefStartUL)
                     break;
                  else if(onRefStartUL < cmpRefStartUL)
                     ;

                  else if(onQryStartUL > cmpQryStartUL)
                     break;
                  else if(onQryStartUL < cmpQryStartUL)
                     ;

                  else if(onRefEndUL > cmpRefEndUL)
                     break;
                  else if(onRefEndUL < cmpRefEndUL)
                     ;

                  else if(onQryEndUL > cmpQryEndUL)
                     break;
                  else if(onQryEndUL < cmpQryEndUL)
                     ;

                  else if(
                     scoreArySL[onSI] < scoreArySL[lastSI]
                  ) break;

                  startAryUL[onSI] ^= startAryUL[lastSI];
                  startAryUL[lastSI] ^= startAryUL[onSI];
                  startAryUL[onSI] ^= startAryUL[lastSI];

                  endAryUL[onSI] ^= endAryUL[lastSI];
                  endAryUL[lastSI] ^= endAryUL[onSI];
                  endAryUL[onSI] ^= endAryUL[lastSI];

                  scoreArySL[onSI] ^= scoreArySL[lastSI];
                  scoreArySL[lastSI] ^= scoreArySL[onSI];
                  scoreArySL[onSI] ^= scoreArySL[lastSI];

                  onSI = lastSI;
               } /*Loop: move swapped element back*/
            }  /*Else: need to swap based on scores*/

            /********************************************\
            * Fun06 Sec03 Sub04:
            *   - move to the next element in the group
            \********************************************/

            nextElm_fun06_sec03_sub04:;
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSI = (subSI - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*refCoordSort_aln_memwaterScan*/

/*-------------------------------------------------------\
| Fun07: memwaterScan
|   - performs a memory efficent Smith Waterman scan
|     (keep best alignment for each query/reference base)
|     alignment on a pair of sequences
| Input;
|   - qrySeqSTPtr:
|     o pointer to an seqST with the query sequence and
|       index 0 coordinates to start (offsetSL)/end
|       (endAlnSL) the alignment.
|   - refSeqSTPtr:
|     o pointer to an seqST with the reference sequence
|       and index 0 coordinates to start (offsetSL)/end
|       (endAlnSL) the alignment.
|   - alnSTPtr:
|     o pointer to aln_memwaterScan structure to hold the
|       results of the alignment
|   - settings:
|     o pointer to an alnSet structure with the gap open,
|       gap extend, and scoring matrix for the alingment
| Output:
|  - Modifies:
|    o variables in alnSTPtr to have the new alignment
|  - Returns:
|    o score for aligment
|    o negative number for memory errors
\-------------------------------------------------------*/
signed long
memwaterScan(
   struct seqST *qrySTPtr, /*query sequence and data*/
   struct seqST *refSTPtr, /*ref sequence and data*/
   struct aln_memwaterScan *alnSTPtr,/*gets alignment*/
   struct alnSet *settings
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC: memwaterAln
   '  - Run a memory efficent Waterman Smith alignment on
   '    input sequences
   '  o fun07 sec01:
   '    - Variable declerations
   '  o fun07 sec02:
   '    - Allocate memory for alignment
   '  o fun07 sec03:
   '    - Fill in initial negatives for ref
   '  o fun0 sec04:
   '    - Fill the matrix with scores
   '  o fun07 sec05:
   '    - Set up for returing matrix (clean up/wrap up)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01: Variable declerations
   ^  o fun07 sec01 sub01:
   ^    - Variables dealing with the query and reference
   ^      starting positions
   ^  o fun07 sec01 sub02:
   ^    - Variables holding the scores (only two rows)
   ^  o fun07 sec01 sub03:
   ^    - Directinol matrix variables
   ^  o fun07 sec01 sub04:
   ^    - Variables for building returend alignment array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec01 Sub01:
   *  - Variables dealing with the query and reference
   *    starting positions
   \*****************************************************/

   signed long scoreSL = 0;     /*score to return*/

   /*Get start & end of query and reference sequences*/
   signed char *refSeqStr = 0;
   signed char *qrySeqStr = 0;

   signed long refLenSL =
      refSTPtr->endAlnSL - refSTPtr->offsetSL + 1;

   signed long qryLenSL =
      qrySTPtr->endAlnSL - qrySTPtr->offsetSL + 1;

   /*Iterators for loops*/
   signed long slRef = 0;
   signed long slQry = 0;

   /*****************************************************\
   * Fun07 Sec01 Sub02:
   *  - Variables holding the scores (only two rows)
   \*****************************************************/

   signed long snpScoreSL = 0;
   signed long insScoreSL = 0;
   signed long delScoreSL = 0;   /*score for deletion*/
   signed long nextSnpScoreSL = 0;/*next match/snp score*/

   /*****************************************************\
   * Fun07 Sec01 Sub03:
   *  - Directional matrix variables
   \*****************************************************/

   /*Keeping track of alignment starting positions*/
   signed long indexSL = 0;    /*index at in matrix*/
   signed long snpIndexSL = 0; /*last snp index*/
   signed long tmpIndexSL = 0; /*for getting snp index*/

   /*first query base in scoring array*/
   signed long startSL = refLenSL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^  - Allocate memory for alignment
   ^  o fun07 sec02 sub01:
   ^    - get lengths and offsets
   ^  o fun07 sec02 sub02:
   ^    - output alignment coodinates memory allocation
   ^  o fun07 sec02 sub03:
   ^    - alignment rows memory allocate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec02 Sub01:
   *   - get lengths and offsets
   \****************************************************/

   alnSTPtr->refLenSI = refLenSL;
   alnSTPtr->qryLenSI = refLenSL;
   alnSTPtr->refOffsetSI = refSTPtr->offsetSL;
   alnSTPtr->qryOffsetSI = qrySTPtr->offsetSL;

   alnSTPtr->outLenSL = refLenSL + qryLenSL;

   /*****************************************************\
   * Fun07 Sec02 Sub02:
   *   - output alignment coodinates memory allocation
   \****************************************************/

   if(alnSTPtr->outSizeSL < alnSTPtr->outLenSL)
   { /*If: need to resize the alignment coordinates*/
      if(alnSTPtr->startArySL)
         free(alnSTPtr->startArySL);
      alnSTPtr->startArySL =
         malloc(alnSTPtr->outLenSL * sizeof(signed long));
      if(! alnSTPtr->startArySL)
         goto memErr_fun07_sec05_sub03;

      if(alnSTPtr->endArySL)
         free(alnSTPtr->endArySL);
      alnSTPtr->endArySL =
         malloc(alnSTPtr->outLenSL * sizeof(signed long));
      if(! alnSTPtr->endArySL)
         goto memErr_fun07_sec05_sub03;

      if(alnSTPtr->scoreArySL)
         free(alnSTPtr->scoreArySL);
      alnSTPtr->scoreArySL =
         malloc(alnSTPtr->outLenSL * sizeof(signed long));
      if(! alnSTPtr->scoreArySL)
         goto memErr_fun07_sec05_sub03;

      alnSTPtr->outSizeSL = alnSTPtr->outLenSL;
   } /*If: need to resize the alignment coordinates*/

   /*****************************************************\
   * Fun07 Sec02 Sub03:
   *   - alignment rows memory allocate
   \****************************************************/

   if(alnSTPtr->rowSizeSI < refLenSL)
   { /*If: need more memory for alignment rows*/
      if(alnSTPtr->indexRowSL)
         free(alnSTPtr->indexRowSL);
      alnSTPtr->indexRowSL =
         malloc((refLenSL + 1) * sizeof(signed long));
      if(! alnSTPtr->indexRowSL)
         goto memErr_fun07_sec05_sub03;

      if(alnSTPtr->scoreRowSL)
         free(alnSTPtr->scoreRowSL);
      alnSTPtr->scoreRowSL =
         malloc((refLenSL + 1) * sizeof(signed long));
      if(! alnSTPtr->scoreRowSL)
         goto memErr_fun07_sec05_sub03;

      if(alnSTPtr->dirRowSC)
         free(alnSTPtr->dirRowSC);
      alnSTPtr->dirRowSC =
         malloc((refLenSL + 1) * sizeof(signed long));
      if(! alnSTPtr->dirRowSC)
         goto memErr_fun07_sec05_sub03;

      alnSTPtr->rowSizeSI = refLenSL;
   } /*If: need more memory for alignment rows*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^  - initialize all values
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(indexSL=0; indexSL < alnSTPtr->outLenSL; ++indexSL)
   { /*loop; till have initalized the query scores*/
      alnSTPtr->startArySL[indexSL] = -1;
      alnSTPtr->endArySL[indexSL] = -1;
      alnSTPtr->scoreArySL[indexSL] = -1;
   } /*loop; till have initalized the query scores*/

   for(indexSL = 0; indexSL <= refLenSL; ++indexSL)
   { /*loop; till have initalized the first row*/
      alnSTPtr->dirRowSC[indexSL] = def_mvStop_alnDefs;
      alnSTPtr->indexRowSL[indexSL] = indexSL + 1;
      alnSTPtr->scoreRowSL[indexSL] = 0;
   } /*loop; till have initalized the first row*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^  - Fill the matrix with scores
   ^  o fun07 sec04 sub01:
   ^    - Final set up before scoring the matrix
   ^  o fun07 sec04 sub02:
   ^    - get snp and ins scores + start loop
   ^  o fun07 sec04 sub03:
   ^    - find high score
   ^  o fun07 sec04 sub04:
   ^    - check if keep score (score > 0)
   ^  o fun07 sec04 sub05:
   ^    - find next deletion score and move to next index
   ^  o fun07 sec04 sub07:
   ^    - prepare to score the next row in the matrix
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec04 Sub01:
   *  - Final set up before scoring the matrix
   \*****************************************************/

   /*Move the row of starting indexes to the last row*/
   alnSTPtr->indexRowSL[0] = indexSL;

   /*These are always negative*/
   delScoreSL = 0;
   nextSnpScoreSL = 0;
   snpIndexSL = alnSTPtr->indexRowSL[0];

   /*incurment to frist base*/
   ++indexSL;
   qrySeqStr = qrySTPtr->seqStr + qrySTPtr->offsetSL;
   refSeqStr = refSTPtr->seqStr + refSTPtr->offsetSL - 1;
      /*offseting reference by 1 to account for the gap
      `  column
      */

   /*****************************************************\
   * Fun07 Sec04 Sub02:
   *  - get snp and ins scores + start loop
   \*****************************************************/

   /*Starting on the first sequence row*/
   for(slQry = 0; slQry < qryLenSL; ++slQry)
   { /*loop; compare query base against all ref bases*/
      for(slRef = 1; slRef <= refLenSL; ++slRef)
      { /*loop; compare one query to one reference base*/

         snpScoreSL =
            getScore_alnSet(
               qrySeqStr[slQry],
               refSeqStr[slRef],
               settings
            ); /*find the score for the base pairs*/

         snpScoreSL += nextSnpScoreSL;
         nextSnpScoreSL = alnSTPtr->scoreRowSL[slRef];

         /*insertion score*/
         #ifdef NOEXTEND
            insScoreSL =
                 alnSTPtr->scoreRowSL[slRef]
               + settings->gapSS;
         #else
            insScoreSL = alnSTPtr->scoreRowSL[slRef];
            insScoreSL +=
               settings->insArySS[
                  alnSTPtr->dirRowSC[slRef]
               ];
         #endif

         /***********************************************\
         * Fun07 Sec04 Sub03:
         *   - find high score
         \***********************************************/

         alnSTPtr->scoreRowSL[slRef] =
            max_genMath(insScoreSL, snpScoreSL);
            /*find if ins/snp is best (5 Op)*/

         tmpIndexSL = alnSTPtr->indexRowSL[slRef];

         alnSTPtr->indexRowSL[slRef] =
            ifmax_genMath(
               insScoreSL,
               snpScoreSL,
               alnSTPtr->indexRowSL[slRef], /*ins index*/
               snpIndexSL                   /*snp index*/
            ); /*get index of high score*/

         snpIndexSL = tmpIndexSL;

         /*find direction (5 Op)*/
         alnSTPtr->dirRowSC[slRef] =
            alnSTPtr->scoreRowSL[slRef] > delScoreSL;
         alnSTPtr->dirRowSC[slRef] +=
            (
                 (snpScoreSL <= insScoreSL)
               & alnSTPtr->dirRowSC[slRef]
            );
         ++alnSTPtr->dirRowSC[slRef];

         /*Logic:
         `   - noDel: maxSC > delSc:
         `     o 1 if deletion not max score
         `     o 0 if deletion is max score
         `   - type: noDel + ((snpSc < insSc) & noDel):
         `     o 1 + (1 & 1) = 2 if insertion is maximum
         `     o 1 + (0 & 1) = 1 if snp is maximum
         `     o 0 + (0 & 0) = 0 if del is max; snp > ins
         `     o 0 + (1 & 0) = 0 if del is max, ins >= snp
         `   - dir: type + 1
         `     o adds 1 to change from stop to direction
         */

         /*finish finding max's*/
         alnSTPtr->indexRowSL[slRef] =
            ifmax_genMath(
               delScoreSL,
               alnSTPtr->scoreRowSL[slRef],
               alnSTPtr->indexRowSL[slRef-1],/*del index*/
               alnSTPtr->indexRowSL[slRef]/*current best*/
            ); /*get index of high score*/

         alnSTPtr->scoreRowSL[slRef] =
            max_genMath(
               delScoreSL,
               alnSTPtr->scoreRowSL[slRef]
         ); /*find if del is best (5 Op)*/
            
         /***********************************************\
         * Fun07 Sec04 Sub04:
         *   - check if keep score (score > 0)
         \***********************************************/

         if(alnSTPtr->scoreRowSL[slRef] <= 0)
         {
            alnSTPtr->dirRowSC[slRef] = 0;
            alnSTPtr->scoreRowSL[slRef] = 0;
            alnSTPtr->indexRowSL[slRef] = indexSL + 1;
            /*always one off for new index's*/
            /* branchless method is slower here*/
         }

         else
         { /*Else: check if have new high score*/
            scoreSL = alnSTPtr->scoreRowSL[slRef];

            if(scoreSL > alnSTPtr->scoreArySL[slRef-1])
            { /*If: new best outerence position score*/
               alnSTPtr->startArySL[slRef - 1] =
                   alnSTPtr->indexRowSL[slRef];
               alnSTPtr->endArySL[slRef - 1] = indexSL;
               alnSTPtr->scoreArySL[slRef-1] = scoreSL;
            } /*If: new best outerence position score*/

            if(
                 scoreSL
               > alnSTPtr->scoreArySL[slQry+startSL]
            ){ /*If: new best query position score*/
               alnSTPtr->startArySL[slQry + startSL] =
                  alnSTPtr->indexRowSL[slRef];
               alnSTPtr->endArySL[slQry + startSL] =
                  indexSL;
               alnSTPtr->scoreArySL[slQry + startSL] =
                  scoreSL;
            } /*If: new best query position score*/
         } /*Else: check if have new high score*/

         /***********************************************\
         * Fun07 Sec04 Sub05:
         *   - find next deletion score and move to next
         \***********************************************/

         #ifdef NOEXTEND
            delScoreSL =
                 alnSTPtr->scoreRowSL[slRef]
               + settings->gapSS;
         #else
            delScoreSL = alnSTPtr->scoreRowSL[slRef];
            delScoreSL +=
               settings->delArySS[
                  alnSTPtr->dirRowSC[slRef]
               ];
         #endif

         ++indexSL;
      } /*loop; compare one query to one reference base*/

     /***************************************************\
     *  Fun07 Sec04 Sub07:
     *   - prepare for the next round
     \***************************************************/

     /*Get scores set up for the gap column*/
	  nextSnpScoreSL = 0;
     delScoreSL = 0;

     alnSTPtr->indexRowSL[0] = indexSL; /*next index*/
     snpIndexSL = alnSTPtr->indexRowSL[0];

     ++indexSL; /*Set index for the next base pair*/
   } /*loop; compare query base against all ref bases*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec05:
   ^  - set up for returing the matrix (clean up/wrap up)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto cleanUp_fun07_sec05;

   memErr_fun07_sec05_sub03:;
      scoreSL = -1;
      goto cleanUp_fun07_sec05;

   cleanUp_fun07_sec05:;
      return scoreSL;
} /*memwaterScan*/

#ifdef NEW_DEBUG
/*-------------------------------------------------------\
| Fun08: trsGetLongest_memwaterScan
|   - find longest trs (tandum repeat sequence) repeat
| Input;
|   - readSTPtr:
|     o pointer to an seqST with the read to scan for trs
|       index 0 coordinates to start (offsetSL)/end
|       (endAlnSL) the alignment.
|   - trsSeqSTPtr:
|     o pointer to an seqST with the reference sequence
|       and index 0 coordinates to start (offsetSL)/end
|       (endAlnSL) the alignment.
|   - startSIPtr:
|     o signed int pionter set to 1st reference base in
|       the trs
|   - endSIPtr:
|     o signed int pionter set to last reference base in
|       the trs
|   - alnSTPtr:
|     o pointer to aln_memwaterScan structure to hold the
|       results of the alignment (and used to align)
|   - settings:
|     o pointer to an alnSet structure with the gap open,
|       gap extend, and scoring matrix for the alingment
| Output:
|  - Modifies:
|    o variables in alnSTPtr to have the new alignment
|    o startSIPtr to have the 1st reference coorindate in
|      the longest found tandum repeat sequence
|    o endSIPtr to have the 1st reference coorindate in
|      the longest found tandum repeat sequence
|  - Returns:
|    o score of longest trs
|    o negative number for memory errors
\-------------------------------------------------------*/
signed long
trsGetLongest_memwaterScan(
   struct seqST *readSTPtr, /*read/sequence to scan*/
   struct seqST *trsSTPtr,  /*ref sequence and data*/
   signed int *startSIPtr,  /*gets start of longest map*/
   signed int *endSIPtr,    /*gets end of longest map*/
   struct aln_memwaterScan *alnSTPtr,/*gets alignment*/
   struct alnSet *settings
){
   signed long scoreSL = 0;

   unsigned long refCoordAryUL[2]; /*0 = start; 1 = end*/
   unsigned long qryCoordAryUL[2]; /*0 = start; 1 = end*/

   unsigned long lastRefCoordAryUL[2];/*0=start; 1=end*/
   unsigned long lastQryCoordAryUL[2];/*0=start; 1=end*/
 
   signed int trsLenSI = 0;
   signed int indexSI = 0;
   signed int nextSI = 0;
   signed int qryNextSI = 0;
   signed int endSI = 0;
   signed short gapSS = trsSTPtr->seqLenUL / 10;
      /*limits to a 10% gap*/

   signed short indelSS = trsSTPtr->seqLenUL / 10;
      /*limits for gaps between bases*/

   *startSIPtr = 0;
   *endSIPtr = 0;

   scoreSL =
      memwaterScan(readSTPtr,trsSTPtr,alnSTPtr,settings);
   if(scoreSL < 0)
      goto memErr_fun08_sec0x;

   for(indexSI=0; indexSI < alnSTPtr->refLenSI; ++indexSI)
      alnSTPtr->scoreRowSL[indexSI] = 0;

   for(indexSI=0; indexSI < alnSTPtr->refLenSI; ++indexSI)
   { /*Loop: find longest length*/
      if(
           settings->minScoreSL
         < alnSTPtr->qryScoreSL[indexSI]
      ){ /*If: no valid score*/
         alnSTPtr->scoreRowSL[indexSI] = -2;
         continue;
      }  /*If: no valid score*/

      indexToCoord(
         alnSTPtr->refLenSI,
         alnSTPtr->qryStartArySL[indexSI],
         &lastRefCoordAryUL[0],
         &lastQryCoordAryUL[0]
      );

      indexToCoord(
         alnSTPtr->refLenSI,
         alnSTPtr->qryEndArySL[indexSI],
         &lastRefCoordAryUL[1],
         &lastQryCoordAryUL[1]
      );

      trsLenSI =
         lastRefCoordAryUL[1] - lastRefCoordAryUL[0];
      nextSI = lastRefCoordAryUL[1] + 1;
      endSI = nextSI + gapSS;

      while(nextSI <= endSI)
      { /*Loop: see if can extend trs*/
         indexToCoord(
            alnSTPtr->refLenSI,
            alnSTPtr->qryStartArySL[indexSI],
            &refCoordAryUL[0],
            &qryCoordAryUL[0]
         );

         indexToCoord(
            alnSTPtr->refLenSI,
            alnSTPtr->qryEndArySL[indexSI],
            &refCoordAryUL[1],
            &qryCoordAryUL[1]
         );

         if(
                (signed long) refCoordAryUL[0]
              - lastRefCoordUL[0]
            > gapUS
         ){ /*If: end of the repeat*/
            alnSTPtr->scoreRowSL[indexSI] = trsLenSI;

            nextSI = lastRefCoordAry[0] + 1;
            qryNextSI = lastQryCoordAry[0] + 1;
            while(nextSI < lastRefCoordAry[1])
            { /*Loop: fill in duplicate entries*/
               indexToCoord(
                  alnSTPtr->refLenSI,
                  alnSTPtr->qryStartArySL[indexSI],
                  &refCoordAryUL[0],
                  &qryCoordAryUL[0]
               );

               if(qryCoordAryUL[0] < qryNextSI - indelSS)
                  ;
               else if(
                  qryCoordAryUL[0] > qryNextSI + indelSS
               ) ;
               else
                  alnSTPtr->scoreRowSL[nextSI] = -1;

               ++nextSI;
               ++qryNextSI;
            } /*Loop: fill in duplicate entries*/

            break;
         } /*If: end of the repeat*/

         else if(
            qryCoordAryUL[0] - lastEndAryUL[1] > gapUS
         ){ /*Else If: end of the repeat*/
         } /*Else If: end of the repeat*/

      } /*Loop: see if can extend trs*/

      curStartSI = 
   } /*Loop: find longest length*/

   return scoreSL;

   memErr_fun08_sec0x:;
      return scoreSL;
} /*trsGetLongest_memwaterScan*/
#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
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
: Copyright (c) 2024 jeremyButtler
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
