/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' clustST SOF: Start Of File
'   - holds structures used in clustST and clustST
'     (for clustering)
'   o TOF: Table Of Functions
'     - tof00: header entry
'     - tof01: structures in this file
'     - tof02: set_clustST support fuctions
'     - tof03: index_clustST general functions
'     - tof04: con_clust general functions
'     - tof05: build/use index_clustST; consensus to
'     - tof06: consensus and cluster functions
'   - TOF00: header entry
'     o header:
'       - included libraries
'   - TOF01: structures
'     o .h st01: set_clustST
'       - has settings for clustering
'     o .h st02: index_clustST
'       - holds scores, clusters, and index's for sam file
'       - ~ 29 bytes per line
'     o .h st03: con_clustST
'       - holds a single consensus/profile for a cluster
'     o .h st04: var_clustST
'       - holds depth of variants in reference (clustST)
'     o .h st05: hist_clustST
'       - holds profile history for reads (clustST)
'   - TOF02: set_clustST support fuctions
'     o fun01: blank_set_clustST
'       - sets start and end to 0
'     o fun02: init_set_clustST
'       - sets settings in set_clustST struct to defaults
'     o fun03: freeStack_set_clustST
'       - no internal variables to free (here for support)
'     o fun04: freeHeap_set_clustST
'       - frees a heap allocated set_clustST struct
'   - TOF03: index_clustST general functions
'     o fun05: blank_index_clustST
'       - blanks an index_clustST struct
'     o fun06: init_index_clustST
'       - initializes an index_clustST struct
'     o fun07: freeStack_index_clustST
'       - frees and initializes variables in index_clustST
'     o fun09: freeHeap_index_clustST
'       - frees index_clustST struct
'     o fun10: setup_index_clustST
'       - setsup memory for index_clustST struct
'     o fun11: realloc_index_clustST
'       - reallocates memory for an index_clustST struct
'   - TOF04: con_clust general functions
'     o fun12: blank_con_clustST
'       - blank an clustST con_clustST struct
'     o fun13: init_con_clustST
'       - initialzie an clustST con_clustST struct
'     o fun14: freeStack_con_clustST
'       - frees variables inside an con_clustST struct
'     o fun15: freeHeap_con_clustST
'       - frees a con_clustST struct
'     o fun16: freeHeapList_con_clustST
'       - frees list of heap allocated con_clustST structs
'     o fun17: mk_con_clustST
'       - makes a con_clustST struct on the heap
'     o fun18: swap_con_clustST
'       - swaps to con_clustST structs (except nextST ptr)
'   - TOF05: build/use index_clustST struct
'     o fun20: mk_index_clustST
'       - get read scores for a sam file
'     o fun21: getRead_clustST
'       - gets a read by index from a sam file
'   - TOF06: consensus and cluster functions
'     o fun22: getCon_clustST
'       - builds a consensus for a cluster
'     o fun23: cmpCons_clustST
'       - compares two consensus clusters to see if same
'     o fun24: plist_con_clustST
'      - print clusters consensuses in a con_clustST list
'     o fun25: getClust_clustST
'       - extracts reads for a signle cluster
'     o fun26: pbins_clustST
'       - extracts reads for each cluster to a sam file
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

#include "clustST.h"

#include <stdio.h>

#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genLib/strAry.h"
#include "../genLib/fileFun.h"

#include "../genBio/samEntry.h"
#include "../genBio/edDist.h"
#include "../genBio/tbCon.h"

/*.h files only (no .c files)*/
#include "../genLib/endLine.h"
#include "../genLib/genMath.h" /*only using .h min macro*/
#include "../genBio/tbConDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_set_clustST
|   - sets start and end to 0
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct to blank
| Output:
|   - Modifies:
|     o startUI and endUI in clustSetSTPtr to be 0
\-------------------------------------------------------*/
void
blank_set_clustST(
   struct set_clustST *clustSetSTPtr
){
   clustSetSTPtr->startUI = 0;
   clustSetSTPtr->endUI = 0;
   clustSetSTPtr->clustSI = 0;
} /*blank_set_clustST*/

/*-------------------------------------------------------\
| Fun02: init_set_clustST
|   - sets all settings in a set_clustST struct to
|     defaults
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct to initialize
| Output:
|   - Modifies:
|     o all variables in clustSetSTPtr to be defaults
\-------------------------------------------------------*/
void
init_set_clustST(
   struct set_clustST *clustSetSTPtr
){
   clustSetSTPtr->repIntervalSL = def_repInterval_clustST;

   /*unique to clustST*/
   clustSetSTPtr->minProbF = def_minProb_clustST;
   clustSetSTPtr->minVarUI = def_minVar_clustST;

   /*unique to clustST*/
   clustSetSTPtr->conRebuildUC = def_conRebuild_clustST;
   clustSetSTPtr->depthProfBl = def_depthProfile_clustST;
   clustSetSTPtr->readErrRateF = def_readErrRate_clustST;
   clustSetSTPtr->conErrRateF = def_conErrRate_clustST;
   clustSetSTPtr->varToErrUI = def_maxVarErrRatio_clustST;
   clustSetSTPtr->lenWeightF = def_lenWeight_clustST;

   /*initial read filtering settings*/
   clustSetSTPtr->minLenUI = def_minLen_clustST;
   clustSetSTPtr->minMapqUC = def_minMapq_clustST;
   clustSetSTPtr->minMedQUI = def_minMedQ_clustST;
   clustSetSTPtr->minAvgQUI = def_minAvgQ_clustST;

   clustSetSTPtr->minPercDepthF =def_minPercDepth_clustST;
   clustSetSTPtr->percOverlapF = def_percOverlap_clustST;

   clustSetSTPtr->winSizeUI = def_window_clustST;
   clustSetSTPtr->winErrUI = def_windowError_clustST;

   /*consensus filtering*/
   clustSetSTPtr->maxConSimF = def_maxConSim_clustST;
   clustSetSTPtr->maxNPercF = def_maxNPerc_clustST;

   /*variant filtering*/
   clustSetSTPtr->minDepthUI = def_minDepth_clustST;
   clustSetSTPtr->indelLenUI = def_indelLen_clustST;
   clustSetSTPtr->minSnpQUC = def_minSnpQ_clustST;

   blank_set_clustST(clustSetSTPtr);
} /*init_set_clustST*/

/*-------------------------------------------------------\
| Fun03: freeStack_set_clustST
|   - no internal variables to free (here for support)
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct with variables to
|       free
| Output:
|   - Modifies:
|     o initializes all variables (nothing to free)
\-------------------------------------------------------*/
void
freeStack_set_clustST(
   struct set_clustST *clustSetSTPtr
){
   if(clustSetSTPtr)
      init_set_clustST(clustSetSTPtr);
} /*freeStack_set_clustST*/

/*-------------------------------------------------------\
| Fun04: freeHeap_set_clustST
|   - frees a heap allocated set_clustST struct
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct to free
| Output:
|   - Frees:
|     o clustSetSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_set_clustST(
   struct set_clustST *clustSetSTPtr
){
   if(clustSetSTPtr)
   { /*If: have someting to free*/
      freeStack_set_clustST(clustSetSTPtr);
      free(clustSetSTPtr);
   } /*If: have someting to free*/
} /*freeHeap_set_clustST*/

/*-------------------------------------------------------\
| Fun05: blank_index_clustST
|   - blanks an index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to blank
| Output:
|   - Modifies:
|     o lenSamUL, keptUL, and numRefUI in struct be 0
\-------------------------------------------------------*/
void
blank_index_clustST(
   struct index_clustST *indexSTPtr
){
   indexSTPtr->lenSamUL = 0;
   indexSTPtr->keptSL = 0;
   indexSTPtr->numRefUI = 0;
} /*blank_index_clustST*/

/*-------------------------------------------------------\
| Fun06: init_index_clustST
|   - initializes an index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to initializes
| Output:
|   - Modifies:
|     o all arrays to be null and lengths to be 0
\-------------------------------------------------------*/
void
init_index_clustST(
   struct index_clustST *indexSTPtr
){
   indexSTPtr->scoreArySC = 0;
   indexSTPtr->startAryUI = 0;
   indexSTPtr->endAryUI = 0;
   indexSTPtr->refAryUI = 0;
   indexSTPtr->clustArySI = 0;

   indexSTPtr->indexAryUL = 0;
   indexSTPtr->lenLineAryUI = 0;

   indexSTPtr->lenUL = 0;

   indexSTPtr->refIdAryStr = 0;
   indexSTPtr->refNumAryUI = 0;
   indexSTPtr->lenRefUI = 0;

   blank_index_clustST(indexSTPtr);
} /*init_index_clustST*/

/*-------------------------------------------------------\
| Fun07: freeStack_index_clustST
|   - frees and initializes variables in index_clustST
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct with variables to
|       free
| Output:
|   - Frees:
|     o all arrays, then initializes
\-------------------------------------------------------*/
void
freeStack_index_clustST(
   struct index_clustST *indexSTPtr
){
   if(indexSTPtr)
   { /*If: have structure*/
      if(indexSTPtr->scoreArySC)
         free(indexSTPtr->scoreArySC);

      if(indexSTPtr->startAryUI)
         free(indexSTPtr->startAryUI);

      if(indexSTPtr->endAryUI)
         free(indexSTPtr->endAryUI);

      if(indexSTPtr->refAryUI)
         free(indexSTPtr->refAryUI);

      if(indexSTPtr->clustArySI)
         free(indexSTPtr->clustArySI);


      if(indexSTPtr->indexAryUL)
         free(indexSTPtr->indexAryUL);
      indexSTPtr->indexAryUL = 0;

      if(indexSTPtr->lenLineAryUI)
         free(indexSTPtr->lenLineAryUI);


      if(indexSTPtr->refIdAryStr)
         free(indexSTPtr->refIdAryStr);

      if(indexSTPtr->refNumAryUI)
         free(indexSTPtr->refNumAryUI);

      init_index_clustST(indexSTPtr);
   } /*If: have structure*/
} /*freeStack_index_clustST*/

/*-------------------------------------------------------\
| Fun09: freeHeap_index_clustST
|   - frees index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to free
| Output:
|   - Frees:
|     o indexSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_index_clustST(
   struct index_clustST *indexSTPtr
){
   if(indexSTPtr)
   { /*If: have struct to free*/
      freeStack_index_clustST(indexSTPtr);
      free(indexSTPtr);
   } /*If: have struct to free*/
} /*freeHeap_index_clustST*/

/*-------------------------------------------------------\
| Fun10: setup_index_clustST
|   - setsup memory for index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to free
|   - sizeUL:
|     o new size of arrays
| Output:
|   - Modifies:
|     o arrays in index_clustST to have memory allocated
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
\-------------------------------------------------------*/
signed char
setup_index_clustST(
   struct index_clustST *indexSTPtr,
   unsigned long sizeUL
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   o fun10 sec01:
   '     - check if need to set default array size
   '   o fun10 sec02:
   '     - scoring and read mapping arrays
   '   o fun10 sec03:
   '     - clustering assignment array
   '   o fun10 sec04:
   '     - file coordinates arrays
   '   o fun10 sec05:
   '     - reference id array
   '   o fun10 sec06:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - check if need to set default array size
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(sizeUL == 0)
      sizeUL = 4096;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - scoring and read mapping arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexSTPtr->lenUL = sizeUL;

   if(indexSTPtr->scoreArySC)
      free(indexSTPtr->scoreArySC);
   indexSTPtr->scoreArySC = 0;

   indexSTPtr->scoreArySC =
      malloc(
         sizeUL *
         sizeof(signed char)
      );

   if(! indexSTPtr->scoreArySC)
      goto memErr_fun10_sec06;


   if(indexSTPtr->startAryUI)
      free(indexSTPtr->startAryUI);
   indexSTPtr->startAryUI = 0;

   indexSTPtr->startAryUI =
      malloc(
         sizeUL *
         sizeof(unsigned int)
      );

   if(! indexSTPtr->startAryUI)
      goto memErr_fun10_sec06;


   if(indexSTPtr->refAryUI)
      free(indexSTPtr->refAryUI);
   indexSTPtr->refAryUI = 0;

   indexSTPtr->refAryUI =
      malloc(
         sizeUL *
         sizeof(unsigned int)
      );

   if(! indexSTPtr->refAryUI)
      goto memErr_fun10_sec06;


   if(indexSTPtr->endAryUI)
      free(indexSTPtr->endAryUI);
   indexSTPtr->endAryUI = 0;

   indexSTPtr->endAryUI =
      malloc(
         sizeUL *
         sizeof(unsigned int)
      );

   if(! indexSTPtr->endAryUI)
      goto memErr_fun10_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - clustering assignment array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSTPtr->clustArySI)
      free(indexSTPtr->clustArySI);
   indexSTPtr->clustArySI = 0;

   indexSTPtr->clustArySI =
      malloc(
         sizeUL *
         sizeof(unsigned int)
      );

   if(! indexSTPtr->clustArySI)
      goto memErr_fun10_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - file coordinates arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSTPtr->indexAryUL)
      free(indexSTPtr->indexAryUL);
   indexSTPtr->indexAryUL = 0;

   indexSTPtr->indexAryUL =
      malloc(
         sizeUL *
         sizeof(unsigned long)
      );

   if(! indexSTPtr->indexAryUL)
      goto memErr_fun10_sec06;


   if(indexSTPtr->lenLineAryUI)
      free(indexSTPtr->lenLineAryUI);
   indexSTPtr->lenLineAryUI = 0;

   indexSTPtr->lenLineAryUI =
      malloc(
         sizeUL *
         sizeof(unsigned int)
      );

   if(! indexSTPtr->lenLineAryUI)
      goto memErr_fun10_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec05:
   ^   - reference id array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSTPtr->refIdAryStr)
      free(indexSTPtr->refIdAryStr);
   indexSTPtr->refIdAryStr = 0;

   indexSTPtr->refIdAryStr = mk_strAry(16);

   if(! indexSTPtr->refIdAryStr)
      goto memErr_fun10_sec06;


   if(indexSTPtr->refNumAryUI)
      free(indexSTPtr->refNumAryUI);
   indexSTPtr->refNumAryUI = 0;

   indexSTPtr->refNumAryUI =
      malloc(16 * sizeof(unsigned int));

   if(! indexSTPtr->refNumAryUI)
      goto memErr_fun10_sec06;


   indexSTPtr->lenRefUI = 16;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec06:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun10_sec06:;
   return def_memErr_clustST;
} /*setup_index_clustST*/

/*-------------------------------------------------------\
| Fun11: realloc_index_clustST
|   - reallocates memory for an index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to assign more
|       memory to
|   - sizeUL:
|     o new size of arrays; use indexSTPtr->lenUL to not
|       resize arrays (except refIdAryStr)
|   - numRefUI:
|     o new number of references; use indexSTPtr->lenRefUI
|       to not resize reference array
| Output:
|   - Modifies:
|     o arrays in index_clustST to be size of sizeUL
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
\-------------------------------------------------------*/
signed char
realloc_index_clustST(
   struct index_clustST *indexSTPtr,
   unsigned long sizeUL,
   unsigned int numRefUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - check if need to resize + resize read stats
   '   o fun11 sec03:
   '     - resize cluster assignment array
   '   o fun11 sec04:
   '     - resize file coordinate array
   '   o fun11 sec05:
   '     - check if need to resize reference id array
   '   o fun11 sec06:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int *tmpUIPtr = 0;
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - check if need to resize + resize read stats
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(sizeUL != indexSTPtr->lenUL)
   { /*If: need to resize arrays*/
      tmpUIPtr =
         (unsigned int *)
         realloc(
            indexSTPtr->scoreArySC,
            sizeUL * sizeof(signed char)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->scoreArySC = (signed char *) tmpUIPtr;



      tmpUIPtr =
         realloc(
            indexSTPtr->startAryUI,
            sizeUL * sizeof(unsigned int)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->startAryUI = tmpUIPtr;


      tmpUIPtr =
         realloc(
            indexSTPtr->endAryUI,
            sizeUL * sizeof(unsigned int)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->endAryUI = tmpUIPtr;


      tmpUIPtr =
         realloc(
            indexSTPtr->refAryUI,
            sizeUL * sizeof(unsigned int)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->refAryUI = tmpUIPtr;

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun11 Sec03:
      ^   - resize cluster assignment array
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      tmpUIPtr =
         (unsigned int *)
         realloc(
            indexSTPtr->clustArySI,
            sizeUL * sizeof(signed int)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->clustArySI = (signed int *) tmpUIPtr;


      indexSTPtr->lenUL = sizeUL;

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun11 Sec04:
      ^   - resize file coordinate array
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      tmpUIPtr =
         (unsigned int *)
         realloc(
            indexSTPtr->indexAryUL,
            sizeUL * sizeof(unsigned long)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->indexAryUL = (unsigned long *) tmpUIPtr;


      tmpUIPtr =
         realloc(
            indexSTPtr->lenLineAryUI,
            sizeUL * sizeof(unsigned int)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->lenLineAryUI = tmpUIPtr;

   } /*If: need to resize arrays*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec05:
   ^   - check if need to resize reference id array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numRefUI != indexSTPtr->lenRefUI)
   { /*If: resizing reference array*/
      tmpStr =
         realloc_strAry(
            indexSTPtr->refIdAryStr,
            numRefUI
         );

      if(! tmpStr)
         goto memErr_fun11_sec06;

      indexSTPtr->refIdAryStr = tmpStr;


      tmpUIPtr =
         realloc(
            indexSTPtr->refNumAryUI,
            numRefUI * sizeof(unsigned int)
         );

      if(! tmpUIPtr)
         goto memErr_fun11_sec06;

      indexSTPtr->refNumAryUI = tmpUIPtr;

      indexSTPtr->lenRefUI = numRefUI;
   } /*If: resizing reference array*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec06:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun11_sec06:;
   return def_memErr_clustST;
} /*realloc_index_clustST*/

/*-------------------------------------------------------\
| Fun12: blank_con_clustST
|   - blank an clustST con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to blank
| Output:
|   - Modifies:
|     o conSTPtr to be blank 
\-------------------------------------------------------*/
void
blank_con_clustST(
   struct con_clustST *conSTPtr
){
   if(conSTPtr->samSTPtr)
      blank_samEntry(conSTPtr->samSTPtr);

   conSTPtr->clustSI = 0;
   conSTPtr->numReadsUL = 0;
   conSTPtr->maxReadsUL = 0;
   conSTPtr->numVarUI = 0;
   conSTPtr->startUI = 0;
   conSTPtr->endUI = 0;
} /*blank_con_clustST*/

/*-------------------------------------------------------\
| Fun13: init_con_clustST
|   - initialzie an clustST con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to initialize
| Output:
|   - Modifies:
|     o conSTPtr to intialize
\-------------------------------------------------------*/
void
init_con_clustST(
   struct con_clustST *conSTPtr
){
   conSTPtr->samSTPtr = 0;
   conSTPtr->nextST = 0;

   conSTPtr->typeArySC = 0;
   conSTPtr->posAryUI = 0;
   conSTPtr->lenProfUI = 0;

   blank_con_clustST(conSTPtr);
} /*init_con_clustST*/

/*-------------------------------------------------------\
| Fun14: freeStack_con_clustST
|   - frees variables inside an con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to free varaibles
| Output:
|   - Frees:
|     o samSTPtr in conSTPtr (nextST not freeded)
|   - Modifies:
|     o blanks conSTPtr
\-------------------------------------------------------*/
void
freeStack_con_clustST(
   struct con_clustST *conSTPtr
){
   if(conSTPtr)
   { /*If: have a struct*/
      if(conSTPtr->samSTPtr)
         freeHeap_samEntry(conSTPtr->samSTPtr);

      if(conSTPtr->typeArySC)
         free(conSTPtr->typeArySC);

      if(conSTPtr->posAryUI)
         free(conSTPtr->posAryUI);

      init_con_clustST(conSTPtr);
   } /*If: have a struct*/
} /*freeStack_con_clustST*/

/*-------------------------------------------------------\
| Fun15: freeHeap_con_clustST
|   - frees a con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to free
| Output:
|   - Frees:
|     o conSTPtr
|   - Returns:
|     o next con_clustST struct in list
\-------------------------------------------------------*/
con_clustST *
freeHeap_con_clustST(
   struct con_clustST *conSTPtr
){
   struct con_clustST *nextSTPtr = 0;

   if(conSTPtr)
   { /*If: have a struct*/
      nextSTPtr = conSTPtr->nextST;

      freeStack_con_clustST(conSTPtr);
      free(conSTPtr);
   } /*If: have a struct*/

   return nextSTPtr;
} /*freeHeap_con_clustST*/

/*-------------------------------------------------------\
| Fun16: freeHeapList_con_clustST
|   - frees a list of heap allocated con_clustST structs
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct list to free
| Output:
|   - Frees:
|     o conSTPtr and all other structs in the list
\-------------------------------------------------------*/
void
freeHeapList_con_clustST(
   struct con_clustST *conSTPtr
){
   while(conSTPtr)
      conSTPtr = freeHeap_con_clustST(conSTPtr);
} /*freeHeapList_con_clustST*/

/*-------------------------------------------------------\
| Fun17: mk_con_clustST
|   - makes a con_clustST struct on the heap
| Input:
|   - samSTPtr:
|     o pointer to sam entry struct to store (0 for none)
|     o only a shallow copy is done (pointer saved only)
|   - clustSI:
|     o cluster number to assign
|   - numReadsUL:
|     o number of reads in cluster
| Output:
|   - Returns:
|     o pointer con_clustST struct with input
|     o 0 for memory error
\-------------------------------------------------------*/
con_clustST *
mk_con_clustST(
   struct samEntry *samSTPtr, /*consensus to add*/
   signed int clustSI,        /*cluster number*/
   unsigned long numReadsUL   /*number reads in cluster*/
){
   struct con_clustST *retSTPtr = 0;

   retSTPtr = malloc(sizeof(struct con_clustST));

   if(! retSTPtr)
      goto memErr_fun17_sec0x;

   init_con_clustST(retSTPtr);

   retSTPtr->samSTPtr = samSTPtr;
   retSTPtr->clustSI = clustSI;
   retSTPtr->numReadsUL = numReadsUL;

   return retSTPtr;

   memErr_fun17_sec0x:;
   if(retSTPtr)
      freeHeap_con_clustST(retSTPtr);
   retSTPtr = 0;
   return 0;
} /*mk_con_clustST*/

/*-------------------------------------------------------\
| Fun18: swap_con_clustST
|   - swaps to con_clustST structs (except nextST ptr)
| Input:
|   - firstSTPtr:
|     o first struct to swap
|   - secSTPtr:
|     o second struct to swap
| Output:
|   - Modifies:
|     o firstSTPtr to have secSTPtr variables
|     o secSTPtr to have frstSTPtr variables
|   - Note: does not swap nextST pointer
\-------------------------------------------------------*/
void
swap_con_clustST(
   struct con_clustST *firstSTPtr,
   struct con_clustST *secSTPtr
){
   struct samEntry *samSwapSTPtr = 0;
   signed char *swapSCPtr = 0;
   
   samSwapSTPtr = firstSTPtr->samSTPtr;
   firstSTPtr->samSTPtr = secSTPtr->samSTPtr;
   secSTPtr->samSTPtr = samSwapSTPtr;

   firstSTPtr->clustSI ^= secSTPtr->clustSI;
   secSTPtr->clustSI ^= firstSTPtr->clustSI;
   firstSTPtr->clustSI ^= secSTPtr->clustSI;

   firstSTPtr->numReadsUL ^= secSTPtr->numReadsUL;
   secSTPtr->numReadsUL ^= firstSTPtr->numReadsUL;
   firstSTPtr->numReadsUL ^= secSTPtr->numReadsUL;

   firstSTPtr->startUI ^= secSTPtr->startUI;
   secSTPtr->startUI ^= firstSTPtr->startUI;
   firstSTPtr->startUI ^= secSTPtr->startUI;

   firstSTPtr->endUI ^= secSTPtr->endUI;
   secSTPtr->endUI ^= firstSTPtr->endUI;
   firstSTPtr->endUI ^= secSTPtr->endUI;

   /*swap the profile*/

   swapSCPtr = firstSTPtr->typeArySC;
   firstSTPtr->typeArySC = secSTPtr->typeArySC;
   secSTPtr->typeArySC = swapSCPtr;

   swapSCPtr = (signed char *) firstSTPtr->posAryUI;
   firstSTPtr->posAryUI = secSTPtr->posAryUI;
   secSTPtr->posAryUI = (unsigned int *) swapSCPtr;

   firstSTPtr->numVarUI ^= secSTPtr->numVarUI;
   secSTPtr->numVarUI ^= firstSTPtr->numVarUI;
   firstSTPtr->numVarUI ^= secSTPtr->numVarUI;

   firstSTPtr->lenProfUI ^= secSTPtr->lenProfUI;
   secSTPtr->lenProfUI ^= firstSTPtr->lenProfUI;
   firstSTPtr->lenProfUI ^= secSTPtr->lenProfUI;
} /*swap_con_clustST*/

/*-------------------------------------------------------\
| Fun20: mk_index_clustST
|   - makes an index_clustST struct for a sam file
| Input:
|   - sizeUL:
|     o size to make arrays at start
|     o any value > 0 will do since arrays are resized
|       when needed, however, for best memory usage, input
|       number of lines in sam file
|   - clustSetSTPtr:
|     o settings for filtering reads
|   - samSTPtr:
|     o pointer to sam struct to use in read hunt
|   - buffStrPtr:
|     o pointer to c-string to use reading sam file
|   - lenBuffULPtr:
|     o pointer to unsigned long with size of buffStrPtr
|   - samFILE:
|     o sam file to convert to stats
| Output:
|   - Modifies:
|     o samSTPtr to hold last entry
|     o buffStrPtr to be larger if needed
|     o lenBuffULPtr to be new buffStrPtr size (if resized)
|     o samFILE to point to start
|   - Returns:
|     o 0 for memory error
|     o pointer to index_clustST struct with scores and
|       index's (index_clustST->keptSL is 0 if no reads)
|       - in clustArySI:
|         i. open reads set to 0
|         ii. headers set to def_header_clustST
|         iii. discarded reads set to def_discard_clustST
\-------------------------------------------------------*/
struct index_clustST *
mk_index_clustST(
   unsigned long sizeUL,
   struct set_clustST *clustSetSTPtr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   signed long *lenBuffSLPtr,
   void *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun20 TOC:
   '   - get read scores for a sam file
   '   o fun20 sec01:
   '     - variable declarations
   '   o fun20 sec02:
   '     - setup; move to file start and memory allocation
   '   o fun20 sec03:
   '     - get scores
   '   o fun20 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned long lineUL = 0;
   signed char errSC = 0;
   unsigned char scoreUC = 0;  /*used in scoring step*/
   unsigned int lenUI = 0;/*used in scoring (log10) step*/
   signed long lenLineSL = 0;

   signed long filePosSL = 0; /*position at in file*/

   signed long indexSL = 0;       /*find reads reference*/
   signed long slCnt = 0;         /*shifting indexs*/
   signed long tmpSL = 0;

   struct index_clustST *retHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec02:
   ^   - setup (move to file start and memory allocation)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fseek(samFILE, 0, SEEK_SET);
      /*make sure at start of file*/

   /*allocate memory for scores, index's, and clusters*/
   retHeapST = malloc(sizeof(struct index_clustST));
   if(! retHeapST)
      goto memErr_fun20_sec04;
   
   init_index_clustST(retHeapST);

   if(sizeUL == 0)
      sizeUL = 4096;
   errSC = setup_index_clustST(retHeapST, sizeUL);
   if(errSC)
      goto memErr_fun20_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec03:
   ^   - get scores
   ^   o fun20 sec03 sub01:
   ^     - get first line of sam file
   ^   o fun20 sec03 sub02:
   ^     - start loop & handel memory resizing (if needed)
   ^   o fun20 sec03 sub03:
   ^     - detect entires to discard
   ^   o fun20 sec03 sub04:
   ^     - score and index reads
   ^   o fun20 sec03 sub05:
   ^     - get next entry
   ^   o fun20 sec03 sub06:
   ^     - final error check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun20 Sec03 Sub01:
   *   - get first line of sam file
   \*****************************************************/

   tmpSL =
      getFullLine_fileFun(
         samFILE,
         buffStrPtr,
         (signed long *) lenBuffSLPtr,
         &lenLineSL, /*gets length of buffer*/
         0
      );

   if(! tmpSL)
      goto noReads_fun20_sec04;
   else if(tmpSL < 0)
      goto memErr_fun20_sec04;


   errSC = lineTo_samEntry(samSTPtr, *buffStrPtr);
   if(errSC)
      goto memErr_fun20_sec04;

   /*****************************************************\
   * Fun20 Sec03 Sub02:
   *   - start loop and handel memory resizing (if needed)
   \*****************************************************/

   while(! errSC)
   { /*Loop: print out stats*/
      if(retHeapST->lenSamUL >= retHeapST->lenUL)
      { /*If: need to get more memory*/
         errSC =
            realloc_index_clustST(
               retHeapST,
               retHeapST->lenUL << 1,
               retHeapST->lenRefUI    /*not resized*/
            );

         if(errSC)
            goto memErr_fun20_sec04;
      } /*If: need to get more memory*/

      /**************************************************\
      * Fun20 Sec03 Sub03:
      *   - detect entires to discard
      \**************************************************/

      if(samSTPtr->extraStr[0] == '@')
      { /*If: comment*/
         retHeapST->scoreArySC[lineUL] =
            def_header_clustST;

         retHeapST->clustArySI[lineUL] =
            def_header_clustST;

         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;

         retHeapST->startAryUI[lineUL] = 0;
         retHeapST->endAryUI[lineUL] = 0;
         retHeapST->refAryUI[lineUL] = 0;
      } /*If: comment*/

      else if(samSTPtr->flagUS & (4 | 256 | 2048))
      { /*Else If: unmapped, secondary, or supplemental*/
         retHeapST->scoreArySC[lineUL] =
            def_discard_clustST;

         retHeapST->clustArySI[lineUL] =
            def_discard_clustST;

         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;

         retHeapST->startAryUI[lineUL] = 0;
         retHeapST->endAryUI[lineUL] = 0;
         retHeapST->refAryUI[lineUL] = 0;
      } /*Else If: unmapped, secondary, or supplemental*/

      else if(samSTPtr->mapqUC < clustSetSTPtr->minMapqUC)
      { /*Else If: mapping quality to low*/
         retHeapST->scoreArySC[lineUL] =
            def_discard_clustST;

         retHeapST->clustArySI[lineUL] =
            def_discard_clustST;

         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;

         retHeapST->startAryUI[lineUL] = 0;
         retHeapST->endAryUI[lineUL] = 0;
         retHeapST->refAryUI[lineUL] = 0;
      } /*Else If: mapping quality to low*/

      else if(
         samSTPtr->alnReadLenUI < clustSetSTPtr->minLenUI
      ){ /*Else If: aligned length to short*/
         retHeapST->scoreArySC[lineUL] =
            def_discard_clustST;

         retHeapST->clustArySI[lineUL] =
            def_discard_clustST;

         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;

         retHeapST->startAryUI[lineUL] = 0;
         retHeapST->endAryUI[lineUL] = 0;
         retHeapST->refAryUI[lineUL] = 0;
      } /*Else If: aligned length to short*/

      else if(
         samSTPtr->medianQF < clustSetSTPtr->minMedQUI
      ){ /*Else If: median q-score to low*/
         retHeapST->scoreArySC[lineUL] =
            def_discard_clustST;

         retHeapST->clustArySI[lineUL] =
            def_discard_clustST;

         retHeapST->refAryUI[lineUL] =
            def_discard_clustST;

         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;

         retHeapST->startAryUI[lineUL] = 0;
         retHeapST->endAryUI[lineUL] = 0;
         retHeapST->refAryUI[lineUL] = 0;
      } /*Else If: median q-score to low*/

      else if(samSTPtr->meanQF < clustSetSTPtr->minAvgQUI)
      { /*Else If: mean q-score to low*/
         retHeapST->scoreArySC[lineUL] =
            def_discard_clustST;

         retHeapST->clustArySI[lineUL] =
            def_discard_clustST;

         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;

         retHeapST->startAryUI[lineUL] = 0;
         retHeapST->endAryUI[lineUL] = 0;
         retHeapST->refAryUI[lineUL] = 0;
      } /*Else If: mean q-score to low*/

      /**************************************************\
      * Fun20 Sec03 Sub04:
      *   - score and index reads
      *   o fun20 sec03 sub04 cat01:
      *     - assign ref number; check if new ref
      *   o fun20 sec03 sub04 cat02:
      *     - assign ref number; add new ref
      *   o fun20 sec03 sub04 cat03:
      *     - get/assign score and index
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun20 Sec03 Sub04 Cat01:
      +   - assign ref number; check if new ref
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: keep read*/
         ++retHeapST->keptSL;

         if(retHeapST->numRefUI == 0)
         { /*If: first reference*/
            add_strAry(
               samSTPtr->refIdStr,
               retHeapST->refIdAryStr,
               0
            );

            retHeapST->refNumAryUI[0] = 1;
            retHeapST->numRefUI = 1;
            retHeapST->refAryUI[lineUL] = 1;
         } /*If: first reference*/

         else
         { /*Else: get reference*/
            indexSL =
               find_strAry(
                  retHeapST->refIdAryStr,
                  samSTPtr->refIdStr,
                  retHeapST->numRefUI
               );

            if(indexSL >= 0)
               retHeapST->refAryUI[lineUL] = 
                  retHeapST->refNumAryUI[indexSL];
                  /*assigned reference number*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun20 Sec03 Sub04 Cat02:
            +   - assign ref number; add new ref
            \+++++++++++++++++++++++++++++++++++++++++++*/

            else
            { /*Else: is a new reference*/
               if(
                     retHeapST->numRefUI + 1
                  >= retHeapST->lenRefUI
               ){ /*If: need more memory*/

                  errSC =
                     realloc_index_clustST(
                        retHeapST,
                        retHeapST->lenUL, /*not resizing*/
                        (retHeapST->numRefUI + 1) << 1
                     );
                     /*we have two arrays, the reference
                     `   id array and the read array, we
                     `   only need to resize reference id
                     */

                  if(errSC)
                     goto memErr_fun20_sec04;
               } /*If: need more memory*/

               indexSL =
                  (signed long)
                  addSort_strAry(
                     samSTPtr->refIdStr,
                     retHeapST->refIdAryStr,
                     retHeapST->numRefUI
                  );

               /*need to account for inserted index*/
               for(
                  slCnt = (signed long)
                          retHeapST->numRefUI;
                  slCnt > indexSL;
                  --slCnt
               ) retHeapST->refNumAryUI[slCnt] =
                    retHeapST->refNumAryUI[slCnt - 1];

               ++retHeapST->numRefUI;

               retHeapST->refNumAryUI[indexSL] =
                  retHeapST->numRefUI;

               retHeapST->refAryUI[lineUL] =
                  retHeapST->numRefUI;
            } /*Else: is a new reference*/
         } /*Else: get reference*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun20 Sec03 Sub04 Cat03:
         +   - get/assign score and index
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         lenUI = samSTPtr->alnReadLenUI;

         scoreUC = 0;

         while(lenUI > 0)
         { /*Loop: log10(length)*/
            ++scoreUC;
            lenUI /= 10;
         } /*Loop: log10(length)*/

         scoreUC *= clustSetSTPtr->lenWeightF;

         retHeapST->scoreArySC[lineUL] = 
            scoreUC + (unsigned char) samSTPtr->medianQF;

         retHeapST->startAryUI[lineUL] =
            samSTPtr->refStartUI;

         retHeapST->endAryUI[lineUL] =
            samSTPtr->refEndUI;

         retHeapST->clustArySI[lineUL] = 0;
         retHeapST->indexAryUL[lineUL] =
            (unsigned long) filePosSL;
      } /*Else: keep read*/
 
      /**************************************************\
      * Fun20 Sec03 Sub05:
      *   - get next entry
      \**************************************************/

      retHeapST->lenLineAryUI[lineUL] = lenLineSL;
      filePosSL += retHeapST->lenLineAryUI[lineUL];

      tmpSL =
         getFullLine_fileFun(
            samFILE,
            buffStrPtr,
            lenBuffSLPtr,
            &lenLineSL, /*gets length of buffer*/
            0
         );

      if(! tmpSL)
        break; /*at end of file*/
      else if(tmpSL < 0)
         goto memErr_fun20_sec04;

      errSC = lineTo_samEntry(samSTPtr, *buffStrPtr);
      if(errSC)
         goto memErr_fun20_sec04;

      ++lineUL;
   } /*Loop: print out stats*/

   /*****************************************************\
   * Fun20 Sec03 Sub06:
   *   - final error check
   \*****************************************************/

   retHeapST->lenSamUL = lineUL;

   if(errSC == def_memErr_samEntry)
      goto memErr_fun20_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun20_sec04;

   memErr_fun20_sec04:;
      if(retHeapST)
         freeHeap_index_clustST(retHeapST);
      retHeapST = 0;
      goto ret_fun20_sec04;

   noReads_fun20_sec04:;
      retHeapST->keptSL = 0;
      goto ret_fun20_sec04;

   ret_fun20_sec04:;
      fseek(samFILE, 0, SEEK_SET);
      return retHeapST;
} /*mk_index_clustST*/

/*-------------------------------------------------------\
| Fun21: getRead_clustST
|   - gets a read by index from a sam file
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct to hold read
|   - lineUL:
|     o line number of read in sam file
|   - offsetUL:
|     o number of bytes from current position to read
|   - indexSTPtr:
|     o pointer to index_clustST struct with line lengths
|   - buffStr:
|     o c-string to read in line with
|     o should be length of target reads line + 2
|   - samFILE:
|     o FILE pointer to sam file to get read from
| Output:
|   - Modifies:
|     o samSTPtr to have new read
|     o buffStr to have reads line
|     o samFILE to be on read after current read
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
|     o def_fileErr_clustST for file errors
\-------------------------------------------------------*/
signed char
getRead_clustST(
   struct samEntry *samSTPtr,       /*will have read*/
   unsigned long lineUL,            /*line of read*/
   unsigned long offsetUL,          /*offset to read*/
   struct index_clustST *indexSTPtr,/*index's of reads*/
   signed char *buffStr,            /*for getting line*/
   void *samFILE                    /*file with read*/
){
   signed char errSC = 0;

   if(offsetUL)
   { /*If: skipping entries*/
      fseek(samFILE, offsetUL, SEEK_CUR);
      offsetUL = 0;
   } /*If: skipping entries*/

   offsetUL =
      fread(
         buffStr,
         sizeof(signed char),
         indexSTPtr->lenLineAryUI[lineUL],
         samFILE
      );

   if(! offsetUL)
      return def_fileErr_clustST;

   buffStr[offsetUL] = '\0';

   errSC = lineTo_samEntry( samSTPtr, buffStr);

   if(errSC)
      return def_memErr_clustST;

   return 0;
} /*getRead_clustST*/

/*-------------------------------------------------------\
| Fun22: getCon_clustST
|   - builds a consensus for a cluster
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct with cluster array
|       to use
|     o clusters assigned with fun03 (0 is no cluster)
|   - clustSI:
|     o cluster number to build consensus for
|   - conSetSTPtr:
|     o pointer to set_tbCon struct with setting for
|       consensus building
|   - clustSetSTPtr:
|     o has settings for filtering and stores the first
|       and last possible base of the consensesus
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - buffStr:
|     o c-string to use reading sam file
|   - samFILE:
|     o sam file with reads to add to consensus
|   - errSCPtr:
|     o pointer to signed char to hold error type
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_clustST for any other error
|       - def_noCon_clustST if consensus had to many N's
|         or could not be built
|   - Returns:
|     o 0 if could not build consenensus or had error
|     o samEntry strucure array with consensus fragments
\-------------------------------------------------------*/
struct samEntry *
getCon_clustST(
   struct index_clustST *indexSTPtr,
   signed int clustSI,
   struct set_clustST *clustSetSTPtr,
   struct set_tbCon *conSetSTPtr,
   struct samEntry *samSTPtr,
   signed char *buffStr,
   void *samFILE,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun22 TOC:
   '   - finds number of reads mapping to best read
   '   o fun22 sec01:
   '     - variable declarations
   '   o fun22 sec02:
   '     - get first entry in sam file
   '   o fun22 sec03:
   '     - build consensus
   '   o fun22 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed long lineSL = 0;

   struct samEntry *samHeapST = 0;
   struct conNt_tbCon *conHeapAryST = 0;
   unsigned int lenConUI = 0;
   unsigned int numMaskUI = 0;
      /*number of masked bases in con*/
   float percF = 0;

   unsigned long offsetUL = 0;
      /*offset to get target read*/

   signed char refIdStr[64];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec02:
   ^   - get first entry in sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   refIdStr[0] = '\0';

   lenConUI =
      clustSetSTPtr->endUI - clustSetSTPtr->startUI;

   ++lenConUI;
   lenConUI += 128; /*allow for some offset*/

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec03:
   ^   - build consensus
   ^   o fun22 sec03 sub01:
   ^     - filter reads + start loop
   ^   o fun22 sec03 sub02:
   ^     - get read
   ^   o fun22 sec03 sub03:
   ^     - add reads to consensus
   ^   o fun22 sec03 sub04:
   ^     - collapse consensus and check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun22 Sec03 Sub01:
   *   - filter reads + start loop
   \*****************************************************/

   for(
      lineSL = 0;
      lineSL < (signed long) indexSTPtr->lenSamUL;
      ++lineSL
   ){ /*Loop: add reads to consensus*/

      if(indexSTPtr->clustArySI[lineSL] != clustSI)
      { /*If: read not in cluster*/
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*If: read not in cluster*/

      /**************************************************\
      * Fun22 Sec03 Sub02:
      *   - get read
      \**************************************************/

      errSC =
         getRead_clustST(
            samSTPtr,
            (unsigned long) lineSL,
            offsetUL,
            indexSTPtr,
            buffStr,
            samFILE
         ); /*get read from sam file*/

      offsetUL = 0;

      if(errSC)
         goto err_fun22_sec04;

      /**************************************************\
      * Fun22 Sec03 Sub03:
      *   - add reads to conensus
      \**************************************************/

      if(refIdStr[0] == '\0')
      { /*If: have not recorded reference name*/
         cpLen_ulCp(
            refIdStr,
            samSTPtr->refIdStr,
            samSTPtr->refIdLenUC
         );
      } /*If: have not recorded reference name*/

      /*adjust to local coordiantes*/

      numMaskUI =
         min_genMath(
            samSTPtr->refStartUI,
            clustSetSTPtr->startUI
         );

      samSTPtr->refStartUI -= numMaskUI;
      samSTPtr->refEndUI -= numMaskUI;

      /*add read to consensus*/

      errSC =
         addRead_tbCon(
            samSTPtr,
            &conHeapAryST,
            &lenConUI,
            conSetSTPtr
         );

      if(errSC == def_memErr_tbConDefs)
         goto err_fun22_sec04;
   } /*Loop: add reads to consensus*/

   /*****************************************************\
   * Fun22 Sec03 Sub04:
   *   - collapse consensus and check for errors
   \*****************************************************/

   samHeapST =
      noFragCollapse_tbCon(
         conHeapAryST,
         lenConUI,
         refIdStr,
         &numMaskUI,
         conSetSTPtr,
         &errSC
      );

   if(errSC == def_memErr_tbConDefs)
      goto err_fun22_sec04;

   if(! samHeapST)
      goto noCon_fun22_sec04;

   percF = (float) numMaskUI;
   percF /= (float) samHeapST->readLenUI;

   if(percF > clustSetSTPtr->maxNPercF)
      goto noCon_fun22_sec04;

   /*set up global coordinates*/
   samHeapST->refStartUI += clustSetSTPtr->startUI;
   samHeapST->refEndUI += clustSetSTPtr->startUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun22_sec04;


   noCon_fun22_sec04:;
   *errSCPtr = def_noCon_clustST;

   if(samHeapST)
      freeHeap_samEntry(samHeapST);
   samHeapST = 0;
   goto ret_fun22_sec04;


   err_fun22_sec04:;
   *errSCPtr = def_memErr_clustST;

   if(samHeapST)
      freeHeap_samEntry(samHeapST);
   samHeapST = 0;

   goto ret_fun22_sec04;



   ret_fun22_sec04:;

   if(conHeapAryST)
      freeHeapAry_conNt_tbCon(
         conHeapAryST,
         lenConUI
      );

   lenConUI = 0;
   conHeapAryST = 0;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   return samHeapST;
} /*getCon_clustST*/

/*-------------------------------------------------------\
| Fun23: cmpCons_clustST
|   - compares two consensus clusters to see if same
| Input:
|   - firstConSTPtr:
|     o pointer to con_clustST struct with first consensus
|       to compare
|   - secConSTPtr:
|     o pointer to con_clustST struct with second
|       consensus to compare
|   - noBoundCheckBl:
|     o 1: skip bounds check (one read overlaps other)
|     o 0: do a bounds check
|   - indexSTPtr:
|     o pointer to index_clustST with clusters
|   - clustSetSTPtr:
|     o pointer to set_clustST struct with settings
|   - edDistResSTPtr:
|     o pointer to edDistResSTPtr struct to hold results
|       from edDist
| Output:
|   - Modifies:
|     o clustArySI in indexSTPtr to have clusters merged
|       if consensus are to similar (lowest cluster
|       assigned)
|     o merges second cluster into first cluster if
|       clusters were to similar
|       - you must free the second cluster (always has
|         discarded cluster)
|   - Returns:
|     o 0 if clusters are different
|     o 1 if merged clusters
\-------------------------------------------------------*/
signed char
cmpCons_clustST(
   struct con_clustST *firstConSTPtr,
   struct con_clustST *secConSTPtr,
   signed char noBoundCheckBl, /*1: no bounds check*/
   struct index_clustST *indexSTPtr,
   struct set_clustST *clustSetSTPtr,
   struct res_edDist *edDistResSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun23: cmpCons_clustST
   '   - compares two consensus clusters to see if same
   '   o fun23 sec01:
   '     - variable declarations   
   '   o fun23 sec02:
   '     - get edit distance and filter distance cons out
   '   o fun23 sec03:
   '     - merge similar consensus
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long distSL = 0;

   signed char retSC = 0;

   /*changing cluster numbers*/
   signed int keepClustSI = 0;
   signed int changeClustSI = 0;

   float distPercF = 0;
   unsigned int *tmpUIPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec02:
   ^   - get edit distance and filter distance cons out
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*disable depth profiling for consensuses*/
   tmpUIPtr = edDistResSTPtr->depthAryUI;
   edDistResSTPtr->depthAryUI = 0;

   if(
      eqlNull_ulCp(
         firstConSTPtr->samSTPtr->refIdStr,
         secConSTPtr->samSTPtr->refIdStr
      )
   ) return 0; /*different references used*/

   if(! noBoundCheckBl)
   { /*If: not doing bounds check*/
      if(
              firstConSTPtr->samSTPtr->refStartUI 
            < secConSTPtr->samSTPtr->refStartUI
         &&
              firstConSTPtr->samSTPtr->refEndUI 
            < secConSTPtr->samSTPtr->refEndUI
      ) return 0; /*one consensus does not cover other*/

      if(
              firstConSTPtr->samSTPtr->refStartUI 
            > secConSTPtr->samSTPtr->refStartUI
         &&
              firstConSTPtr->samSTPtr->refEndUI 
            > secConSTPtr->samSTPtr->refEndUI
      ) return 0; /*one consensus does not cover other*/
   } /*If: not doing bounds check*/
   
   distSL =
      readCmpDist_edDist(
         firstConSTPtr->samSTPtr,
         secConSTPtr->samSTPtr,
         clustSetSTPtr->indelLenUI,
         clustSetSTPtr->minSnpQUC,
         -1,              /*will check overlaps later*/
         0,               /*not doing depth profile*/
         clustSetSTPtr->winSizeUI,
         edDistResSTPtr
      ); /*get edit distance*/

   edDistResSTPtr->depthAryUI = tmpUIPtr;

   distPercF = (float) distSL;
   distPercF /= (float) edDistResSTPtr->overlapUI;

   if(1 - distPercF <= clustSetSTPtr->maxConSimF)
      return 0; /*to much difference*/

   distSL =
      percDist_edDist(
        edDistResSTPtr,
        clustSetSTPtr->winSizeUI,
        clustSetSTPtr->conErrRateF
      );

   if(
           edDistResSTPtr->overlapUI
        >= clustSetSTPtr->winSizeUI
     &&
           edDistResSTPtr->probMaxWinUI
        > clustSetSTPtr->winErrUI
   ) return 0; /*window scan found difference region*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec03:
   ^   - merge similar consensus
   ^   o fun23 sec03 sub01:
   ^     - find lowest cluster number
   ^   o fun23 sec03 sub02:
   ^     - find consensus to return as keep
   ^   o fun23 sec03 sub03:
   ^     - merge clusters and return keep consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun23 Sec03 Sub01:
   *   - find lowest cluster number
   \*****************************************************/

   if(
        firstConSTPtr->clustSI
      < secConSTPtr->clustSI
   ){ /*If: first cluster has lowest number*/
      keepClustSI = firstConSTPtr->clustSI;
      changeClustSI = secConSTPtr->clustSI;

      secConSTPtr->clustSI = firstConSTPtr->clustSI;
   } /*If: first cluster has lowest number*/

   else
   { /*Else: second cluster has lowest number*/
      keepClustSI = secConSTPtr->clustSI;
      changeClustSI = firstConSTPtr->clustSI;

      firstConSTPtr->clustSI = secConSTPtr->clustSI;
   } /*Else: second cluster has lowest number*/

   /*****************************************************\
   * Fun23 Sec03 Sub02:
   *   - find consensus to return as keep
   \*****************************************************/

   firstConSTPtr->numReadsUL += secConSTPtr->numReadsUL;
   secConSTPtr->numReadsUL = firstConSTPtr->numReadsUL;

   if(
        firstConSTPtr->samSTPtr->alnReadLenUI
      > secConSTPtr->samSTPtr->alnReadLenUI
   ) retSC = 1; /*fist cluster is longer*/

   else if(
        firstConSTPtr->samSTPtr->alnReadLenUI
      < secConSTPtr->samSTPtr->alnReadLenUI
   ){ /*Else If: second cluster is longer*/
      swap_con_clustST(
         firstConSTPtr,
         secConSTPtr
      );

      retSC = -1;
   } /*Else If: second cluster is longer*/

   else if(
        firstConSTPtr->numReadsUL
      < secConSTPtr->numReadsUL
   ){ /*Else If: second cluster has more reads*/
      swap_con_clustST(
         firstConSTPtr,
         secConSTPtr
      );

      retSC = -1;
   } /*Else If: second cluster has more reads*/

   else
      retSC = 1; /*first cluster is equal or better*/

   firstConSTPtr->startUI =
      min_genMath(
         firstConSTPtr->startUI,
         secConSTPtr->startUI
      );

   firstConSTPtr->endUI =
      max_genMath(
         firstConSTPtr->endUI,
         secConSTPtr->endUI
      );


   /*****************************************************\
   * Fun23 Sec03 Sub03:
   *   - merge clusters and return keep consensus
   \*****************************************************/

   for(
      distSL = 0;
      distSL < (signed long) indexSTPtr->lenSamUL;
      ++distSL
   ){ /*Loop: merge clusters*/
      if(indexSTPtr->clustArySI[distSL] == changeClustSI)
         indexSTPtr->clustArySI[distSL] = keepClustSI;
   } /*Loop: merge clusters*/

   return retSC;
} /*cmpCons_clustST*/

/*-------------------------------------------------------\
| Fun24: plist_con_clustST
|   - print the clusters consensuses in a con_clustST list
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct list with
|       consensuses to print
|   - headerStr:
|     o c-string with header to print (null = no header)
|   - pgHeadStr:
|     o c-string with program header to print (null = no
|       header)
|   - outFILE:
|     o file to print consensuses to
| Output:
|   - Prints:
|     o headers and consensus to outFILE as a sam file
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memeory errors
\-------------------------------------------------------*/
signed char
plist_con_clustST(
   struct con_clustST *conSTPtr, /*consensuses to print*/
   signed char *headerStr,       /*sam file header*/
   signed char *pgHeadStr,       /*program header*/
   void *outFILE                 /*file to print to*/
){
   signed char errSC = 0;
   signed char *tmpStr = 0;
   struct samEntry *samSTPtr = 0;

   if(headerStr)
      fprintf(
         outFILE,
         "%s",
         headerStr
      ); /*print headers for sam file*/

   if(pgHeadStr)
      fprintf(
         outFILE,
         "%s",
         pgHeadStr
      ); /*prints the program header for sam file*/

   while(conSTPtr)
   { /*Loop: print consensuses*/
      samSTPtr = conSTPtr->samSTPtr;

      if(! samSTPtr)
      { /*If: cluster is empty*/
         conSTPtr = conSTPtr->nextST;
         continue;
      } /*If: cluster is empty*/

      /*most of time query id not informative*/
      tmpStr = samSTPtr->qryIdStr;
      samSTPtr->qryIdLenUC = 0;

      *tmpStr++ = 'c';
      ++samSTPtr->qryIdLenUC;
      *tmpStr++ = 'l';
      ++samSTPtr->qryIdLenUC;
      *tmpStr++ = 'u';
      ++samSTPtr->qryIdLenUC;
      *tmpStr++ = 's';
      ++samSTPtr->qryIdLenUC;
      *tmpStr++ = 't';
      ++samSTPtr->qryIdLenUC;
      *tmpStr++ = ':';
      ++samSTPtr->qryIdLenUC;

      samSTPtr->qryIdLenUC +=
         numToStr(
            tmpStr,
            conSTPtr->clustSI
         );

      p_samEntry(samSTPtr, 1, outFILE);

      if(errSC)
         goto memErr_fun24;

      fprintf(
         outFILE,
         "\tclust:i:%i%s",
         conSTPtr->clustSI,
         str_endLine
      );

      conSTPtr = conSTPtr->nextST;
      samSTPtr = 0;
   } /*Loop: print consensuses*/

   fflush(outFILE);
   return 0;

   memErr_fun24:;
   return def_memErr_clustST;
} /*plist_con_edCLust*/

/*-------------------------------------------------------\
| Fun25: getClust_clustST
|   - extracts reads for a signle cluster
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct with clusters
|   - clustSI:
|     o cluster number to extract reads for
|   - headStr:
|     o program header to print
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - buffStrPtr:
|     o pointer to c-string to use reading sam file
|   - lenBuffULPtr:
|     o pointer to unsigned long with size of buffStrPtr
|   - samFILE:
|     o sam file to extract reads from
|   - outFILE:
|     o file to save reads to
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o lenBuffULPtr to have buffStrPtr size (if resized)
|   - Returns:
|     o 0 for no errors
|     o def_memErr for memory errors
\-------------------------------------------------------*/
signed char
getClust_clustST(
   struct index_clustST *indexSTPtr,
   signed int clustSI,
   signed char *headStr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   unsigned long *lenBuffULPtr,
   void *samFILE,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun25 TOC:
   '   - finds number of reads mapping to best read
   '   o fun25 sec01:
   '     - variable declarations
   '   o fun25 sec02:
   '     - move to start of sam file
   '   o fun25 sec03:
   '     - extract reads
   '   o fun25 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed long lineSL = 0;
   unsigned long offsetUL = 0;

   signed char headBl = 1; /*need to print out header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec02:
   ^   - move to start of sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fseek(samFILE, 0, SEEK_SET);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec03:
   ^   - extract reads
   ^   o fun25 sec03 sub01:
   ^     - check if read in cluster (or is header)
   ^   o fun25 sec03 sub02:
   ^     - get read
   ^   o fun25 sec03 sub03:
   ^     - print read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec03 Sub01:
   *   - check if read in cluster (or is header)
   \*****************************************************/

   for(
      lineSL = 0;
      lineSL < (signed long) indexSTPtr->lenSamUL;
      ++lineSL
   ){ /*Loop: find reads assigned to cluster*/

      if(
             indexSTPtr->clustArySI[lineSL]
          == def_header_clustST
      ) ; /*is a header; print out*/

      else if(indexSTPtr->clustArySI[lineSL] != clustSI)
      { /*If: read not in cluster*/
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         continue;
      } /*If: read not in cluster*/

      /**************************************************\
      * Fun25 Sec03 Sub02:
      *   - get read
      \**************************************************/

      if(
         indexSTPtr->lenLineAryUI[lineSL] >= *lenBuffULPtr
      ){ /*If: need more memory*/
         free(*buffStrPtr);
         *buffStrPtr = 0;
         *lenBuffULPtr = 0;
      } /*If: need more memory*/

      if(! *buffStrPtr)
      { /*If: need to get memory*/
         *buffStrPtr =
            malloc(
               (indexSTPtr->lenLineAryUI[lineSL] + 8)
                  * sizeof(signed char)
            );

         if(! *buffStrPtr)
            goto err_fun25_sec04;
         *lenBuffULPtr = indexSTPtr->lenLineAryUI[lineSL];
      } /*If: need to get memory*/

      errSC =
         getRead_clustST(
            samSTPtr,
            (unsigned long) lineSL,
            offsetUL,
            indexSTPtr,
            *buffStrPtr,
            samFILE
         ); /*get read from sam file*/

      offsetUL = 0;

      if(errSC)
         goto err_fun25_sec04;

      /**************************************************\
      * Fun25 Sec03 Sub03:
      *   - print read
      \**************************************************/
 
      if(samSTPtr->extraStr[0] == '@')
      { /*If: comment*/
         fprintf(
            (FILE *) outFILE,
            "%s%s",
            samSTPtr->extraStr,
            str_endLine
         );

         continue;
      } /*If: comment*/

      if(headBl)
      { /*If: need to print program header*/
         headBl = 0;

         fprintf(
            (FILE *) outFILE,
            "%s%s",
            headStr,
            str_endLine
         );
      } /*If: need to print program header*/

      if(indexSTPtr->clustArySI[lineSL] == clustSI)
      { /*Else If: printing read*/
         p_samEntry(samSTPtr, 1, outFILE);

         if(errSC == def_memErr_samEntry)
            goto err_fun25_sec04;

         fprintf(
            (FILE *) outFILE,
            "\tclust:i:%i%s",
            clustSI,
            str_endLine
         );
      } /*Else If: printing read*/
   } /*Loop: find number of matches*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun25_sec04;


   err_fun25_sec04:;
   errSC = def_memErr_clustST;
   goto ret_fun25_sec04;


   ret_fun25_sec04:;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   return errSC;
} /*getClust_clustST*/

/*-------------------------------------------------------\
| Fun26: pbins_clustST
|   - extracts reads for each cluster to a sam file
| Input:
|   - prefixStr:
|     o c-string with prefix for output file name
|   - clustSI:
|     o last cluster to extract reads for, should be last
|       cluster in targArySI, but does not need to be
|   - indexSTPtr:
|     o pointer to index_clustST struct with clusters
|   - pgHeadStr:
|     o program header to print (null = no print)
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - buffStrPtr:
|     o pointer to c-string to use reading sam file
|   - lenBuffULPtr:
|     o pointer to unsigned long with size of buffStrPtr
|   - samFILE:
|     o sam file to extract reads from
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o lenBuffULPtr to have buffStrPtr size (if resized)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
|     o def_fileErr_clustST for file errors
\-------------------------------------------------------*/
signed char
pbins_clustST(
   signed char *prefixStr,
   signed int clustSI,
   struct index_clustST *indexSTPtr,
   signed char *pgHeadStr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   unsigned long *lenBuffULPtr,
   void *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun26 TOC:
   '   - extracts reads for each cluster to a sam file
   '   o fun26 sec01:
   '     - variable declarations
   '   o fun26 sec02:
   '     - allocate memory for header
   '   o fun26 sec03:
   '     - get header
   '   o fun26 sec04:
   '     - extract reads
   '   o fun26 sec05:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed long lineSL = 0;

   signed char *tmpStr = 0;
   signed char *headHeapStr = 0;
   unsigned int lenHeadUI = 4096;
   unsigned int bytesUI = 0;

   signed char *clustHeapAryBl = 0;
      /*tells if printed header*/
   signed char outFileStr[2048];/*holds output file name*/
   unsigned int lenPrefixUI = endStr_ulCp(prefixStr);
   FILE *outFILE = 0;

   unsigned long offsetUL = 0; /*offset to next read*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec02:
   ^   - allocate memory for header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fseek(samFILE, 0, SEEK_SET);

   for(
      lineSL = 0;
      lineSL < (signed long) indexSTPtr->lenSamUL;
      ++lineSL
   ){ /*Loop: get header length*/
      if(
             indexSTPtr->clustArySI[lineSL]
          != def_header_clustST
      ) break;; /*on reads*/

      lenHeadUI += indexSTPtr->lenLineAryUI[lineSL];
   } /*Loop: get header length*/


   headHeapStr =
      malloc((lenHeadUI + 1) * sizeof(signed char));

   if(! headHeapStr)
      goto memErr_fun26_sec05;

   if(clustSI > 0)
      clustHeapAryBl =
         calloc(
            clustSI + 1,
            sizeof(signed char)
         );
   else
      clustHeapAryBl =
         calloc(
            2,
            sizeof(signed char)
         );

   if(! clustHeapAryBl)
      goto memErr_fun26_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec03:
   ^   - get header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      lineSL = 0;
      lineSL < (signed long) indexSTPtr->lenSamUL;
      ++lineSL
   ){ /*Loop: get header*/

      if(
             indexSTPtr->clustArySI[lineSL]
          != def_header_clustST
      ) break; /*on reads*/

      if(
         indexSTPtr->lenLineAryUI[lineSL] >= *lenBuffULPtr
      ){ /*If: have to little memory*/
         if(*buffStrPtr)
            free(*buffStrPtr);
         *buffStrPtr = 0;
      }  /*If: have to little memory*/

      if(! *buffStrPtr)
      { /*If: have to little memory*/
         *buffStrPtr =
            malloc(
               (indexSTPtr->lenLineAryUI[lineSL] + 8)
                 * sizeof(signed char)
            );

         if(! *buffStrPtr)
            goto memErr_fun26_sec05;
         *lenBuffULPtr = indexSTPtr->lenLineAryUI[lineSL];
      }  /*If: have to little memory*/

      offsetUL =
         fread(
            *buffStrPtr,
            sizeof(signed char),
            indexSTPtr->lenLineAryUI[lineSL],
            samFILE
         );

      (*buffStrPtr)[offsetUL] = '\0';

      if(! offsetUL)
         goto fileErr_fun26_sec05;

      cpLen_ulCp(
         &headHeapStr[bytesUI],
         *buffStrPtr,
         indexSTPtr->lenLineAryUI[lineSL]
      );

      bytesUI += indexSTPtr->lenLineAryUI[lineSL];
   } /*Loop: get header*/

   offsetUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec04:
   ^   - extract reads
   ^   o fun26 sec04 sub01:
   ^     - filter out non-cluster reads + start loop
   ^   o fun26 sec04 sub02:
   ^     - get read
   ^   o fun26 sec04 sub03:
   ^     - build file name for the reads cluster
   ^   o fun26 sec04 sub04:
   ^     - open the clusters sam file (bin)
   ^   o fun26 sec04 sub05:
   ^     - print read to clusters sam file
   ^   o fun26 sec04 sub06:
   ^     - move to next read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun26 Sec04 Sub01:
   *   - filter out non-cluster reads + start loop
   \*****************************************************/

   while(lineSL < (signed long) indexSTPtr->lenSamUL)
   { /*Loop: print clusters*/
      if(indexSTPtr->clustArySI[lineSL] <= 0)
      { /*If: not a cluster*/
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         ++lineSL;
         continue;
      } /*If: not a cluster*/

      if(indexSTPtr->clustArySI[lineSL] > clustSI)
      { /*If: beyond cluster range*/
         offsetUL += indexSTPtr->lenLineAryUI[lineSL];
         ++lineSL;
         continue;
      } /*If: beyond cluster range*/

      /**************************************************\
      * Fun25 Sec03 Sub02:
      *   - get read
      \**************************************************/

      if(
         indexSTPtr->lenLineAryUI[lineSL] >= *lenBuffULPtr
      ){ /*If: have to little memory*/
         if(*buffStrPtr)
            free(*buffStrPtr);
         *buffStrPtr = 0;
      }  /*If: have to little memory*/

      if(! *buffStrPtr)
      { /*If: have to little memory*/
         *buffStrPtr =
            malloc(
               (indexSTPtr->lenLineAryUI[lineSL] + 8)
                 * sizeof(signed char)
            );

         if(! *buffStrPtr)
            goto memErr_fun26_sec05;
         *lenBuffULPtr = indexSTPtr->lenLineAryUI[lineSL];
      }  /*If: have to little memory*/

      errSC =
         getRead_clustST(
            samSTPtr,
            (unsigned long) lineSL,
            offsetUL,
            indexSTPtr,
            *buffStrPtr,
            samFILE
         ); /*get read from sam file*/

      offsetUL = 0;

      if(errSC)
      { /*If: had error*/
         if(errSC == def_memErr_samEntry)
            goto memErr_fun26_sec05;
         if(errSC == def_fileErr_samEntry)
            goto fileErr_fun26_sec05;
      } /*If: had error*/

      /**************************************************\
      * Fun26 Sec04 Sub03:
      *   - build file name for the reads cluster
      \**************************************************/

       tmpStr = outFileStr;

       cpLen_ulCp(
          tmpStr,
          prefixStr,
          lenPrefixUI
       ); /*ad prefix to file name*/

       tmpStr += lenPrefixUI;
       *tmpStr++ = '-';

       cpLen_ulCp(
          tmpStr,
          samSTPtr->refIdStr,
          samSTPtr->refIdLenUC
       ); /*add reference id to file name*/

       tmpStr += samSTPtr->refIdLenUC;
       *tmpStr++ = '-';

       tmpStr +=
          numToStr(
             tmpStr,
             indexSTPtr->clustArySI[lineSL]
          ); /*add cluster number to file name*/

       *tmpStr++ = '.';
       *tmpStr++ = 's';
       *tmpStr++ = 'a';
       *tmpStr++ = 'm';
       *tmpStr = '\0';

      /**************************************************\
      * Fun26 Sec04 Sub04:
      *   - open the clusters sam file (bin)
      \**************************************************/

       if(
            clustHeapAryBl[indexSTPtr->clustArySI[lineSL]]
          == 0
       ){ /*If: first read in cluster*/

          outFILE =
             fopen(
                (char *) outFileStr,
                "w"
             );

          if(! outFILE)
             goto fileErr_fun26_sec05;

          fprintf(
             outFILE,
             "%s",
             headHeapStr /*has new line at end*/
          ); /*print header for sam file*/

          if(pgHeadStr)
             fprintf(
                outFILE,
                "%s",
                pgHeadStr
             ); /*print header for sam file*/

          clustHeapAryBl[
             indexSTPtr->clustArySI[lineSL]
          ] = 1;
       } /*If: first read in cluster*/

       else
       { /*Else: not a new file*/
          outFILE = fopen((char *) outFileStr, "a");
          if(! outFILE)
             goto fileErr_fun26_sec05;
       } /*Else: not a new file*/

      /**************************************************\
      * Fun26 Sec04 Sub05:
      *   - print read to clusters sam file
      \**************************************************/

       p_samEntry(samSTPtr, 1, outFILE);

       fprintf(
          outFILE,
          "\tclust:i:%i%s",
          indexSTPtr->clustArySI[lineSL],
          str_endLine
       );

      fclose(outFILE);
      outFILE = 0;

      /**************************************************\
      * Fun26 Sec04 Sub06:
      *   - move to next read
      \**************************************************/

      ++lineSL;
   } /*Loop: print clusters*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun26 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
      goto ret_fun26_sec05;


   memErr_fun26_sec05:;
      errSC = def_memErr_clustST;
      goto ret_fun26_sec05;


   fileErr_fun26_sec05:;
      errSC = def_fileErr_clustST;
      goto ret_fun26_sec05;


   ret_fun26_sec05:;
      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      if(headHeapStr)
         free(headHeapStr);
      headHeapStr = 0;

      if(clustHeapAryBl)
         free(clustHeapAryBl);
      clustHeapAryBl = 0;

      fseek(
         samFILE,
         0,
         SEEK_SET
      );

      return errSC;
} /*pbins_clustST*/

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
