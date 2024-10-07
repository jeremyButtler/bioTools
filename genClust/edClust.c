/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' edClust SOF: Start Of File
'   - clusters reads by similarity using edit distance
'   o header:
'     - included libraries
'   o fun01: getBestRead_edClust
'     - finds read with best score from a score array
'   o fun02: depthProf_edClust
'     - find the depth profile for a read
'   o fun03: findNumMap_edClust
'     - finds number of reads mapping to best read
'   o fun04: cluster_edClust
'     - clusters reads
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

#include "edClust.h"

#include <stdio.h>

#include "../genLib/ulCp.h"

#include "../genBio/samEntry.h"
#include "../genBio/tbCon.h"
#include "../genBio/edDist.h"

#include "clustST.h"

/*.h files only (no .c files)*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*only using .h min macro*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/tbConDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: getBestRead_edClust
|   - finds read with best score from a score array; fun01
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct to have best read
|   - indexSTPtr:
|     o pointer to index_clustST with score array to
|       search
|   - buffStrPtr:
|     o c-string to use reading sam file
|     o this must be big enough to get the entire line
|       use buffer used with mk_index_clustST to get this
|   - samFILE:
|     o sam file to find best read in
| Output:
|   - Modifies:
|     o samFILE to be at start
|     o samSTPtr to have selected read
|   - Returns:
|     o index of selected read in scoreArySC
|     o def_memErr_edClust for memory errors
|     o def_fileErr_edClust for file errors
|     o def_noReads_edClust if no reads in file
\-------------------------------------------------------*/
signed long
getBestRead_edClust(
   struct samEntry *samSTPtr,
   struct index_clustST *indexSTPtr,
   signed char *buffStr,
   void *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - finds read with best score from score array
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - find highest score
   '   o fun01 sec03:
   '     - get highest scoring read
   '   o fun01 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   sint bestScoreSI = 0;
   slong entrySL = 0;
   slong bestIndexSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - find highest score
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*I pay O(n) cost here, but O(1) cost for setting
   `   reads to -1 (discarded) later. Also, allows me to
   `   use the score index as the line number in the sam
   `   file.
   */

   for(
      entrySL = 0;
      entrySL < (slong) indexSTPtr->lenSamUL;
      ++entrySL
   ){ /*Loop: find best score*/

      if(indexSTPtr->clustArySI[entrySL])
         continue; /*not read or read already assigned*/

      if(bestScoreSI < indexSTPtr->scoreArySC[entrySL])
      { /*If: new best score*/
         bestScoreSI = indexSTPtr->scoreArySC[entrySL];
         bestIndexSL = entrySL;
      } /*If: new best score*/

   } /*Loop: find best score*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - get highest scoring read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSTPtr->clustArySI[bestIndexSL])
      goto noReadErr_fun01_sec04; /*no reads in file*/
      
   fseek(
      samFILE,
      indexSTPtr->indexAryUL[bestIndexSL],
      SEEK_SET
   );

   errSC =
      getRead_clustST(
         samSTPtr,
         (unsigned long) bestIndexSL,
         0,
         indexSTPtr,
         buffStr,
         samFILE
      );
      
   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_clustST)
         goto memErr_fun01_sec04;
      if(errSC == def_fileErr_clustST)
         goto fileErr_fun01_sec04;
   } /*If: had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun01_sec04;


   memErr_fun01_sec04:;
   bestIndexSL = def_memErr_edClust;
   goto ret_fun01_sec04;


   fileErr_fun01_sec04:;
   bestIndexSL = def_fileErr_edClust;
   goto ret_fun01_sec04;


   noReadErr_fun01_sec04:;
   bestIndexSL = def_noReads_edClust;
   goto ret_fun01_sec04;


   ret_fun01_sec04:;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   return bestIndexSL;
} /*getBestRead_edClust*/

/*-------------------------------------------------------\
| Fun02: depthProf_edClust
|   - find the depth profile for a read
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference (read)
|       to get profile for
|   - refNumUI:
|     o reference the reference read was mapped to
|   - clustSI:
|     o cluster working on
|   - indexSTPtr:
|     o pointer to index_clustST struct with file index's
|   - clustSetSTPtr:
|     o pointer to set_clustST struct with settings
|   - samSTPtr:
|     o pointer to samEntry struct to use in file reading
|   - buffStr:
|     o pointer to c-string use as buffer in file reading
|     o needs to be able to hold largest line in file
|   - samFILE:
|     o FILE pointer to sam file with reads
|   - errSCPtr:
|     o pointer to signed char to hold errors
| Output:
|   - Modifies:
|     o samSTPtr to have last read in file
|     o buffStr to have last line in file
|     o samFILE to point to the start
|     o errSCPtr to hold error messages:
|       - 0 for no error
|       - def_memErr_edClust for memory error
|       - def_fileErr_edClust for file error
|  - Returns:
|    o pointer to unsigned int array with depth profile
|    o 0 for errors
\-------------------------------------------------------*/
unsigned int *
depthProf_edClust(
   struct samEntry *refSTPtr, /*read to get profile for*/
   unsigned int refNumUI,   /*reference read mapped to*/
   signed int clustSI,               /*cluster on*/
   struct index_clustST *indexSTPtr, /*has file index's*/
   struct set_clustST *clustSetSTPtr,/*settings*/
   struct samEntry *samSTPtr,        /*for reading file*/
   signed char *buffStr,             /*for reading file*/
   void *samFILE,                    /*sam file*/
   signed char *errSCPtr             /*holds errors*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - find the depth profile for a read
   '   o fun02 sec01:
   '     - varaible declarations
   '   o fun02 sec02:
   '     - allocate memory and get start of file
   '   o fun02 sec03:
   '     - build depth profile
   '   o fun02 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - varaible declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int *depthHeapAryUI = 0; /*depth array*/
   unsigned long offsetUL = 0;
   unsigned long lineUL = 0;

   unsigned int overlapUI = 0;
   float qryPercOverlapF = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - allocate memory and get start of file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   depthHeapAryUI =
      calloc(
         refSTPtr->readLenUI + 9 + 128,
         sizeof(uint)
      ); /*allocate memory for depth array*/

   if(! depthHeapAryUI)
      goto memErr_fun02_sec04;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - build depth profile
   ^   o fun02 sec03 sub01:
   ^     - filter reads
   ^   o fun02 sec03 sub02:
   ^     - get read and add to profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *   - filter reads
   \*****************************************************/

   for(
      lineUL = 0;
      lineUL < indexSTPtr->lenSamUL;
      ++lineUL
   ){ /*Loop: add reads to depth profile*/

      if(indexSTPtr->clustArySI[lineUL] == 0) ;

      else if(indexSTPtr->clustArySI[lineUL] == clustSI) ;

      else
      { /*Else: already assigned or discarded*/
         offsetUL += indexSTPtr->lenLineAryUI[lineUL];
         continue;
      } /*Else: already assigned or discarded*/

      if(refNumUI != indexSTPtr->refAryUI[lineUL])
      { /*If: references are different*/
         offsetUL += indexSTPtr->lenLineAryUI[lineUL];
         continue;
      } /*If: references are different*/

      if(
           indexSTPtr->startAryUI[lineUL]
         > refSTPtr->refEndUI
      ){ /*If: references are different*/
         offsetUL += indexSTPtr->lenLineAryUI[lineUL];
         continue;
      } /*If: references are different*/

      if(
           indexSTPtr->endAryUI[lineUL]
         < refSTPtr->refStartUI
      ){ /*If: references are different*/
         offsetUL += indexSTPtr->lenLineAryUI[lineUL];
         continue;
      } /*If: references are different*/

      overlapUI =
         min_genMath(
            refSTPtr->refEndUI,
            indexSTPtr->endAryUI[lineUL]
         );

      overlapUI -=
         max_genMath(
            refSTPtr->refStartUI,
            indexSTPtr->startAryUI[lineUL]
         );

      qryPercOverlapF =
            (float) overlapUI
          / (float)
            (
                  indexSTPtr->endAryUI[lineUL]
                - indexSTPtr->startAryUI[lineUL]
            ); /*find percent overlap on query*/
      
      if(qryPercOverlapF < clustSetSTPtr->percOverlapF)
      { /*If: not enough overlap*/
         offsetUL += indexSTPtr->lenLineAryUI[lineUL];
         continue;
      } /*If: not enough overlap*/

      /**************************************************\
      * Fun02 Sec03 Sub02:
      *   - get read and add to profile
      \**************************************************/

      *errSCPtr =
         getRead_clustST(
            samSTPtr,
            lineUL,
            offsetUL,
            indexSTPtr,
            buffStr,
            samFILE
         ); /*get read from sam file*/

      offsetUL = 0;

      if(*errSCPtr)
      { /*If: had error*/
         if(*errSCPtr == def_memErr_clustST)
            goto memErr_fun02_sec04;
         if(*errSCPtr == def_fileErr_clustST)
            goto fileErr_fun02_sec04;
      } /*If: had error*/


      addReadToDepth_edDist(
         refSTPtr,
         samSTPtr,
         clustSetSTPtr->minSnpQUC,
         clustSetSTPtr->percOverlapF,
         depthHeapAryUI
      );
   } /*Loop: add reads to depth profile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun02_sec04;

   memErr_fun02_sec04:;
   *errSCPtr = def_memErr_edClust;
   goto errCleanUp_fun02_sec04;

   fileErr_fun02_sec04:;
   *errSCPtr = def_fileErr_edClust;
   goto errCleanUp_fun02_sec04;

   errCleanUp_fun02_sec04:;
   if(depthHeapAryUI)
      free(depthHeapAryUI);
   depthHeapAryUI = 0;
   goto ret_fun02_sec04;


   ret_fun02_sec04:;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   return depthHeapAryUI;
} /*depthProf_edClust*/

/*-------------------------------------------------------\
| Fun03: findNumMap_edClust
|   - finds number of reads mapping to best read
| Input:
|   - bestSTPtr:
|     o pointer to samEntry struct with best read
|   - bestIndexSL:
|     o index of best read (return from fun01)
|   - clustSetSTPtr:
|     o has settings for filtering and stores the first
|       and last possible base of the consensesus
|   - indexSTPtr:
|     o pointer to index_clustST struct with reads index's
|       and open reads
|   - clustSI:
|     o cluster working on (what to assign to targArySI)
|   - conBl:
|     o 1: use consensus to read error rate
|     o 0: use read to read error rate
|   - buffStrPtr:
|     o c-string to use reading sam file
|     o should be able to hold longest line in file
|       (fun19 sets this up)
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - samFILE:
|     o sam file with reads to compare to best read
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o startUI and endUI in clustSetSTPtr to have start
|       and end of the consensus
|     o clustArySI in indexSTPtr to have hits assigned to
|       clustSI
|   - Returns:
|     o number of reads shared with best read
|     o def_memErr_edClust for memory errors
|     o def_fileErr_edClust for file errors
\-------------------------------------------------------*/
signed long
findNumMap_edClust(
   struct samEntry *bestSTPtr,
   signed long bestIndexSL,
   struct set_clustST *clustSetSTPtr,
   struct index_clustST *indexSTPtr,
   signed int clustSI,
   signed char conBl,
   struct samEntry *samSTPtr,
   signed char *buffStr,
   void *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - finds number of reads mapping to best read
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - get first entry in sam file
   '   o fun03 sec03:
   '     - find number of hits
   '   o fun03 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   slong distSL = 0;
   slong hitsSL = 0;
   slong lineSL = 0;

   uint overlapUI = 0;
   uint numIndelUI = 0;    /*here for function call*/
   uint indelEventsUI = 0; /*here for function call*/
   uint *depthHeapUI = 0;  /*depth profile array*/

   float qryPercOverlapF = 0;
   float errRateF = 0;

   ulong offsetUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get first entry in sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(conBl)
      errRateF = clustSetSTPtr->conErrRateF;
   else
      errRateF = clustSetSTPtr->readErrRateF;

   clustSetSTPtr->startUI = 0;
   clustSetSTPtr->endUI = 0;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   if(clustSetSTPtr->depthProfBl)
   { /*If: doing depth profiling*/
      depthHeapUI =
         depthProf_edClust(
            bestSTPtr,
            indexSTPtr->refAryUI[bestIndexSL],
            clustSetSTPtr->clustSI,
            indexSTPtr,
            clustSetSTPtr,
            samSTPtr,
            buffStr,
            samFILE,
            &errSC
         ); /*get read depth profile*/

      if(errSC)
      { /*If: had error*/
         if(errSC == def_memErr_edClust)
            goto memErr_fun03_sec04;
         if(errSC == def_fileErr_edClust)
            goto fileErr_fun03_sec04;
      } /*If: had error*/
   } /*If: doing depth profiling*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - find number of hits
   ^   o fun03 sec03 sub01:
   ^     - filter reads + start loop
   ^   o fun03 sec03 sub02:
   ^     - get read
   ^   o fun03 sec03 sub03:
   ^     - find edit distance and decide if keep
   ^   o fun03 sec03 sub04:
   ^     - move to next read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - filter reads + start loop
   \*****************************************************/

   for(
      lineSL = 0;
      lineSL < (slong) indexSTPtr->lenSamUL;
      ++lineSL
   ){ /*Loop: find number of matches*/
      if(indexSTPtr->clustArySI[lineSL] == 0) ;

      else if(indexSTPtr->clustArySI[lineSL] == clustSI) ;

      else
      { /*Else: already assigned or discarded*/
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*Else: already assigned or discarded*/

      if(
            indexSTPtr->refAryUI[bestIndexSL]
         != indexSTPtr->refAryUI[lineSL]
      ){ /*If: references are different*/
         indexSTPtr->clustArySI[lineSL] = 0;
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*If: references are different*/

      if(
           indexSTPtr->startAryUI[lineSL]
         > bestSTPtr->refEndUI
      ){ /*If: references are different*/
         indexSTPtr->clustArySI[lineSL] = 0;
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*If: references are different*/

      if(
           indexSTPtr->endAryUI[lineSL]
         < bestSTPtr->refStartUI
      ){ /*If: references are different*/
         indexSTPtr->clustArySI[lineSL] = 0;
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*If: references are different*/

      overlapUI =
         (float)
         min_genMath(
            bestSTPtr->refEndUI,
            indexSTPtr->endAryUI[lineSL]
         );

      overlapUI -=
         (float)
         max_genMath(
            bestSTPtr->refStartUI,
            indexSTPtr->startAryUI[lineSL]
         );

      qryPercOverlapF =
            (float) overlapUI
          / (float)
            (
                  indexSTPtr->endAryUI[lineSL]
                - indexSTPtr->startAryUI[lineSL]
            ); /*find percent overlap on query*/
      
      if(qryPercOverlapF < clustSetSTPtr->percOverlapF)
      { /*If: not enough overlap*/
         indexSTPtr->clustArySI[lineSL] = 0;
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*If: not enough overlap*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - get read
      \**************************************************/

      errSC =
         getRead_clustST(
            samSTPtr,
            (ulong) lineSL,
            offsetUL,
            indexSTPtr,
            buffStr,
            samFILE
         ); /*get read from sam file*/

      offsetUL = 0;

      if(errSC)
      { /*If: had error*/
         if(errSC == def_memErr_clustST)
            goto memErr_fun03_sec04;
         if(errSC == def_fileErr_clustST)
            goto fileErr_fun03_sec04;
      } /*If: had error*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - find edit distance and decide if keep
      \**************************************************/

      distSL =
         readCmpDist_edDist(
             samSTPtr,
             bestSTPtr,
             clustSetSTPtr->indelLenUI,
             clustSetSTPtr->minSnpQUC,
             clustSetSTPtr->percOverlapF,
             clustSetSTPtr->minDepthUI,
             depthHeapUI,
             &overlapUI,
             &numIndelUI,    /*ignoring*/
             &indelEventsUI  /*ignoring*/
         );

      if(distSL < 0)
      { /*If: distance to small*/
         indexSTPtr->clustArySI[lineSL] = 0;
         continue;
      } /*If: distance to small*/

      qryPercOverlapF = (float) overlapUI;
      qryPercOverlapF /= (float) samSTPtr->alnReadLenUI;

      if(qryPercOverlapF < clustSetSTPtr->percOverlapF)
      { /*If: to little overlap with reference*/
         indexSTPtr->clustArySI[lineSL] = 0;
         continue;
      } /*If: to little overlap with reference*/

      distSL =
         percDist_edDist(
            distSL,
            overlapUI,
            errRateF
         ); /*find variant:error ratio*/

      if(distSL > clustSetSTPtr->varToErrUI)
      { /*If: reads are to distant*/
         indexSTPtr->clustArySI[lineSL] = 0;
         continue;
      } /*If: reads are to distant*/

      indexSTPtr->clustArySI[lineSL] = clustSI;
      ++hitsSL;

      if(clustSetSTPtr->startUI == 0)
         clustSetSTPtr->startUI = samSTPtr->refStartUI;

      clustSetSTPtr->startUI =
         min_genMath(
            samSTPtr->refStartUI,
            clustSetSTPtr->startUI
         );

      clustSetSTPtr->endUI =
         max_genMath(
            samSTPtr->refEndUI,
            clustSetSTPtr->endUI
         );
   } /*Loop: find number of matches*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun03_sec04;

   memErr_fun03_sec04:;
   hitsSL = def_memErr_edClust;
   goto ret_fun03_sec04;

   fileErr_fun03_sec04:;
   hitsSL = def_fileErr_edClust;
   goto ret_fun03_sec04;

   ret_fun03_sec04:;

   if(depthHeapUI)
      free(depthHeapUI);
   depthHeapUI = 0;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   return hitsSL;
} /*findNumMap_edClust*/

/*-------------------------------------------------------\
| Fun04: cluster_edClust
|   - clusters reads
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST pointer to set to
|       clusters (should be null if not; is freeded)
|   - clustSetSTPtr:
|     o pointer to set_clustST struct with cluster setting
|   - conSetSTPtr:
|     o pointer to set_tbCon struct with consensus setting
|   - samSTPtr:
|     o pointer to samEntry struct to use in reading file
|   - buffStrPtr:
|     o pointer to c-string to use reading sam files
|   - lenBuffULPtr:
|     o pointer to unsigned long with buffStrPtr length
|   - samFILE:
|     o sam file with reads to cluster
|   - logFILE:
|     o file to print log to (use 0 for no logging)
|   - errSCPtr:
|     o pointer to signed char to hold error type
| Output:
|   - Modifies:
|     o all arries clustSTPtr to have clusters and index's
|     o buffStrPtr to be resized to longest line in file
|     o lenBuffULPtr to hav buffStrPtr's length
|     o samFILE to point to start
|     o clustSI in clustSetSTPtr to be on last cluster
|     o errSC:
|       o 0 for no errors
|       o def_memErr_edClust for memory errors
|       o def_fileErr_edClust for file errors
|       o def_noReads_edClust if to few reads
|   - Prints:
|     o log (stats; results, and errors) to logFILE
|   - Returns:
|     o list of con_clustST structs with consensus
|       clusters
\-------------------------------------------------------*/
con_clustST *
cluster_edClust(
   struct index_clustST **indexSTPtr,
   struct set_clustST *clustSetSTPtr,
   struct set_tbCon *conSetSTPtr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   unsigned long *lenBuffULPtr,
   void *samFILE,
   void *logFILE,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - cluster reads
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - initialize and set up variables
   '   o fun04 sec03:
   '     - set up index array (and score)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;         /*for error checking*/

   ulong numReadsUL = 0;    /*number of reads left*/
   ulong clustReadsUL = 0;  /*number reads clustered*/
   slong depthSL = 0;       /*read depth for cluster*/
    
   uchar ucCon = 0;    /*iterator for consensus building*/

   struct samEntry bestSamStackST;/*read to cluster with*/
   slong bestIndexSL = 0;         /*index of best read*/

   struct samEntry *conSamHeapST = 0; /*for consensus*/

   struct con_clustST *retHeapST = 0;
   struct con_clustST *conNodeST = 0;
   struct con_clustST *cmpNodeST = 0;

   schar profBl = clustSetSTPtr->depthProfBl;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - initialize and set up variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*indexSTPtr)
      freeHeap_index_clustST(*indexSTPtr);
   *indexSTPtr = 0;

   init_samEntry(&bestSamStackST);

   errSC = setup_samEntry(&bestSamStackST);

   if(errSC)
      goto memErr_fun04_sec04_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - set up index array (and score)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   numReadsUL = getNumLines_clustST(samFILE);

   if(! numReadsUL)
      goto fileErr_fun04_sec04_sub03;

   *indexSTPtr =
      mk_index_clustST(
         numReadsUL + 1, /*number lines in sam file*/
         clustSetSTPtr,  /*has settings for indexing*/
         samSTPtr,       /*for reading sam file*/
         buffStrPtr,     /*for reading sam file*/
         lenBuffULPtr,   /*length of buffStrPtr*/
         samFILE 
      );

   if(! indexSTPtr)
      goto memErr_fun04_sec04_sub02;

   if(
        (*indexSTPtr)->keptSL
      < (slong) clustSetSTPtr->minDepthUI
   ) goto lowReads_fun04_sec04_sub04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - cluster reads
   ^   o fun04 sec04 sub01:
   ^     - print progress + start loop
   ^   o fun04 sec04 sub02:
   ^     - get best read and its cluster
   ^   o fun04 sec04 sub03:
   ^     - build consensus and get depth
   ^   o fun04 sec04 sub04:
   ^     - compare consensus (and update read depth)
   ^   o fun04 sec04 sub05:
   ^     - print comparision result to log
   ^   o fun04 sec04 sub06:
   ^     - if merged, try to rebuild consensus
   ^   o fun04 sec04 sub07:
   ^     - zero extra pointers before moving on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec04 Sub01:
   *   - print progress + start loop
   \*****************************************************/

   numReadsUL = (*indexSTPtr)->keptSL;

   if(logFILE)
      fprintf(
         logFILE,
         "kept reads: %lu\n\n",
         numReadsUL
      );

   clustSetSTPtr->clustSI = 1; /*on first cluster*/

   while(numReadsUL >= clustSetSTPtr->minDepthUI)
   { /*Loop: cluster reads*/

      fprintf(
         stderr,
         "\r%07lu",
         numReadsUL
      ); /*update on how many reads left*/

      fflush(stderr);

      /**************************************************\
      * Fun04 Sec04 Sub02:
      *   - get best read and its cluster
      \**************************************************/

      bestIndexSL =
         getBestRead_edClust(
            &bestSamStackST,/*get un-clusetred best read*/
            *indexSTPtr,    /*has cluster assignments*/
            *buffStrPtr,    /*for getting best read*/
            samFILE
         ); /*find best read to cluster with*/

      if(bestIndexSL < 0)
      { /*If: had problem*/
         if(bestIndexSL == def_noReads_edClust)
            break; /*finished*/

         else if(bestIndexSL == def_memErr_edClust)
            goto memErr_fun04_sec04_sub02;

         else
            goto fileErr_fun04_sec04_sub03;
      } /*If: had problem*/

      depthSL =
         findNumMap_edClust(
            &bestSamStackST, /*read to cluster around*/
            bestIndexSL,  /*read index to cluster around*/
            clustSetSTPtr, /*settings for clustering*/
            *indexSTPtr,   /*has cluster array to update*/
            clustSetSTPtr->clustSI, /*cluster on*/
            0,             /*not a consensus*/
            samSTPtr,      /*for getting reads*/
            *buffStrPtr, /*for getting reads*/
            samFILE
         ); /*find cluster depth for best read*/

      if(depthSL < (sint) clustSetSTPtr->minDepthUI)
      { /*If: read depth to low*/
         (*indexSTPtr)->clustArySI[bestIndexSL] =
            def_discard_clustST;/*mark as discarded read*/

         --numReadsUL; /*1 less read*/
         continue;     /*find next best read*/
      } /*If: read depth to low*/

      /**************************************************\
      * Fun04 Sec04 Sub03:
      *   - build consensus and get depth
      \**************************************************/
      
      for(
         ucCon = 0;
         ucCon <= clustSetSTPtr->conRebuildUC;
         ++ucCon
      ){ /*Loop: build cosensus*/
         if(conSamHeapST)
            freeHeap_samEntry(conSamHeapST);
         conSamHeapST = 0;

         clustSetSTPtr->depthProfBl = 0;

         conSamHeapST =
            getCon_clustST(
               *indexSTPtr,
               clustSetSTPtr->clustSI,
               clustSetSTPtr,
               conSetSTPtr,
               samSTPtr,
               *buffStrPtr,
               samFILE,
               &errSC
            ); /*build the consensus*/

         clustSetSTPtr->depthProfBl = profBl;

         if(errSC)
         { /*If: had an error*/
            if(errSC == def_noCon_clustST)
               break; /*could not build consensus*/
            else
               goto memErr_fun04_sec04_sub02;
         } /*If: had an error*/


         depthSL =
            findNumMap_edClust(
               conSamHeapST,  /*consensus to cluster*/
               bestIndexSL,   /*original read index*/
               clustSetSTPtr, /*settings for clustering*/
               *indexSTPtr,   /*cluster array to update*/
               clustSetSTPtr->clustSI,  /*cluster on*/
               1,             /*is a consensus*/
               samSTPtr,      /*for getting reads*/
               *buffStrPtr,    /*for getting reads*/
               samFILE
            ); /*find cluster depth for best read*/

         if(depthSL < (sint) clustSetSTPtr->minDepthUI)
            break; /*not enough reads for cluster*/
      } /*Loop: build cosensus*/

      if(
            errSC == def_noCon_clustST
         || depthSL < (sint) clustSetSTPtr->minDepthUI
      ){ /*If: could not build consensus or low depth*/
         (*indexSTPtr)->clustArySI[bestIndexSL] =
            def_discard_clustST;/*mark as discarded read*/

         --numReadsUL; /*1 less read*/

         if(conSamHeapST)
            freeHeap_samEntry(conSamHeapST);
         conSamHeapST = 0;

         continue;     /*find next best read*/
      } /*If: could not build consensus or low depth*/

      /**************************************************\
      * Fun04 Sec04 Sub04:
      *   - compare consensus (and update read depth)
      \**************************************************/

      conNodeST =
         mk_con_clustST(
            conSamHeapST,
            clustSetSTPtr->clustSI,
            (ulong) depthSL
         ); /*build node con_clustST node for consensus*/

      conNodeST->startUI = clustSetSTPtr->startUI;
      conNodeST->endUI = clustSetSTPtr->endUI;

      if(! conNodeST)
         goto memErr_fun04_sec04_sub02;

      clustReadsUL += depthSL;
      numReadsUL -= depthSL;
         /*at this points reads are in a cluster*/

      if(logFILE)
         fprintf(
            logFILE,
            "clust %i: %lu reads\t%s\n",
            clustSetSTPtr->clustSI,
            depthSL,
            bestSamStackST.qryIdStr
         );

      if(! retHeapST)
         retHeapST = conNodeST;

      else
      { /*Else: not first cosensus*/
         cmpNodeST = retHeapST; /*first cosensus*/

         depthSL =
            cmpCons_clustST(
               cmpNodeST,
               conNodeST,
               *indexSTPtr,
               clustSetSTPtr
            ); /*compare cons and merge similar clusters*/

         while(cmpNodeST->nextST)
         { /*Loop: compare all consensus*/
            if(depthSL)
            { /*If: merged clusters*/
               freeHeap_con_clustST(conNodeST);
               conNodeST = 0;
               break;
            } /*If: merged clusters*/

            cmpNodeST = cmpNodeST->nextST;

            depthSL =
               cmpCons_clustST(
                  cmpNodeST,
                  conNodeST,
                  *indexSTPtr,
                  clustSetSTPtr
               ); /*compare and merge similar clusters*/
         } /*Loop: compare all consensus*/

         if(depthSL)
         { /*If: merged clusters*/
            freeHeap_con_clustST(conNodeST);
            conNodeST = 0;
         } /*If: merged clusters*/

         else
            cmpNodeST->nextST = conNodeST; /*add to list*/
      } /*Else: not first cosensus*/

      /**************************************************\
      * Fun04 Sec04 Sub05:
      *   - print comparison result to log
      \**************************************************/

      if(conNodeST)
      { /*If: kept cluster*/
         conNodeST = 0;
         ++clustSetSTPtr->clustSI;/*move to next cluster*/
      } /*If: kept cluster*/

      else
      { /*Else: merged clusters*/
         if(logFILE)
            fprintf(
               logFILE,
               "clust %i->%i: clust %i has %lu reads\n",
               clustSetSTPtr->clustSI,
               cmpNodeST->clustSI,
               cmpNodeST->clustSI,
               cmpNodeST->numReadsUL
            );

         /***********************************************\
         * Fun04 Sec04 Sub06:
         *   - if merged, try to rebuild consensus
         \***********************************************/

         clustSetSTPtr->startUI = cmpNodeST->startUI;
         clustSetSTPtr->endUI = cmpNodeST->endUI;

         conSamHeapST =
            getCon_clustST(
               *indexSTPtr,
               cmpNodeST->clustSI,
               clustSetSTPtr,
               conSetSTPtr,
               samSTPtr,
               *buffStrPtr,
               samFILE,
               &errSC
            ); /*rebuild the consensus*/

         if(errSC == def_memErr_clustST)
            goto memErr_fun04_sec04_sub02;

         if(errSC != def_noCon_clustST)
         { /*If: rebuilt consensus*/
            freeHeap_samEntry(cmpNodeST->samSTPtr);
            cmpNodeST->samSTPtr = conSamHeapST;
         } /*If: rebuilt consensus*/
      } /*Else: merged clusters*/

      /**************************************************\
      * Fun04 Sec04 Sub07:
      *   - zero extra pointers before moving on
      \**************************************************/

      if(logFILE)
         fflush(logFILE);

      cmpNodeST = 0;
      conSamHeapST = 0;
   } /*Loop: cluster reads*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - clean up
   ^   o fun04 sec04 sub01:
   ^     - no error clean up
   ^   o fun04 sec04 sub02:
   ^     - memory error clean up
   ^   o fun04 sec04 sub03:
   ^     - file error clean up
   ^   o fun04 sec04 sub04:
   ^     - to few reads error clean up
   ^   o fun04 sec04 sub05:
   ^     - error clean up (all errors call)
   ^   o fun04 sec04 sub06:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec04 Sub01:
   *   - no error clean up
   \*****************************************************/

   if(logFILE)
   { /*If: logging*/
      fprintf(
         logFILE,
         "\nkept reads: %li\n",
         (*indexSTPtr)->keptSL
         
      );

      fprintf(
         logFILE,
         "clustered reads: %lu\n",
         clustReadsUL
      );

      fprintf(
         logFILE,
         "percent clustered:%0.2f\n",
         100
           * (
                (float) clustReadsUL
              / (float) (*indexSTPtr)->keptSL
            )
      );

      fflush(logFILE);
   } /*If: logging*/

   fprintf(
      stderr,
      "\nkept reads: %li\n",
      (*indexSTPtr)->keptSL
      
   );

   fprintf(
      stderr,
      "clustered reads: %lu\n",
      clustReadsUL
   );

   fprintf(
      stderr,
      "percent clustered:%0.2f\n",
      100
        * (
             (float) clustReadsUL
           / (float) (*indexSTPtr)->keptSL
         )
   );

   fflush(stderr);

   *errSCPtr = 0;
   goto ret_fun04_sec04_sub06;

   /*****************************************************\
   * Fun04 Sec04 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun04_sec04_sub02:;

   if(logFILE)
      fprintf(
         logFILE,
         "memory error when (%lu) reads left\n",
         numReadsUL
      );

   *errSCPtr = def_memErr_edClust;
   goto errCleanup_fun04_sec04_sub05;

   /*****************************************************\
   * Fun04 Sec04 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_fun04_sec04_sub03:;
   *errSCPtr = def_fileErr_edClust;

   if(logFILE)
      fprintf(
         logFILE,
         "file error when (%lu) reads left\n",
         numReadsUL
      );

   goto errCleanup_fun04_sec04_sub05;

   /*****************************************************\
   * Fun04 Sec04 Sub04:
   *   - to few reads error clean up
   \*****************************************************/

   lowReads_fun04_sec04_sub04:;
   *errSCPtr = def_noReads_edClust;

   if(logFILE)
      fprintf(
         logFILE,
         "to few reads (%lu) after filterin to cluster\n",
         (*indexSTPtr)->keptSL
      );

   goto errCleanup_fun04_sec04_sub05;

   /*****************************************************\
   * Fun04 Sec04 Sub05:
   *   - error clean up (all errors call)
   \*****************************************************/

   errCleanup_fun04_sec04_sub05:;
   if(retHeapST)
      freeHeapList_con_clustST(retHeapST);
   retHeapST = 0;
   goto ret_fun04_sec04_sub06;

   /*****************************************************\
   * Fun04 Sec04 Sub06:
   *   - general clean up (everything calls)
   \*****************************************************/

   ret_fun04_sec04_sub06:;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   freeStack_samEntry(&bestSamStackST);
   return retHeapST;
} /*cluster_edClust*/

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
