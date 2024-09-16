/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' clustSam SOF: Start Of File
'   - clusters reads by similarity
'   o header:
'     - included libraries
'   o .h st01: profile_clustSam
'     - holds profile (used for clustering) for a
'       reference read
'   o fun01: blank_profile_samClust
'     - blanks an profile_samClust structure
'   o fun02: init_profile_samClust
'     - initializes a profile_samClust structure
'   o fun03: setupReadHitAry_profile_samClust
'     - allocates memory for the readHitArySI array in
'       a profile_samClust struct
'   o fun04: freeStack_profile_samClust
'     - frees all variables in a profile_samClust struct
'   o fun05: freeHeap_profile_samClust
'     - frees a profile_samClust struct on heap
'   o .c fun06: swap_profile_samClust
'     - swaps two index's in a the arrays in a
'       profile_clustSam struct
'   o fun07: sortDepth_profile_samClust
'     - sorts arrays in profile_samClust struct by depth
'   o fun08: sortPos_profile_samClust
'     - sorts arrays in profile_samClust struct by
'       position
'   o fun09: resort_profile_clustSam
'     - resorts profile_clustSam to original order
'   o .c fun10: allocRead_profile_clustSam
'     - allocates memory (makes sure have enough memory)
'       for a signle reads profile
'   o fun11: build_profile_clustSam
'     - builds a profile for a profile_clustSam struct
'   o fun12: addReadTo_profile_clustSam
'     - adds a read to a profile_clustSam struct
'   o fun13: bust_profile_samClust
'     - finds variants in profile to extract reads with
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

#include "samClust.h"

#include <stdio.h>

#include "../genLib/seqST.h"
#include "../genLib/samEntry.h"
#include "../genLib/edDist.h"

#include "../genLib/alnSet.h"
#include "../genLib/dirMatrix.h"
#include "../genLib/needle.h"

/*.h files only (no .c files)*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*only using .h min macro*/
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/charCp.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
!   - .h  #include "../genAlnindexToCoord.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_profile_samClust
|   - blanks a profile_samClust structure
| Input:
|   - profSTPtr:
|     o pointer to a profile_samClust structure to blank
|   - blankRegFlagSC:
|     o blank discard status for readHitArySI (you can
|       combine flags with (flag1 | flag2 | ... | flagn)
|       - def_blankProfile_clustSam blanks all profile
|         discarded reads (no longer discarded)
|       - def_blankRegion_clustSam blanks all region
|         discarded reads (no longer discarded)
|       - def_blankDiscard_clustSam blanks all discarded
|         reads (no longer discarded)
| Output:
|   - Modifies:
|     o all variables in profSTPtr to be 0 or default
\-------------------------------------------------------*/
void
blank_profile_samClust(
   struct profile_samClust *profSTPtr,
   signed char blankRegFlagSC
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - blanks a profile_samClust structure
   '   o fun01 sec01:
   '     - variable declarations and small blanks
   '   o fun01 sec02:
   '     - blank profile and profile supporting arrays
   '   o fun01 sec03:
   '     - blank read status arrays
   '   o fun01 sec04:
   '     - blank profile history arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations and small blanks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiCnt = 0;

   profSTPtr->profStartUI = 0;
   profSTPtr->profEndUI = 0;
   profSTPtr->refIdStr[0] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - blank profile and profile supporting arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         profSTPtr->posAryUI
      || profSTPtr->baseAryUC
      || profSTPtr->typeArySC
      || profSTPtr->supAryUI
      || profSTPtr->mapAryUI
   ){ /*If: have at least on array to blank*/
      for(
         uiCnt = 0;
         uiCnt < profSTPtr->lenProfileUI;
         ++uiCnt
      ){ /*Loop: blank arrays*/
         if(profSTPtr->posAryUI)
            profSTPtr->posAryUI[uiCnt] = 0;

         if(profSTPtr->lenAryUI)
            profSTPtr->lenAryUI[uiCnt] = 0;

         if(profSTPtr->baseAryUC)
            profSTPtr->baseAryUC[uiCnt] =
              def_blankNt_clustSam;

         if(profSTPtr->typeArySC)
            profSTPtr->typeArySC[uiCnt] = (schar) '\0';

         if(profSTPtr->supAryUI)
            profSTPtr->supAryUI[uiCnt] = 0;

         if(profSTPtr->mapAryUI)
            profSTPtr->mapAryUI[uiCnt] = 0;
      } /*Loop: blank arrays*/
   } /*If: have at least on array to blank*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - blank read status arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         profSTPtr->readHitArySI
      && blankRegFlagSC > 0
   ){ /*If: blanking the read hit array*/

      for(
         uiCnt = 0;
         uiCnt < der_lenHist_clustSam;
         ++uiCnt
      ){ /*Loop: blank read hits*/
         if(profSTPtr->readHitAry[uiCnt] >= 0)
            profSTPtr->readHitAry[uiCnt] = 0;

         else if(
               profSTPtr->readHitAry[uiCnt]
            == def_profileDiscard_clustSam
            &&
               blankRegFlagSC & def_blankProfile_clustSam
         ) profSTPtr->readHitAry[uiCnt] = 0;

         else if(
                  profSTPtr->readHitAry[uiCnt]
               == def_regionDiscard_clustSam
            &&
               blankRegFlagSC & def_blankRegion_clustSam
         ) profSTPtr->readHitAry[uiCnt] = 0;

         else if(
                  profSTPtr->readHitAry[uiCnt]
               == def_discard_clustSam
            &&
               blankRegFlagSC & def_blankDiscard_clustSam
         ) profSTPtr->readHitAry[uiCnt] = 0;
      } /*Loop: blank read hits*/

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun01 Sec04:
      ^   - blank profile history arrays
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(
            profSTPtr->cntAryUI
         || profSTPtr->depthAryUI
         || profSTPtr->varAryUI
         || profSTPtr->histAryUI
      ){ /*If: have history array to blank*/
         for(
            uiCnt = 0;
            uiCnt < profSTPtr->histIndexUI;
            ++uiCnt
         ){ /*Loop: blank variant and count history*/
            if(profSTPtr->cntAryUI)
               profSTPtr->cntAryUI[uiCnt] = 0;

            if(profSTPtr->depthAryUI)
               profSTPtr->depthAryUI[uiCnt] = 0;

            if(profSTPtr->varAryUI)
               profSTPtr->varAryUI[uiCnt] = -1;

            if(profSTPtr->histAryUI)
               profSTPtr->histAryUI[uiCnt] = 0;
         } /*Loop: blank variant and count history*/
      } /*If: have history array to blank*/
   } /*If: blanking the read hit array*/

   profSTPtr->histIndexUI = 0;
} /*blank_profile_samClust*/

/*-------------------------------------------------------\
| Fun02: init_profile_samClust
|   - initializes a profile_samClust structure
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to initialize
| Output:
|   - Modifies:
|     o all arrays to be null (0) and array size to be 0
\-------------------------------------------------------*/
void
init_profile_samClust(
   struct profile_samClust *profSTPtr
){
   profSTPtr->posAryUI = 0;
   profSTPtr->lenAryUI = 0;
   profSTPtr->baseAryUC = 0;
   profSTPtr->typeArySC = 0;
   profSTPtr->supAryUI = 0;
   profSTPtr->mapAryUI = 0;
   profSTPtr->lenProfileUI = 0;
   profSTPtr->readHitArySI = 0;
   profSTPtr->lenHitUL = 0;

   profSTPtr->cntAryUI = 0;
   profSTPtr->depthAryUI = 0;
   profSTPtr->varAryUI = 0;
   profSTPtr->histAryUI = 0;
   profSTPtr->lenHistUI = 0;
   profSTPtr->histIndexUI = 0;

   blank_profile_samClust(
      profSTPtr,
      0             /*do not care about read hits*/
   );
} /*init_profile_samClust*/

/*-------------------------------------------------------\
| Fun03: setupReadHitAry_profile_samClust
|   - allocates memory for the readHitArySI array in
|     a profile_samClust struct
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to setup
|       the readHitArySI array
|   - numReadsUL:
|     o number of reads in the sam file
| Output:
|   - Modifies:
|     o readHitArySI to have memory
|     o lenHitUL to be numReadsUL
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
setupReadHitAry_profile_samClust(
   stuct profile_samClust *profSTPtr,
   unsigned loong numReadsUL
){
   ulong ulBlank = 0;

   if(profSTPtr->readHitArySI)
   { /*If: already have an array*/

      if(profSTPtr->lenHitUL >= numReadsUL)
      { /*If: array is already correct size*/

         for(
            ulBlank = 0;
            ulBlank < numReadsUL;
            ++ulBlank
         ) profSTPtr->readHitArySI[ulBlank] = 0;

         goto ret_noErr_fun03;
      } /*If: array is already correct size*/

      free(profSTPtr->readHitArySI);
   } /*If: already have an array*/

   profSTPtr->readHitArySI = 0;

   profSTPtr->readHitArySI =
      calloc(
         numReadsUL,
         sizeof(sint)
      );
 
   if(! profSTPtr->readHitArySI)
      goto ret_memErr_fun03;

   ret_noErr_fun03:;
   return 0;

   ret_memErr_fun03:;
   return def_memErr_clustSam;
} /*setupReadHitAry_profile_samClust*/

/*-------------------------------------------------------\
| Fun04: freeStack_profile_samClust
|   - frees all variables in a profile_samClust struct
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct with internal
|       variables to free
| Output:
|   - Frees:
|     o all arrays and sets to 0
|   - Modifies:
|     o lenProfileUI to be 0
\-------------------------------------------------------*/
void
freeStack_profile_samClust(
   struct profile_samClust *profSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   o fun04 sec01:
   '     - free profile arrays (pos/len/base/type/map/sup)
   '   o fun04 sec02:
   '     - free read hit array
   '   o fun04 sec03:
   '     - free history arrays
   '   o fun04 sec04:
   '     - run blank function
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - free profile arrays (pos/len/base/type/map/sup)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(profSTPtr)
   { /*If: have struct to free*/
      if(profSTPtr->posAryUI)
         free(profSTPtr->posAryUI);

      profSTPtr->posAryUI = 0;


      if(profSTPtr->lenAryUI)
         free(profSTPtr->lenAryUI);

      profSTPtr->lenAryUI = 0;


      if(profSTPtr->baseAryUC)
         free(profSTPtr->baseAryUC);

      profSTPtr->baseAryUC = 0;


      if(profSTPtr->typeArySC)
         free(profSTPtr->typeArySC);

      profSTPtr->typeArySC = 0;


      if(profSTPtr->supAryUI)
         free(profSTPtr->supAryUI);

      profSTPtr->supAryUI = 0;


      if(profSTPtr->mapAryUI)
         free(profSTPtr->mapAryUI);

      profSTPtr->mapAryUI = 0;

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun04 Sec02:
      ^   - free read hit array
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(profSTPtr->readHitArySI)
         free(profSTPtr->readHitArySI);

      profSTPtr->readHitArySI = 0;
      profSTPtr->lenHitUL = 0;

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun04 Sec03:
      ^   - free history arrays
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      if(profSTPtr->cntAryUI)
         free(profSTPtr->cntAryUI);

      profSTPtr->cntAryUI = 0;


      if(profSTPtr->depthAryUI)
         free(profSTPtr->depthAryUI);

      profSTPtr->depthAryUI = 0;


      if(profSTPtr->varAryUI)
         free(profSTPtr->varAryUI);

      profSTPtr->varAryUI = 0;


      if(profSTPtr->histAryUI)
         free(profSTPtr->histAryUI);

      profSTPtr->histAryUI = 0;


      profSTPtr->lenHistUI = 0;
      profSTPtr->histIndexUI = 0;

      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
      ^ Fun04 Sec04:
      ^   - run blank function
      \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      profSTPtr->lenProfileUI = 0;

      blank_profile_samClust(
         profSTPtr,
         0             /*read hit array freeded
      );
   } /*If: have struct to free*/
} /*freeStack_profile_samClust*/

/*-------------------------------------------------------\
| Fun05: freeHeap_profile_samClust
|   - frees a profile_samClust struct on heap
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to free
| Output:
|   - Frees:
|     o profSTPtr (you must set to null)
\-------------------------------------------------------*/
void
freeHeap_profile_samClust(
   struct profile_samClust *profSTPtr
){
   if(profSTPtr)
   { /*If: have struct to free*/
      freeStack_profile_samClust(profSTPtr);
      free(profSTPtr);
   } /*If: have struct to free*/
} /*freeHeap_profile_samClust*/

/*-------------------------------------------------------\
| Fun06: swap_profile_samClust
|   - swaps two index's in a the arrays in a
|     profile_clustSam struct
| Input:
|   - profSTPtr:
|     o pointer to a profile_clustSam struct to swap two
|       items
|   - oneUL:
|     o first item to swap
|   - twoUL:
|     o second item to swap
| Output:
|   - Modifies:
|     o posAryUI, lenAryUI, baseAryUC, typeArySC,
|       supAryUI, and mapAryUI to have the two
|       index's swapped
\-------------------------------------------------------*/
void
swap_profile_samClust(
   struct swap_profile_samClust *profSTPtr,
   unsigned long oneUL,
   unsigned long twoUL
){
   profSTPtr->posAryUI[oneUL] ^=
      profSTPtr->posAryUI[twoUL];
   profSTPtr->posAryUI[twoUL] ^=
      profSTPtr->posAryUI[oneUL];
   profSTPtr->posAryUI[oneUL] ^=
      profSTPtr->posAryUI[twoUL];

   profSTPtr->lenAryUI[oneUL] ^=
      profSTPtr->lenAryUI[twoUL];
   profSTPtr->lenAryUI[twoUL] ^=
      profSTPtr->lenAryUI[oneUL];
   profSTPtr->lenAryUI[oneUL] ^=
      profSTPtr->lenAryUI[twoUL];

   profSTPtr->baseAryUC[oneUL] ^=
      profSTPtr->baseAryUC[twoUL];
   profSTPtr->baseAryUC[twoUL] ^=
      profSTPtr->baseAryUC[oneUL];
   profSTPtr->baseAryUC[oneUL] ^=
      profSTPtr->baseAryUC[twoUL];

   profSTPtr->typeArySC[oneUL] ^=
      profSTPtr->typeArySC[twoUL];
   profSTPtr->typeArySC[twoUL] ^=
      profSTPtr->typeArySC[oneUL];
   profSTPtr->typeArySC[oneUL] ^=
      profSTPtr->typeArySC[twoUL];

   profSTPtr->supAryUI[oneUL] ^=
      profSTPtr->supAryUI[twoUL];
   profSTPtr->supAryUI[twoUL] ^=
      profSTPtr->supAryUI[oneUL];
   profSTPtr->supAryUI[oneUL] ^=
      profSTPtr->supAryUI[twoUL];

   profSTPtr->mapAryUI[oneUL] ^=
      profSTPtr->mapAryUI[twoUL];
   profSTPtr->mapAryUI[twoUL] ^=
      profSTPtr->mapAryUI[oneUL];
   profSTPtr->mapAryUI[oneUL] ^=
      profSTPtr->mapAryUI[twoUL];

   profSTPtr->indexAryIU[oneUL] ^=
      profSTPtr->indexAryIU[twoUL];
   profSTPtr->indexAryIU[twoUL] ^=
      profSTPtr->indexAryIU[oneUL];
   profSTPtr->indexAryIU[oneUL] ^=
      profSTPtr->indexAryIU[twoUL];
} /*swap_profile_samClust*/

/*-------------------------------------------------------\
| Fun07: sortDepth_profile_samClust
|   - sorts arrays in profile_samClust struct by depth
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to sort
| Output:
|   - Modifies:
|     o arrays in profSTPtr to be sorted by depth
\-------------------------------------------------------*/
void
sortDepth_profile_samClust(
   struct profile_samClust *profSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - sorts arrays in profSTPtr by depth
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find the number of rounds to sort for
   '   o fun07 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = profSTPtr->lenProfileUI -1;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ulIndex = 0;
   unsigned long ulElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if( ((slong) numElmUI) <= 0)
      return; /*nothing to sort*/

   /*Recursion formuia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ulIndex = 0;
         ulIndex <= subUL;
         ++ulIndex
      ){ /*Loop: though sub array*/
         ulElm = ulIndex;

         for(
            ulElm = ulIndex;
            ulElm + subUL <= endUL;
            ulElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ulElm + subUL;

            if(
                  profSTPtr->supAryUI[ulElm]
               < profSTPtr->supAryUI[nextUL]
            ) ; /*not positioned yet*/

            else if(
                 profSTPtr->supAryUI[ulElm]
               > profSTPtr->supAryUI[nextUL]
            ) goto swap_fun07_sec03;

            else if(
                 profSTPtr->mapAryUI[ulElm]
               > profSTPtr->mapAryUI[nextUL]
            ) goto swap_fun07_sec03;

            else if(
                 profSTPtr->posAryUI[ulElm]
               > profSTPtr->posAryUI[nextUL]
            ) goto swap_fun07_sec03;

            else if(
                 profSTPtr->lenAryUI[ulElm]
               > profSTPtr->lenAryUI[nextUL]
            ) goto swap_fun07_sec03;

            else if(
                 profSTPtr->baseAryUC[nextUL]
              != def_blankNt_clustSam
            ) ;

            else if(
                 profSTPtr->baseAryUC[nextUL] != 'I'
            ) goto swap_fun07_sec03;

            else
            { /*Else: swap elements*/
               swap_fun07_sec03:;

               swap_profile_samClust(
                 profStrPtr,
                 ulElm,
                 nextUL
               );

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(
                        profSTPtr->supAryUI[onUL]
                     < profSTPtr->supAryUI[lastUL]
                  ) ; /*not positioned yet*/

                  else if(
                       profSTPtr->supAryUI[onUL]
                     > profSTPtr->supAryUI[lastUL]
                  ) break;

                  else if(
                       profSTPtr->mapAryUI[onUL]
                     > profSTPtr->mapAryUI[lastUL]
                  ) break;

                  else if(
                       profSTPtr->posAryUI[onUL]
                     > profSTPtr->posAryUI[lastUL]
                  ) break;

                  else if(
                       profSTPtr->lenAryUI[onUL]
                     > profSTPtr->lenAryUI[lastUL]
                  ) break;

                  else if(
                       profSTPtr->typeAryUC[lastUL]
                    != def_blankNt_clustSam
                  ) ;

                  else if(
                       profSTPtr->typeAryUC[lastUL] != 'I'
                  ) break; /*so insertions come first*/

                  else
                     break; /*lastUL is '\0'*/

                  swap_profile_samClust(
                    profStrPtr,
                    onUL,
                    lastUL
                  );

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*Else: swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortDepth_profile_samClust*/

/*-------------------------------------------------------\
| Fun08: sortPos_profile_samClust
|   - sorts arrays in profile_samClust struct by position
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to sort
| Output:
|   - Modifies:
|     o arrays in profSTPtr to be sorted by position
\-------------------------------------------------------*/
void
sortPos_profile_samClust(
   struct profile_samClust *profSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - sorts arrays in piofileSTPtr by position
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - find the number of rounds to sort for
   '   o fun08 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   unsigned long numElmUL = profSTPtr->lenProfileUI -1;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ulIndex = 0;
   unsigned long ulElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if( ((slong) numElmUI) <= 0)
      return; /*nothing to sort*/

   /*Recursion formuia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ulIndex = 0;
         ulIndex <= subUL;
         ++ulIndex
      ){ /*Loop: though sub array*/
         ulElm = ulIndex;

         for(
            ulElm = ulIndex;
            ulElm + subUL <= endUL;
            ulElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ulElm + subUL;

            if(
                 profSTPtr->posAryUI[ulElm]
               < profSTPtr->posAryUI[nextUL]
            ) ;

            else if(
                 profSTPtr->posAryUI[ulElm]
               > profSTPtr->posAryUI[nextUL]
            ) goto swapElm_fun08_sec03;

            else if(
                 profSTPtr->lenAryUI[ulElm]
               > profSTPtr->lenAryUI[nextUL]
            ) goto swapElm_fun08_sec03;

            else if(profSTPtr->typeArySC[nextUL] == 'I')
               goto swapElm_fun08_sec03;

            else if(
              profSTPtr->baseAryUC[nextUL] != '\0'
            ) ; /* want null bases to come first*/

            else
            ){ /*Else: I need to swap an element*/
               swapElm_fun08_sec03:;

               swap_profile_samClust(
                 profStrPtr,
                 ulElm,
                 nextUL
               );

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(
                        profSTPtr->posAryUI[onUL]
                     < profSTPtr->posAryUI[lastUL]
                  ) ; /*not positioned yet*/

                  else if(
                       profSTPtr->posAryUI[onUL]
                     > profSTPtr->posAryUI[lastUL]
                  ) break;

                  else if(
                       profSTPtr->lenAryUI[onUL]
                     > profSTPtr->lenAryUI[lastUL]
                  ) break;

                  else if(
                     profSTPtr->typeArySC[nextUL] != 'I'
                  ) break; /*so insertions come first*/

                  else if(
                    profSTPtr->baseAryUC[lastUL] !='\0'
                  ) ;

                  else
                     break; /*lastUL is '\0'*/

                  swap_profile_samClust(
                    profStrPtr,
                    onUL,
                    lastUL
                  );

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*Else: I need to swap an element*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortDepth_profile_samClust*/

/*-------------------------------------------------------\
| Fun09: resort_profile_clustSam
|   - resorts profile_clustSam to original order; position
| Input:
|   o profSTPtr:
|     - pointer to a profiler_clustSam struct to resort
| Output:
|   - Modifies:
|     o arrays in profSTPtr to be resorted
\-------------------------------------------------------*/
void
resort_profile_clustSam(
   struct profile_clustSam *profSTPtr
){
   uint uiPos = 0;

   while(uiPos < profileSTPtr->lenProfileUI)
   { /*Loop: resort the profile arrays*/
      if(profileSTPtr->indexAryUI[uiPos] != uiPos)
      { /*If: need to swap positions*/
         swap_profile_samClust(
           profStrPtr,
           uiPos,
           profileSTPtr->indexAryUI[uiPos]
         );
      } /*If: need to swap positions*/

      else
         ++uiPos;
   } /*Loop: resort the profile arrays*/
   /*worst case is 2n, but requires presorted index*/
} /*resort_profile_clustSam*/


/*-------------------------------------------------------\
| Fun10: allocRead_profile_clustSam
|   - allocates memory (or makes sure have enough memory)
|     for a single reads profile
| Input:
|   - profSTPtr:
|     o pointer to profile_clustSam struct to check if
|       has enough memory (if not allocate)
|   - numVarUI:
|     o expected number of variants
|     o you can find this with dist_edDist
|       (../genLib/edDist.h; fun02)
| Output:
|   - Modifies:
|     o all array related variables to be updated to the
|       new size if more memory was needed
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
allocRead_profile_clustSam(
   struct profile_samClust *profSTPtr,
   unsigned int numVarUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun10 sec01:
   '     - allocate memroy for arrays (if needed)
   '   o fun10 sec02:
   '     - return errors (or lack of errors)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - allocate memroy for arrays (if needed)
   ^   o fun10 sec01 sub01:
   ^     - variant position and length array allocation
   ^   o fun10 sec01 sub02:
   ^     - variant type and nucleotide arrays
   ^   o fun10 sec01 sub03:
   ^     - read depth arrays
   ^   o fun10 sec01 sub04:
   ^     - index array for resorting/profile extraction
   ^   o fun10 sec01 sub05:
   ^     - history (depth and selected profile) arrays
   ^   o fun10 sec01 sub06:
   ^     - set array length
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec01 Sub01:
   *   - variant position and length array allocation
   \*****************************************************/

   if(profSTPtr->lenProfileUI < numVarUI)
   { /*If: need to allocate memory*/

      if(profSTPtr->posAryUI)
         free(profSTPtr->posAryUI);

      profSTPtr->posAryUI = 0;

      profSTPtr->posAryUI =
         calloc(
            numVarUI,
            sizeof(uint)
         );

      if(! profSTPtr->posAryUI)
         goto memErr_fun10_sec03;


      if(profSTPtr->lenAryUI)
         free(profSTPtr->lenAryUI);

      profSTPtr->lenAryUI = 0;

      profSTPtr->lenAryUI =
         calloc(
            numVarUI,
            sizeof(uint)
         );

      if(! profSTPtr->lenAryUI)
         goto memErr_fun10_sec03;

      /**************************************************\
      * Fun10 Sec01 Sub02:
      *   - variant type and nucleotide arrays
      \**************************************************/

      if(profSTPtr->baseAryUC)
         free(profSTPtr->baseAryUC);

      profSTPtr->baseAryUC = 0;

      profSTPtr->baseAryUC =
         calloc(
            numVarUI,
            sizeof(uchar)
         );

      if(! profSTPtr->baseAryUC)
         goto memErr_fun10_sec03;


      if(profSTPtr->typeArySC)
         free(profSTPtr->typeArySC;

      profSTPtr->typeArySC = 0;

      profSTPtr->typeArySC =
         calloc(
            numVarUI,
            sizeof(schar)
         );

      if(! profSTPtr->typeArySC)
         goto memErr_fun10_sec03;

      /**************************************************\
      * Fun10 Sec01 Sub03:
      *   - read depth arrays
      \**************************************************/

      if(profSTPtr->supAryUI)
         free(profSTPtr->supAryUI);

      profSTPtr->supAryUI = 0;

      profSTPtr->supAryUI =
         calloc(
            numVarUI,
            sizeof(uint)
         );

      if(! profSTPtr->supAryUI)
         goto memErr_fun10_sec03;


      if(profSTPtr->mapAryUI)
         free(profSTPtr->mapAryUI);

      profSTPtr->mapAryUI = 0;

      profSTPtr->mapAryUI =
         calloc(
            numVarUI,
            sizeof(uint)
         );

      if(! profSTPtr->mapAryUI)
         goto memErr_fun10_sec03;

      /**************************************************\
      * Fun10 Sec01 Sub04:
      *   - index array for resorting/profile extraction
      \**************************************************/

      if(profSTPtr->mapAryUI)
         free(profSTPtr->mapAryUI);

      profSTPtr->mapAryUI = 0;

      profSTPtr->mapAryUI =
         calloc(
            numVarUI,
            sizeof(uint)
         );

      if(! profSTPtr->mapAryUI)
         goto memErr_fun10_sec03;

      profSTPtr->lenProfileUI = numVarUI;

      /**************************************************\
      * Fun10 Sec01 Sub05:
      *   - history (depth and selected profile) arrays
      \**************************************************/

      if(profSTPtr->cntVarAryUI)
         free(profSTPtr->cntVarAryUI);

      profSTPtr->cntVarAryUI = 0;

      profSTPtr->cntVarAryUI =
            calloc(
               numVarUI,
               sizeof(uint)
            );

      if(! profSTPtr->cntVarAryUI)
         goto memErr_fun10_sec03;


      if(profSTPtr->depthVarAryUI)
         free(profSTPtr->depthVarAryUI);

      profSTPtr->depthVarAryUI = 0;

      profSTPtr->depthVarAryUI =
            calloc(
               numVarUI,
               sizeof(uint)
            );

      if(! profSTPtr->depthVarAryUI)
         goto memErr_fun10_sec03;

      /**************************************************\
      * Fun10 Sec01 Sub06:
      *   - set array length
      \**************************************************/

      profSTPtr->histIndexUI = 0;
      profSTPtr->lenProfileUI = numVarUI;
   } /*If: need to allocate memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - return errors (or absence of errors)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun10_sec03:;
   return def_memErr_clustSam;
} /*allocRead_profile_clustSam*/

/*-------------------------------------------------------\
| Fun11: build_profile_clustSam
|   - builds a profile for a profile_clustSam struct
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to hold profile
|   - samSTPtr:
|     o pointer to samEntry struct with read to profile
|   - minIndelLenUI:
|     o minimum indel length to keep indel in profile
|   - minQUC:
|     o minimum q-score to keep snp in profile
|   - newRegBl:
|     o 1: mapping to new region (remove region discards)
|     o 0: keep region discard flags
| Output:
|   - Modifies:
|     o profSTPtr to have varaint profile for samSTPtr
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
build_profile_clustSam(
   struct profile_samClust *profSTPtr
   struct samEntry *samSTPtr,
   unsigned int minIndelLenUI,
   unsigned char minQUC,
   signed char newRegBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - builds a profile for a profile_clustSam struct
   '   o fun11 sec01:
   '     - variable declaratons
   '   o fun11 sec02:
   '     - memory allocation and profile setup
   '   o fun11 sec03:
   '     - build profile
   '   o fun11 sec04:
   '     - sort by depth and setup index's (resorting)
   '   o fun11 sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    schar errSC = 0;

    uint seqPosUI = 0;
    uint cigPosUI = 0;
    uint tmpUI = 0;
    uchar qUC = 0;

    uint profPosUI = 0;
    uint refPosUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - memory allocation and profile setup
   ^   o fun11 sec02 sub01:
   ^     - allocate memory (if needed) for profile
   ^   o fun11 sec02 sub02:
   ^     - set up profile meta data
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   profPosUI =
      (uint)
      dist_edDist(
         samSTPtr,
         minINdelLenUI,
         minQUC,
         &refPosUI, /*number indels (only want distance)*/
         &tmpUI     /*do not care about indel events*/
      ); /*get number of variants*/
         /*dist_edDist uses same filters as fun11*/

   refPosUI = 0;

   errSC =
      allocRead_profile_clustSam(
         profSTPtr,
         profPosUI
      );

   if(errSC)
      goto memErr_fun11_sec05_sub02;

   /*****************************************************\
   * Fun11 Sec02 Sub02:
   *   - set up profile metat data
   \*****************************************************/

   errSC = def_blankProfile_clustSam;

   if(newRegBl)
     errSC |= def_blankRegion_clustSam;

   blank_profile_clustSam(
      profSTPtr,
      errSC
   );

   errSC = 0;

   lenCp_ulCp(
      profSTPtr->refIdStr,
      samSTPtr->refIdStr,
      samSTPtr->lenRefIdUC
   ); /*copy reference id for later checks*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - build profile
   ^   o fun11 sec03 sub01:
   ^     - skip over matches/masked bases + start loop
   ^   o fun11 sec03 sub02:
   ^     - add snps to profile
   ^   o fun11 sec03 sub03:
   ^     - add deletions to profile
   ^   o fun11 sec03 sub04:
   ^     - add insertoins to profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - skip over matches/masked bases + start loop
   \*****************************************************/

   for(
      cigPosUI = 0;
      cigPosUI < samSTPtr->lenCigUI;
      ++cigPosUI
   ){ /*Loop: build profile*/
      switch(samSTPtr->cigTypeStr[cigPosUI])
      { /*Switch: check cigar type*/
         case 'M': /*no way to check if match or snp*/
         case '=':
         /*Case: match*/
            seqPosUI += samSTPtr->cigArySI[cigPosUI];
            refPosUI += samSTPtr->cigArySI[cigPosUI];
            break;
         /*Case: match*/

         case 'S':
         /*Case: soft masked*/
            seqPosUI += samSTPtr->cigArySI[cigPosUI];
            break;
         /*Case: soft masked*/

         /***********************************************\
         * Fun11 Sec03 Sub02:
         *   - add snps to profile
         \***********************************************/

         case 'X':
         /*Case: snp*/
            tmpSI = samSTPtr->cigArySI[cigPosUI];

            while(tmpSI > 0)
            { /*Loop: find number of good SNPs*/
               qUC = (uchar) samSTPtr->qStr[seqPosUI];
               qUC -= def_adjQ_samEntry;

               if(qUC >= minQUC)
               { /*If: keeping SNP*/
                  profSTPtr->posAryUI[profPosUI]=refPosUI;
                  profSTPtr->lenAryUI[profPosUI] = 1;
                  profSTPtr->typeArySC[profPosUI] = 'X';

                  profSTPtr->baseAryUC[profPosUI] =
                       samSTPtr->seqStr[seqPosUI]
                     & def_ntToCode_alnSet;

                  ++profPosUI;
               } /*If: keeping SNP*/

               ++seqPosUI;
               ++refPosUI;
               --tmpSI;
            } /*Loop: find number of good SNPs*/

            break;
         /*Case: snp*/

         /***********************************************\
         * Fun11 Sec03 Sub03:
         *   - add deletions to profile
         \***********************************************/

         case 'D':
         /*Case: deletion*/
            tmpSI = samSTPtr->cigArySI[cigPosUI]

            if(
                  samSTPtr->cigArySI[cigPosUI]
               >= minIndelLenUI
            ){ /*If: deletion large enough to keep*/
               while(
               { /*Loop: add deletion to array*/
                  profSTPtr->posAryUI[profPosUI]=refPosUI;
                  profSTPtr->lenAryUI[profPosUI] = tmpSI;
                  profSTPtr->typeArySC[profPosUI] = 'D';

                  profSTPtr->baseAryUC[profPosUI] =
                     def_del_clustSam;

                  ++profPosUI;
                  ++refPosUI;
                  --tmpSI;
               } /*Loop: add deletion to array*/
            } /*If: deletion large enough to keep*/

            refPosUI += samSTPtr->cigArySI[cigPosUI];
            break;
         /*Case: deletion*/

         /***********************************************\
         * Fun11 Sec03 Sub04:
         *   - add insertions to profile
         \***********************************************/

         case 'I':
         /*Case: insertion*/
            tmpSI = samSTPtr->cigArySI[cigPosUI]

            if(tmpSI >= minIndelLenUI)
            { /*If: insertion large enough to keep*/

               while(
               { /*Loop: add insertion to array*/
                  profSTPtr->posAryUI[profPosUI]=refPosUI;
                  profSTPtr->lenAryUI[profPosUI] = tmpSI;
                  profSTPtr->typeArySC[profPosUI] = 'I';

                  profSTPtr->baseAryUC[profPosUI] =
                       samSTPtr->seqStr[seqPosUI]
                     & def_ntToCode_alnSet;

                  ++profPosUI;
                  ++seqPosUI;
                  --tmpSI;
               } /*Loop: add insertion to array*/
            } /*If: insertion large enough to keep*/

            break;
         /*Case: insertion*/

         /*everything else is hard masked, skip over*/
      } /*Switch: check cigar type*/
   } /*Loop: build profile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - sort by depth and setup index's (resorting)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortPos_profile_samClust(profSTPtr);

   for(
      profPosUI = 0;
      profPosUI < profileSTPtr->lenProfileUI;
      ++profPosUI
   ) profileSTPtr->indexAryUI[profPosUI] = profPosUI;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec05:
   ^   - clean up
   ^   o fun11 sec05 sub01:
   ^     - no error clean up
   ^   o fun11 sec05 sub02:
   ^     - memory error clean up
   ^   o fun11 sec05 sub03:
   ^     - return result (all call)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec05 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto ret_fun11_sec05_sub03;

   /*****************************************************\
   * Fun11 Sec05 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_fun11_sec05_sub02:;

   errSC =  def_memErr_clustSam;

   if(profSTPtr->baseAryUC)
      free(profSTPtr->baseAryUC);

   profSTPtr->baseAryUC = 0;


   if(profSTPtr->lenAryUI)
      free(profSTPtr->lenAryUI);

   profSTPtr->lenAryUI = 0;


   if(profSTPtr->supAryUI)
      free(profSTPtr->supAryUI);

   profSTPtr->supAryUI = 0;


   if(profSTPtr->mapAryUI)
      free(profSTPtr->mapAryUI);

   profSTPtr->mapAryUI = 0;


   profSTPtr->lenProfileUI = 0;

   goto ret_fun11_sec05_sub03;

   /*****************************************************\
   * Fun11 Sec05 Sub03:
   *   - return result (all call)
   \*****************************************************/

   ret_fun11_sec05_sub03:;
   return errSC;
} /*build_profile_clustSam*/

/*-------------------------------------------------------\
| Fun12: addReadTo_profile_clustSam
|   - adds a read to a profile_clustSam struct
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to hold profile
|   - samSTPtr:
|     o pointer to samEntry struct with read to add
|   - readOnUL:
|     o read currently on in profile
|   - minIndelLenUI:
|     o minimum indel length to keep indel in profile
|   - minQUC:
|     o minimum q-score to keep snp in profile
|   - minPercCovF:
|     o minimum percent of read covering profile
|   - alnSetSTPtr:
|     o has alignment settings for insertion regions
| Output:
|   - Modifies:
|     o profSTPtr to have read depth and mapped reads
|       updated
|     o readHitArySI[readOnUL] to be set to
|       def_profileDiscard_clustSam if read failed checks
|   - Returns:
|     o 0 for no errors
|     o def_noMap_clustSam if read not in profile
|     o def_discarded_clustSam if read was marked as
|       discarded
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
addReadTo_profile_clustSam(
   struct profile_samClust *profSTPtr
   struct samEntry *samSTPtr,
   unsigned long readOnUL,
   unsigned int minIndelLenUI,
   unsigned char minQUC,
   float minPercCovF,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC:
   '   - adds a read to a profile_clustSam struct
   '   o fun12 sec01:
   '     - variable declartions
   '   o fun12 sec02:
   '     - ovlerap and other checks
   '   o fun12 sec03:
   '     - add read to profile
   '   o fun12 sec04:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;

   uint seqPosUI = 0;
   uint cigPosUI = 0;
   uint tmpUI = 0;
   uchar qUC = 0;
   schar indexBl = 0;

   uint profPosUI = 0;
   uint refPosUI = 0;
   slong scoreSL = 0;

   struct seqST qryNoFreeST;
   struct seqST profNoFreeST;
   struct dirMatrix matrixStackST;
   schar *cigTypeHeapStr = 0;
   sint *cigHeapArySI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - overlap and other checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_seqST(&qryNoFreeST);
   init_seqST(&profNoFreeST);
   init_dirMatrix(&matrixStackST);

   if(profSTPtr->readHitArySI[readOnUL] < 0)
      goto discardedRead_fun12_sec04;

   if(samSTPtr->endUI < profSTPtr->profStartUI)
      goto noMap_fun12_sec04;

   if(samSTPtr->startUI > profSTPtr->profEndUI)
      goto noMap_fun12_sec04;

   if(
      eql_charCp(
         profSTPtr->refIdStr,
         samSTPtr->refIdStr,
         (schar) '\0'
      )
   ) goto noMap_fun12_sec04;
     /*read mapped to different reference than profile*/

   seqPosUI =
      min_genMath(
         samSTPtr->refEndUI,
         profSTPtr->profEndUI
   ); /*find end of overlap*/

   seqPosUI -= profSTPtr->profSartUI;

   tmpUI = profSTPtr->profEndUI;
   tmpUI -= profSTPtr->profSartUI;

   if((float) seqPosUI / (float) tmpUI) < minPercCovF)
      goto noMap_fun12_sec04;
      /*read does not cover enough of profile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - move to first base in profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


   cigTypeHeapStr =
      calloc(
         samSTPtr->readLenUI,
         sizeof(schar)
      );

   if(! cigTypeHeapStr)
      goto memErr_fun12_sec05;


   cigHeapArySI =
      calloc(
         samSTPtr->readLenUI,
         sizeof(sint)
      );

   if(! cigHeapArySI)
      goto memErr_fun12_sec05;

   seqToIndex_alnSet(samSTPtr->seqStr);
   indexBl = 1;

   while(profSTPtr->baseAryUC[profPosUI] == '\0')
   { /*Loop: move to first profile position*/
      ++profPosUI;

      if(profPosUI >= profSTPtr->lenProfileUI)
         goto done_fun12_sec05; /*nothing in profile*/
   } /*Loop: move to first profile position*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec04:
   ^   - add read to profile
   ^   o fun12 sec04 sub01:
   ^     - skip over matches/masked bases + start loop
   ^   o fun12 sec04 sub02:
   ^     - add snps to profile; if match profile
   ^   o fun12 sec04 sub03:
   ^     - add deletions to profile; if matches profile
   ^   o fun12 sec04 sub04:
   ^     - add insertions to profile; if matches profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec04 Sub01:
   *   - skip over matches/masked bases + start loop
   \*****************************************************/

   for(
      cigPosUI = 0;
      cigPosUI < samSTPtr->lenCigUI;
      ++cigPosUI
   ){ /*Loop: build profile*/
      switch(samSTPtr->cigTypeStr[cigPosUI])
      { /*Switch: check cigar type*/
         case 'M': /*no way to check if match or snp*/
         case '=':
         /*Case: match*/
            seqPosUI += samSTPtr->cigArySI[cigPosUI];
            refPosUI += samSTPtr->cigArySI[cigPosUI];

            while(
               refPosUI > profSTPtr->posAryUI[profPosUI]
            ){ /*Loop: move past old profile entries*/
               ++profileSTPtr->mapAryUI[profPosUI];
               ++profPosUI;

               if(profPosUI > profSTPtr->lenProfileUI)
                  goto done_fun12_sec05;
            } /*Loop: move past old profile entries*/

            break;
         /*Case: match*/

         case 'S':
         /*Case: soft masked*/
            seqPosUI += samSTPtr->cigArySI[cigPosUI];
            break;
         /*Case: soft masked*/

         /***********************************************\
         * Fun12 Sec04 Sub02:
         *   - add snps to profile
         *   o fun12 sec04 sub02 cat01:
         *     - check if profile supports snps at pos
         *   o fun12 sec04 sub02 cat02:
         *     - move past insertions + start snp loop
         *   o fun12 sec04 sub02 cat03:
         *     - check if keeping SNP
         *   o fun12 sec04 sub02 cat04:
         *     - move to next profile item
         *   o fun12 sec04 sub02 cat05:
         *     - finished with SNPs
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun12 Sec04 Sub02 Cat01:
         +   - check if profile supports snps at position
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'X':
         /*Case: snp*/
            tmpSI = samSTPtr->cigArySI[cigPosUI];

            if(
                 refPosUI + tmpSI
               < profSTPtr->posAryUI[profPosUI]
            ) goto skipSNP_fun12_sec04_sub02_cat05;
              /*profile does not have SNPs at positions*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub02 Cat02:
            +   - move past insertions + start snp loop
            \+++++++++++++++++++++++++++++++++++++++++++*/

            while(tmpSI > 0)
            { /*Loop: find number of good SNPs*/

               while(
                  profSTPtr->typeArySC[profPosUI] == 'I'
               ){ /*Loop: move past insertions*/
                  ++profileSTPtr->mapAryUI[profPosUI];
                  ++profPosUI;

                  if(profPosUI > profSTPtr->lenProfileUI)
                     goto done_fun12_sec05; /*done*/
               } /*Loop: move past insertions*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun12 Sec04 Sub02 Cat03:
               +   - check if keeping SNP
               \++++++++++++++++++++++++++++++++++++++++*/

               qUC = (uchar) samSTPtr->qStr[seqPosUI];
               qUC -= def_adjQ_samEntry;

               errSC =
                  getMatch_alnSet(
                     samSTPtr->seqStr[seqPosUI],
                     profPosUI->baseAryUC[profPosUI]
                  );

               if(
                 refPosUI !=profPosUI->posAryUI[profPosUI]
               ) ; /*no support for position*/

               else if(
                  profPosUI->typeArySC[profPosUI] != 'X'
               ) ; /*no snp at position*/

               else if(qUC < minQUC) ;
                  /*quality score is to low*/

               else if(errSC != def_ntEql_alnDefs) ;
                   /*not a match; ignore*/

               else
               { /*Else: keeping SNP*/
                  ++profSTPtr->supAryUI[profPosUI];
                  ++profPosUI;
               } /*Else: keeping SNP*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun12 Sec04 Sub02 Cat04:
               +   - move to next profile item
               \++++++++++++++++++++++++++++++++++++++++*/

               ++seqPosUI;
               ++refPosUI;
               --tmpSI;

               while(
                 refPosUI > profSTPtr->posAryUI[profPosUI]
               ){ /*Loop: make sure on next profile*/
                  ++profileSTPtr->mapAryUI[profPosUI];
                  ++profPosUI;

                  if(profPosUI > profSTPtr->lenProfileUI)
                     goto done_fun12_sec05;
               } /*Loop: make sure on next profile*/
            } /*Loop: find number of good SNPs*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub02 Cat05:
            +   - finished with SNPs
            \+++++++++++++++++++++++++++++++++++++++++++*/

            skipSNP_fun12_sec04_sub02_cat05:;

            /*account for ending loop early*/
            seqPosUI += tmpSI;
            refPosUI += tmpSI;

            while(
               refPosUI > profSTPtr->posAryUI[profPosUI]
            ){ /*Loop: set up for next entry*/
               ++profileSTPtr->mapAryUI[profPosUI];
               ++profPosUI;

               if(profPosUI > profSTPtr->lenProfileUI)
                  goto done_fun12_sec05;
            } /*Loop: set up for next entry*/

            break;
         /*Case: snp*/

         /***********************************************\
         * Fun12 Sec04 Sub03:
         *   - add deletions to profile
         *   o fun12 sec04 sub03 cat01:
         *     - check if keeping deletion
         *   o fun12 sec04 sub03 cat02:
         *     - move past insertions + start del loop
         *   o fun12 sec04 sub03 cat03:
         *     - if keeping deletion add to count
         *   o fun12 sec04 sub03 cat04:
         *     - move to next item in profile
         *   o fun12 sec04 sub03 cat05:
         *     - finished with deletion entry
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun12 Sec04 Sub03 Cat01:
         +   - check if keeping deletion
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'D':
         /*Case: deletion*/
            tmpSI = samSTPtr->cigArySI[cigPosUI];

            if(tmpSI < (sint) minIndelLenUI)
               goto skipDel_fun12_sec04_sub03_cat05;

            if(
                 refPosUI + tmpSI
               < profSTPtr->posAryUI[profPosUI]
            ) goto skipDel_fun12_sec04_sub03_cat05;
              /*profile has no dels at positions*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub03 Cat02:
            +   - move past insertions + start del loop
            \+++++++++++++++++++++++++++++++++++++++++++*/

            while(tmpSI > 0)
            { /*Loop: add deletion to profile*/

               while(profSTPtr->typeArySC[profPosUI]=='I')
               { /*Loop: move past insertions*/
                  ++profileSTPtr->mapAryUI[profPosUI];
                  ++profPosUI;

                  if(profPosUI > profSTPtr->lenProfileUI)
                     goto done_fun12_sec05; /*done*/
               } /*Loop: move past insertions*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun12 Sec04 Sub03 Cat03:
               +   - if keeping deletion add to count
               \++++++++++++++++++++++++++++++++++++++++*/

               if(
                 profSTPtr->posAryUI[profPosUI] !=refPosUI
               ) ; /*reference position not in profile*/

               else if(
                  profSTPtr->typeArySC[profPosUI] != 'D'
               ) ; /*no deletions in profile*/

               else
               { /*Else: deletion is in profile*/
                  ++profSTPtr->supAryUI[profPosUI];
                  ++profPosUI;
               } /*Else: deletion is in profile*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun12 Sec04 Sub03 Cat04:
               +   - move to next item in profile
               \++++++++++++++++++++++++++++++++++++++++*/

               ++refPosUI;
               ++tmpSI;

               while(
                    refPosUI
                  > profSTPtr->posAryUI[profPosUI]
               ){ /*Loop: make sure on next profile*/
                  ++profileSTPtr->mapAryUI[profPosUI];
                  ++profPosUI;

                  if(profPosUI > profSTPtr->lenProfileUI)
                     goto done_fun12_sec05;
               } /*Loop: make sure on next profile*/
            } /*Loop: add deletion to profile*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub03 Cat05:
            +   - finished with deletion entry
            \+++++++++++++++++++++++++++++++++++++++++++*/

            skipDel_fun12_sec04_sub03_cat05:;

            refPosUI += tmpSI; /*account for early end*/

            while(
                 refPosUI
               > profSTPtr->posAryUI[profPosUI]
            ){ /*Loop: set up for next entry*/
               ++profileSTPtr->mapAryUI[profPosUI];
               ++profPosUI;

               if(profPosUI > profSTPtr->lenProfileUI)
                  goto done_fun12_sec05;
            } /*Loop: set up for next entry*/

            break;
         /*Case: deletion*/

         /***********************************************\
         * Fun12 Sec04 Sub04:
         *   - add insertions to profile
         *   o fun12 sec04 sub03 cat01:
         *     - check if keeping insertion
         *   o fun12 sec04 sub03 cat02:
         *     - check single insertion case
         *   o fun12 sec04 sub03 cat03:
         *     - align insertion (more than 1)
         *   o fun12 sec04 sub03 cat04:
         *     - check alignment cigar for matches
         *   o fun12 sec04 sub03 cat05:
         *     - finished with insertion entry
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun12 Sec04 Sub04 Cat01:
         +   - check if keeping insertion
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'I':
         /*Case: insertion*/
            tmpSI = samSTPtr->cigArySI[cigPosUI];

            if(tmpSI < (sint) minIndelLenUI)
               goto skipIns_fun12_sec04_sub04_cat05;

            if(refPosUI < profSTPtr->posAryUI[profPosUI])
               goto skipIns_fun12_sec04_sub04_cat05;
               /*profile has no ins at positions*/

            if(profSTPtr->typeAryUI[profPosUI] != 'I')
               goto skipIns_fun12_sec04_sub04_cat05;
               /*profile; no insertions at position*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub03 Cat02:
            +   - check single insertion case
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(
                  profSTPtr->lenAryUI[profPosUI] == 1
               && tmpSI == 1
            ){ /*If: insertion is one base long*/
               errSC =
                  getMatch_alnSet(
                     samSTPtr->seqStr[seqPosUI],
                     profPosUI->baseAryUC[profPosUI]
                  );

               if(errSC == def_ntEql_alnDefs)
                  ++profSTPtr->supAryUI[profPosUI];

               ++profileSTPtr->mapAryUI[profPosUI];
               ++profPosUI;
               goto skipIns_fun12_sec04_sub04_cat05;
            } /*If: insertion is one base long*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub03 Cat03:
            +   - align insertion (more than 1)
            \+++++++++++++++++++++++++++++++++++++++++++*/

            qryNoFreeST.seqStr =
               &samSTPtr->seqStr[seqPosUI];

            refNoFreeST.seqStr =
               &profSTPtr->baseAryUC[profPosUI];

            qryNoFreeST.offsetUL = 0;
            qryNoFreeST.endAlnUL = tmpSI - 1;

            refNoFreeST.offsetUL = 0;

            refNoFreeST.endAlnUL =
                 profSTPtr->lenAryUI[profPosUI] - 1;

            scoreSL =
               needle(
                  &qryNoFreeST,
                  &profNoFreeST,
                  &matrixStackST,
                  alnSetSTPtr
               );

            if(matrixStackST.errSC == def_memErr_needle)
                goto memErr_fun12_sec05;

            tmpSI = samSTPtr->readLenUI;

            scoreSL =
               getCig_dirMatrix(
                  &matrixStackST,
                  0, /*use index in matrixStackST*/
                  0, /*forward (not reverse) read*/
                  &qryNoFreeST,
                  &profNoFreeST,
                  &cigTypeHeapStr,
                  &cigHeapArySI,
                  0,            /*start cigar at index 0*/
                  (uint *) &tmpSI,
                  &((uint *) scoreSL)[0], /*discarding*/
                  &((uint *) scoreSL)[1], /*discarding*/
                  0, /*no del possible at end*/
                  alnSetSTPtr
               );

            /*no longer need*/
            qryNoFreeST.seqStr = 0;
            qryNoFreeST.offsetUL = 0;
            qryNoFreeST.endAlnUL = 0;

            posNoFreeST.seqStr = 0;
            posNoFreeST.offsetUL = 0;
            posNoFreeST.endAlnUL = 0;

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub03 Cat04:
            +   - check alignment cigar for matches
            \+++++++++++++++++++++++++++++++++++++++++++*/

            for(
               tmpSI = 0;
               tmpSI < scoreSL;
               ++tmpSI
            ){ /*Loop: compare alignments*/
               switch(cigTypeHeapStr[tmpSI])
               { /*Switch: check cigar*/
                  case '=':
                  /*Case: match*/
                      qUC =
                        (uchar) samSTPtr->qStr[seqPosUI];

                      qUC -= def_adjQ_samEntry;

                      if(qUC >= minQUC)
                      { /*If: keeping match*/
                         profSTPtr->supAryUI[profPosUI]
                           += cigHeapArySI[tmpSI];
                      } /*If: keeping match*/

                      ++profileSTPtr->mapAryUI[
                         profPosUI
                      ];

                      ++profPosUI;
                      break;
                  /*Case: match*/

                  case 'X':
                  case 'D':
                  /*Case: SNP or deleltion*/
                      ++profileSTPtr->mapAryUI[
                         profPosUI
                      ];

                      ++profPosUI;
                      break;
                  /*Case: SNP or deleltion*/

                  case 'I':
                  /*Case: insertion*/
                      break;
                  /*Case: insertion*/

                  /*needle man does not soft mask*/
               } /*Switch: check cigar*/
            } /*Loop: compare alignments*/

            /*for moving to next entry at end*/
            tmpSI = samSTPtr->cigArySI[cigPosUI];

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec04 Sub03 Cat05:
            +   - finished with insertion entry
            \+++++++++++++++++++++++++++++++++++++++++++*/

            skipIns_fun12_sec04_sub04_cat05:;

            seqPosUI += tmpSI;

            while(profSTPtr->typeArySC[profPosUI] =='I')
            { /*Loop: move past unused insertions*/
               ++profileSTPtr->mapAryUI[profPosUI];
               ++profPosUI;

               if(profPosUI > profSTPtr->lenProfileUI)
                  goto done_fun12_sec05; /*done*/
            } /*Loop: move past unused insertions*/

            break;
         /*Case: insertion*/

         /*everything else is hard masked, skip over*/
      } /*Switch: check cigar type*/
   } /*Loop: build profile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun12_sec05:;
   errSC = 0;
   goto ret_fun12_sec05;

   noMap_fun12_sec05:;
   errSC = def_noMap_samClust;

   profSTPtr->readHitArySI[readOnUL] =
      def_profileDiscard_clustSam;

   goto ret_fun12_sec05;

   discardedRead_fun12_sec04:;
   errSC = def_discarded_clustSam;
   goto ret_fun12_sec05;

   memErr_fun12_sec05:;
   errSC = def_memErr_smaClust;
   goto ret_fun12_sec05;

   ret_fun12_sec05:;

   if(cigTypeHeapStr)
      free(cigTypeHeapStr);

   cigTypeHeapStr = 0;

   if(cigHeapArySI)
      free(cigHeapArySI);

   cigHeapArySI = 0;

   if(indexBl)
      indexToSeq(samSTPtr->seqStr);

   indexBl = 0;

   freeStack_dirMatrix(&matrixStackST);

   return errSC;
} /*addReadTo_profile_clustSam*/

/*-------------------------------------------------------\
| Fun13: bust_profile_samClust
|   - finds variants in profile to extract reads with
| Input:
|   - profSTPtr:
|     o pointer to profile_clustSam struct to bust
|       (get minimum profile for)
|   - minDepthUI:
|     o minimum depth before going bust
| Output:
|   - Modifies:
|     o profSTPtr:
|       o sorts profile arrays in profSTPtr by depth
|       o cntAryUI to have number variants in profile
|       o histAryUI to have when variant index first
|         appeared
|       o varAryUI to have varaint index's (profile)
|       o histIdnexUC to be on next history entry
\-------------------------------------------------------*/
void
bust_profile_samClust(
   struct profile_clustSam *profSTPtr,
   unsigned int minDepthUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   - finds variants in profile to extract reads with
   '   o fun13 sec01:
   '     - variable declarations
   '   o fun13 sec02:
   '     - sort by depth and get past full supported vars
   '   o fun13 sec03:
   '     - find variant go bust at (to low read depth)
   '   o fun13 sec04:
   '     - return index of last variant before bust
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint uiPos = 0;
   uint depthUI = 0;
   uint oldDepthUI = 0;
   uint oldPosUI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - sort by depth and get past full supported vars
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortDepth_profile_samClust(profSTPtr);
   ++profSTPtr->histIndexUI;

   while(
          profSTPtr->supAryUI[uiPos] ==
       == profSTPtr->mapAryUI[uiPos]
   ){ /*Loop: past fully supported positions*/
      ++uiPos;

      depthUI =
         min_genMath(
            profSTPtr->mapAryUI[uiPos],
            depthUI
         );

      if(
         profSTPtr->histAryUI[
              profSTPtr->indexAryUI[uiPos]
         ] < 0
      ){ /*If: variants first time*/
         profSTPtr->histAryUI[
           profSTPtr->indexAryUI[uiPos]
         ] = profSTPtr->histIndexUI;
      } /*If: variants first time*/
   } /*Loop: past fully supported positions*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - find variant go bust at (to low read depth)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(depthUI > minDepthUI)
   { /*Loop: find maximum number of variants*/
      depthUI =
         min_genMath(
            profSTPtr->mapAryUI[uiPos],
            depthUI
         );

      oldDepthUI = depthUI;
      oldPosUI = uiPos;

      depthUI =
         (uint)
         (
            (float) depthUI *
            (
                 (float) profSTPtr->supAryUI
               / (float) profSTPtr->mapAryUI
            )
         ); /*find updated depth*/

      if(
         profSTPtr->histAryUI[
              profSTPtr->indexAryUI[uiPos]
         ] < 0
      ){ /*If: variants first time*/
         profSTPtr->histAryUI[
           profSTPtr->indexAryUI[uiPos]
         ] = profSTPtr->histIndexUI;
      } /*If: variants first time*/

      ++uiPos;

      if(uiPos >= profSTPtr->lenProfileUI)
         break; /*no more variants*/
   } /*Loop: find maximum number of variants*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec04:
   ^   - return index of last variant before bust
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(depthUI < minDepthUI)
      --oldPosUI;

   /*update history with depth and variant count*/
   profSTPtr->deptAryUI[profSTPtr->histIndexUI] = depthUI;
   profSTPtr->cntAryUI[profSTPtr->histIndexUI] = oldPosUI;

   return oldPosUI;
} /*bust_profile_samClust*/

/*-------------------------------------------------------\
| Fun14: boom_profile_samClust
|   - marks (boom) reads that were discarded (did not
|     map to profile)
| Input:
|   - profSTPtr:
|     o pointer to profile_clustSam struct with profile
|       to use (generate with bust_profile_samClust)
|   - minIndelLenUI:
|     o minimum indel length to re-align an indel
|   - minQUC:
|     o minimum q-score to keep a SNP
|   - samSTPtr:
|     o pointer to samEntry struct to read sam file with
|   - buffStr:
|     o pointer to c-string for reading the sam file
|   - lenBuffUL:
|     o pointer to unsigned long with length of buffStr
|   - samFILE:
|     o samFILE to scan for reads
| Output:
|   - Modifies:
|     o samFILE to point to start of file
|     o depthAryUI in profSTPtr to have number reads
|       matching profile
|     o samSTPtr to be blank (used to read files)
|     o buffStr to be larger if needed
|     o lenBuffUL to have new size of buffStr if resized
|   - Returns:
|     o number of reads matching profile
|     o -1 for memory error
\-------------------------------------------------------*/
signed long
boom_profile_samClust(
   struct profile_clustSam *profSTPtr,
   unsigned long minIndelLenUI,
   unsigned char minQUC,
   struct samEntry *samSTPtr,
   signed char **buffStr,
   unsigned long *lenBuffUL,
   void *samFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC:
   '   - marks (boom) reads that were discarded (did not
   '     map to profile)
   '   o fun14 sec01:
   '     - variable declarations
   '   o fun14 sec02:
   '     - initiazation and setup
   '   o fun14 sec03:
   '     - 
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;

   slong numMatchSL = 0;

   uint uiVar = 0; /*varaint in on profile*/
   uint uiHist = profSTPtr->histIndexUI; /*for conveince*/

   uint seqPosUI = 0;
   uint refPosUI = 0;
   uint cigPosUI = 0;
   uint tmpUI = 0;

   struct samEntry samStackST;
   struct seqST qryNoFreeST;
   struct seqST profNoFreeST;
   struct dirMatrix matrixStackST;
   schar *cigTypeHeapStr = 0;
   sint *cigHeapArySI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - initiazation and setup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   resort_profile_clustSam(profSTPtr);

   init_seqST(&qryNoFreeST);
   init_seqST(&profNoFreeST);
   init_dirMatrix(&matrixStackST);

   cigTypeHeapStr =
      calloc(
         samSTPtr->readLenUI,
         sizeof(schar)
      );

   if(! cigTypeHeapStr)
      goto memErr_fun14_sec0x;


   cigHeapArySI =
      calloc(
         samSTPtr->readLenUI,
         sizeof(sint)
      );

   if(! cigHeapArySI)
      goto memErr_fun14_sec0x;

   fseek(
      (FILE *) samFILE,
      0,
      SEEK_SET
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - move past header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      get_samEntry(
         samSTPtr,
         buffStr,
         lenBuffUL,
         samFILE
      );

   if(errSC)
   { /*If: error*/
      if(errSC == def_memErr_samEntry)
         goto memErr_fun14_sec0x;
      else
         goto fileErr_fun14_sec0x;
   } /*If: error*/

   while(errSC)
   { /*Loop: get reads by profile*/
      if(samSTPtr->extraStr[0] == '@')
         break;

      errSC =
         get_samEntry(
            samSTPtr,
            buffStr,
            lenBuffUL,
            samFILE
         );
   } /*Loop: get reads by profile*/

   if(errSC)
   { /*If: error*/
      if(errSC == def_memErr_samEntry)
         goto memErr_fun14_sec0x;
      else
         goto fileErr_fun14_sec0x;
   } /*If: error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec04:
   ^   - check cigar entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

      seqToIndex_alnSet(samSTPtr->seqStr);

   while(errSC)
   { /*Loop: get reads by profile*/
      for(
         cigPosUI = 0;
         cigPosUI < samSTPtr->lenCigUI;
         ++cigPosUI
      ){ /*Loop: check cigar*/
         switch(samSTPtr->cigTypeStr[cigPosUI])
         { /*Switch: profile cigar*/
            case '=':
            /*Case: match to reference*/
               seqPosUI += samSTPtr->cigTypeStr[cigPosUI];
               refPosUI += samSTPtr->cigTypeStr[cigPosUI];

               while(
                 refPosUI > profSTPtr->varAryUI[profPosUI]
               ){ /*Loop: move to next varaint*/
                  ++profPosUI;

                  if(profPosUI >= profSTPtr->lenProfileUI)
                     goto checkProfile_fun14_sec0y_sub0x;
               } /*Loop: move to next varaint*/

               break;
            /*Case: match to reference*/

            case 'X':
            /*Case: SNP to reference*/
               tmpSI = samSTPtr->cigTypeStr[cigPosUI];
                  
               while(tmpSI > 0)
               { /*Loop: find snps in profile*/

                  while(
                       refPosUI
                    > profSTPtr->varAryUI[profPosUI]
                  ){ /*Loop: move to next varaint*/
                     ++profPosUI;

                     if(
                           profPosUI
                        >= profSTPtr->lenProfileUI
                     )goto checkProfile_fun14_sec0y_sub0x;
                  } /*Loop: move to next varaint*/

                  if(profPosUI >= profSTPtr->lenProfileUI)
                     goto checkProfile_fun14_sec0y_sub0x;

                  --tmpSI;
               } /*Loop: find snps in profile*/

               break;
            /*Case: SNP to reference*/
         } /*Switch: profile cigar*/
      } /*Loop: check cigar*/

      checkProfile_fun14_sec0y_sub0x:;

      nextEntry_fun14_sec03_sub0x:;

      errSC =
         get_samEntry(
            samSTPtr,
            buffStr,
            lenBuffUL,
            samFILE
         );
   } /*Loop: get reads by profile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec0x:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun14_sec0x:;
   goto ret_fun14_sec0x;

   memErr_fun14_sec0x:;
   numMatchSL = def_memErr_clustSam;
   goto ret_fun14_sec0x;

   fileErr_fun14_sec0x:;
   numMatchSL = def_fileErr_clustSam;
   goto ret_fun14_sec0x;

   ret_fun14_sec0x:;

   if(cigTypeHeapStr)
      free(cigTypeHeapStr);

   cigTypeHeapStr = 0;

   if(cigHeapArySI)
      free(cigHeapArySI);

   cigHeapArySI = 0;

   if(indexBl)
      indexToSeq(samSTPtr->seqStr);

   indexBl = 0;

   freeStack_dirMatrix(&matrixStackST);

   return numMatchSL;
} /*boom_profile_samClust*/
