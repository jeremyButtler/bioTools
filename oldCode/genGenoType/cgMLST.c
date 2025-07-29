/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cgMLST SOF: Start Of File
'   - functios for finding cgMLST profile for mapped
'     reads/consensus
'   o header:
'     - included libraries
'   o .h st01: allele_cgMLSt
'     - has allele sequences, loci names, and count table
'   o fun01: blank_allele_cgMLST
'     - blanks a cgMLST struct (lazy blank when possible)
'   o fun02: init_allele_cgMLST
'     - initializes a cgMLST struct
'   o fun03: freeStack_allele_cgMLST
'     - frees variables in a allele_cgMLST struct
'   o fun04: freeHeap_allele_cgMLST
'     - frees an allele_cgMLST struct
'   o .c fun05: swap_allele_cgMlst
'     - swaps two alleles in a allele_cgMLST struct
'   o fun06: findStart_allele_cgMLST
'     - finds allele with closest starting coordiante
'   o fun07: sort_allele_cgMLST
'     - sorts loci and alleles by reference start
'   o fun08: getLoci_allele_cgMLST
'     - gets loci for a allele_cgMLST struct
'   o fun09: scanLoci_allele_cgMLST
'     - search loci for best matching allele
'   o fun10: seqToLoci_allele_cgMLST
'     - add all loci in sequence to allele_cgMLST table
'   o fun11: plin_allele_cgMLST
'     - prints lineage for the cgMLST results
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

#include "cgMLST.h"
#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/ptrAry.h"

#include "../genBio/samEntry.h"
#include "../genBio/edDist.h"

/*.h files only*/
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/strAry.h"
!   o .c  #include "../genBio/seqST.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
!   o .h  #include "../genLib/genMath.h" only using macros
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_allele_cgMLST
|   - blanks a cgMLST struct (lazy blank when possible)
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct to blank
| Output:
|   - Modifies:
|     o cntAryUI in alleleSTPtr to be full  of 0's
|     o numAlleleSL in alleleSTPtr to be 0
|     o numLociSL to be 0
\-------------------------------------------------------*/
void
blank_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
){
   signed long slIndex = 0;

   if(! alleleSTPtr)
      return;


   if(alleleSTPtr->cntAryUI)
   { /*If: have a count table to blank*/
      for(
         slIndex = 0;
         slIndex < alleleSTPtr->numAlleleSL;
         ++slIndex
      ){ /*Loop: blank counts*/
         alleleSTPtr->cntAryUI[slIndex] = 0;
         alleleSTPtr->offAryUI[slIndex] = 0;
      } /*Loop: blank counts*/
   } /*If: have a count table to blank*/


   alleleSTPtr->numAlleleSL = 0;
   alleleSTPtr->numLociSL = 0;
} /*blank_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun02: init_allele_cgMLST
|   - initializes a cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct to initialize
| Output:
|   - Modifies:
|     o sets every variable in alleleSTPtr to 0/null
\-------------------------------------------------------*/
void
init_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
){
   if(! alleleSTPtr)
      return;

   alleleSTPtr->cntAryUI = 0;
   alleleSTPtr->offAryUI = 0;
   alleleSTPtr->mapSamST = 0;
   alleleSTPtr->sizeAlleleSL = 0;

   alleleSTPtr->lociIndexArySI = 0;
   alleleSTPtr->lociIdsST = 0;
   alleleSTPtr->sizeLociSL = 0;

   blank_allele_cgMLST(alleleSTPtr);
} /*init_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun03: freeStack_allele_cgMLST
|   - frees variables in an allele_cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct
| Output:
|   - Modifies:
|     o frees all pointers variables and sets everything
|       to 0/null
\-------------------------------------------------------*/
void
freeStack_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
){
   signed long slIndex = 0;

   if(! alleleSTPtr)
      return;

   if(alleleSTPtr->cntAryUI)
      free(alleleSTPtr->cntAryUI);

   if(alleleSTPtr->offAryUI)
      free(alleleSTPtr->cntAryUI);

   if(alleleSTPtr->lociIndexArySI)
      free(alleleSTPtr->lociIndexArySI);

   if(alleleSTPtr->lociIdsST)
      free(alleleSTPtr->lociIdsST);

   if(alleleSTPtr->mapSamST)
   { /*If: have samEntry struct array to free*/
      for(
         slIndex = 0;
         slIndex < alleleSTPtr->sizeAlleleSL;
         ++slIndex
      ){ /*Loop: free all samEntry structs*/
         freeStack_samEntry(
           &alleleSTPtr->mapSamST[slIndex]
         );
      }  /*Loop: free all samEntry structs*/

      free(alleleSTPtr->mapSamST);
   } /*If: have samEntry struct array to free*/
   

   init_allele_cgMLST(alleleSTPtr);
} /*freeStack_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun04: freeHeap_allele_cgMLST
|   - frees an allele_cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct to free
| Output:
|   - Frees:
|     o alleleSTPtr, you must set to null/0
\-------------------------------------------------------*/
void
freeHeap_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
){
   if(! alleleSTPtr)
      return;

   freeStack_allele_cgMLST(alleleSTPtr);
   free(alleleSTPtr);
} /*freeHeap_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun05: swap_allele_cgMlst
|   - swaps two alleles in a allele_cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to alleleSTPtr sturct with alleles to
|       swap
|   - firstSL:
|     o frist allele to swap
|   - secSL:
|     o second allele to swap
| Output:
|   - Modifies:
|     o cntAryUI and mapSamST at fristSL in alleleSTPtr
|       to have secSL values
|     o cntAryUI and mapSamST at secSL in alleleSTPtr
|       to have firstSL values
\-------------------------------------------------------*/
void
swap_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr,/*alleles to swap*/
   signed long firstSL,              /*first swap index*/
   signed long secSL                 /*second swap index*/
){
   swap_samEntry(
      &alleleSTPtr->mapSamST[firstSL],
      &alleleSTPtr->mapSamST[secSL]
   );

   alleleSTPtr->cntAryUI[firstSL] ^=
      alleleSTPtr->cntAryUI[secSL];

   alleleSTPtr->cntAryUI[secSL] ^=
      alleleSTPtr->cntAryUI[firstSL];

   alleleSTPtr->cntAryUI[firstSL] ^=
      alleleSTPtr->cntAryUI[secSL];


   alleleSTPtr->offAryUI[firstSL] ^=
      alleleSTPtr->offAryUI[secSL];

   alleleSTPtr->offAryUI[secSL] ^=
      alleleSTPtr->offAryUI[firstSL];

   alleleSTPtr->offAryUI[firstSL] ^=
      alleleSTPtr->offAryUI[secSL];
} /*swap_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun06: findStart_allele_cgMLST
|   - finds allele with closest starting coordiante
| Input:
|   - alleleSTPtr
|     o pointer to allele_cgMLST struct to search
|   - startUI:
|     o starting coordinate to find
| Output:
|   - Returns:
|     o closest index to target coordiante, you must make
|       sure the closet target is in range.
|     o returns last element if startUI was greater then
|       any element
\-------------------------------------------------------*/
signed long
findStart_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr,/*start coordiantes*/
   unsigned int startUI             /*coordiante to find*/
){
   signed long leftSL = 0;
   signed long rightSL = alleleSTPtr->numAlleleSL - 1;
   signed long midSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: search for coordiante*/
      midSL = (leftSL + rightSL) >> 1;

      if(
           startUI
         > alleleSTPtr->mapSamST[midSL].refStartUI
      ) leftSL = midSL + 1;

      else if(
           startUI
         < alleleSTPtr->mapSamST[midSL].refStartUI
      ) rightSL = midSL - 1;

      else
      { /*Else: found coordiante*/
         if(! midSL)
            return midSL;

         if(
               startUI
            == alleleSTPtr->mapSamST[midSL - 1].refStartUI
         ) rightSL = midSL - 1; /*still have matches*/

         else
            return midSL; /*found first index*/
      } /*Else: found coordiante*/
   } /*Loop: search for coordiante*/

   if(midSL + 1 >= alleleSTPtr->numAlleleSL)
      return midSL;

   if(startUI > alleleSTPtr->mapSamST[midSL].refStartUI)
      ++midSL; /*on the lesser element*/

   return midSL;
} /*findStart_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun07: sort_allele_cgMLST
|   - sorts loci and alleles by reference start
|     coordinate and allele (query) id
| Input:
|   - alleleSTPtr
|     o pointer to allele_cgMLST struct to sort
| Output:
|   - Modifies:
|     o cntAryUI and mapSamST arrays in alleleSTPtr to be
|       sorted by reference start, and then qry id
|       (lociId--alleleNumber)
|     o lociIndexArySI in alleleSTPtr to have new start
|       index's
\-------------------------------------------------------*/
void
sortAllele_cgMLST(
   struct allele_cgMLST *alleleSTPtr /*to sort*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - sorts loci and alleles by reference start
   '     coordinate and allele (query) id
   '   o fun07 sec01:
   '     - variable declerations
   '   o fun07 sec02:
   '     - find the number of roun07 to sort for
   '   o fun07 sec03:
   '     - sort the arrays
   '   o fun07 sec04:
   '     - reassign locus id index's
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   signed long numElmSL = alleleSTPtr->numAlleleSL;

   /*Number of sorting roun07*/
   signed long subSL = 0;
   signed long nextSL = 0;
   signed long lastSL = 0;
   signed long onSL = 0;

   /*Variables to incurment loops*/
   signed long slIndex = 0;
   signed long slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - find the max search value (number roun07 to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmSL)
      return;

   /*Recursion formsla: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSL = 1; /*Initialzie first array*/

   while(subSL < numElmSL - 1)
      subSL = (3 * subSL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - sort arrays
   ^   o fun07 sec03 sub01:
   ^     - start subset loop
   ^   o fun07 sec03 sub02:
   ^     - forward swap (first swap)
   ^   o fun07 sec03 sub03:
   ^     - reverse swap (backwards check)
   ^   o fun07 sec03 sub04:
   ^     - move to next set in subset
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec03 Sub01:
   *   - start subset loop
   \*****************************************************/

   while(subSL > 0)
   { /*Loop: all roun07*/
      for(
         slIndex = 0;
         slIndex <= subSL;
         ++slIndex
      ){ /*Loop: though sub array*/
         slElm = slIndex;

         for(
            slElm = slIndex;
            slElm + subSL < numElmSL;
            slElm += subSL
         ){ /*Loop: swap elements in subarray*/
            nextSL = slElm + subSL;

            /********************************************\
            * Fun07 Sec03 Sub02:
            *   - forward swap (first swap)
            \********************************************/

            if(
                 alleleSTPtr->mapSamST[slElm].refStartUI
               < alleleSTPtr->mapSamST[nextSL].refStartUI
            ) ; /*already positioned*/

            else if(
                 alleleSTPtr->mapSamST[slElm].refStartUI
               > alleleSTPtr->mapSamST[nextSL].refStartUI
            ) goto swapFor_fun07_sec03;

            else if(
               eqlNull_ulCp(
                  alleleSTPtr->mapSamST[slElm].qryIdStr,
                  alleleSTPtr->mapSamST[nextSL].refIdStr
               ) > 0
            ){ /*If I need to swap an element*/
               swapFor_fun07_sec03:;

               swap_allele_cgMLST(
                  alleleSTPtr,
                  slElm,
                  nextSL
               );

               /*****************************************\
               * Fun07 Sec03 Sub03:
               *   - reverse swap (backwards check)
               \*****************************************/

               lastSL = slElm;
               onSL = slElm;

               while(lastSL >= subSL)
               { /*loop: move swapped element back*/
                  lastSL -= subSL;

                  if(
                       alleleSTPtr->mapSamST[
                          onSL
                       ].refStartUI
                     < alleleSTPtr->mapSamST[
                         lastSL
                       ].refStartUI
                  ) ; /*already positioned*/

                  else if(
                       alleleSTPtr->mapSamST[
                          onSL
                       ].refStartUI
                     > alleleSTPtr->mapSamST[
                          lastSL
                       ].refStartUI
                  ) break;

                  else if(
                     eqlNull_ulCp(
                        alleleSTPtr->mapSamST[
                           onSL
                        ].qryIdStr,
                        alleleSTPtr->mapSamST[
                           lastSL
                        ].refIdStr
                     ) > 0
                  ) break;

                  swap_allele_cgMLST(
                     alleleSTPtr,
                     onSL,
                     lastSL
                  );

                  onSL = lastSL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      /**************************************************\
      * Fun07 Sec03 Sub04:
      *   - move to next set in subset
      \**************************************************/

      subSL = (subSL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - reassign locus id index's
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      onSL = 0;
      onSL < alleleSTPtr->numLociSL;
      ++onSL
   ){ /*Loop: sort loci index by start*/
      alleleSTPtr->lociIndexArySI[onSL] =
         findStart_allele_cgMLST(
            alleleSTPtr,
            alleleSTPtr->lociStartAryUI[onSL]
         ); /*find new start*/
   }  /*Loop: sort loci index by start*/

} /*sortAllele_cgMLST*/

/*-------------------------------------------------------\
| Fun08: getLoci_allele_cgMLST
|   - gets loci for a allele_cgMLST struct
| Input:
|   - lociMapFILE:
|     o FILE pointer to sam file with mapped loci
|   - errSCPtr:
|     o pointer to signed char to hold error status
| Output:
|   - Modifies:
|     o errSCPtr
|       * 0 for no errors
|       * def_memErr_cgMLST for memory errors
|       * def_fileErr_cgMLST for file errors
|   - Returns:
|     o pointer to allele_cgMLST struct with loci
|     o 0 for error
\-------------------------------------------------------*/
struct allele_cgMLST *
getLoci_allele_cgMLST(
   void *lociMapFILE,     /*sam file with mapped loci*/
   signed char *errSCPtr  /*for error reporting*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' fun08 TOC:
   '   - gets loci for a allele_cgMLST struct
   '   o fun08 sec01:
   '     - variable declarations
   '   o fun08 sec02:
   '     - initial setup (allocation)
   '   o fun08 Sec03:
   '     - get loci entries
   '   o fun08 sec05:
   '     - sort and return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_initLen_fun08 16
   signed char *buffHeapStr = 0;
   unsigned long lenBuffUL = 0;

   signed char *tmpSCPtr = 0;
   signed char lociStr[256];
   signed char *cpStr = 0;

   signed long slIndex = 0;
   signed long slSwap = 0;
   struct allele_cgMLST *retSTPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - initial setup (allocation)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retSTPtr = malloc(sizeof(struct allele_cgMLST)); 
   if(! retSTPtr)
      goto memErr_fun08_sec05;
   init_allele_cgMLST(retSTPtr);


   retSTPtr->cntAryUI =
      calloc(
         def_initLen_fun08,
         sizeof(unsigned int)
      );
   if(! retSTPtr->cntAryUI)
      goto memErr_fun08_sec05;


   retSTPtr->lociIndexArySI =
      calloc(
         def_initLen_fun08,
         sizeof(unsigned int)
      );
   if(! retSTPtr->lociIndexArySI)
      goto memErr_fun08_sec05;


   retSTPtr->lociStartAryUI =
      calloc(
         def_initLen_fun08,
         sizeof(unsigned int)
      );
   if(! retSTPtr->lociStartAryUI)
      goto memErr_fun08_sec05;


   retSTPtr->lociIdsST = mk_str_ptrAry(def_initLen_fun08);
   if(! retSTPtr->lociIdsST)
      goto memErr_fun08_sec05;
    

   retSTPtr->mapSamST =
       malloc(
          def_initLen_fun08 * sizeof(struct samEntry)
       );
   if(! retSTPtr->mapSamST)
      goto memErr_fun08_sec05;
   for(
      slIndex = 0;
      slIndex < def_initLen_fun08;
      ++slIndex
   ) init_samEntry(&retSTPtr->mapSamST[slIndex]);

   for(
      slIndex = 0;
      slIndex < def_initLen_fun08;
      ++slIndex
   ){ /*Loop: setup samEntry struct*/
      if( setup_samEntry(&retSTPtr->mapSamST[slIndex]) )
        goto memErr_fun08_sec05;
   } /*Loop: setup samEntry struct*/

   retSTPtr->sizeAlleleSL = def_initLen_fun08;
   retSTPtr->sizeLociSL = def_initLen_fun08;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - get loci entries
   ^   o fun08 sec03 sub01:
   ^     - get past header
   *   - get entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec03 Sub01:
   *   - get past header
   \*****************************************************/

   *errSCPtr =
      get_samEntry(
         &retSTPtr->mapSamST[0],
         &buffHeapStr,
         &lenBuffUL,
         (FILE *) lociMapFILE
      );

   if(*errSCPtr)
   { /*If: had error*/
      if(*errSCPtr == def_memErr_samEntry)
         goto memErr_fun08_sec05;
      else
         goto fileErr_fun08_sec05; /*emptye file*/
   } /*If: had error*/


   while(! *errSCPtr)
   { /*Loop: get past header*/
      if(retSTPtr->mapSamST[0].extraStr[0] != '@')
         break; /*past header*/

      else
         *errSCPtr =
            get_samEntry(
               &retSTPtr->mapSamST[0],
               &buffHeapStr,
               &lenBuffUL,
               (FILE *) lociMapFILE
            );
   } /*Loop: get past header*/

   if(*errSCPtr)
   { /*If: had error*/
      if(*errSCPtr == def_memErr_samEntry)
         goto memErr_fun08_sec05;
      else
         goto fileErr_fun08_sec05; /*only headers*/
   } /*If: had error*/

   /*****************************************************\
   * Fun08 Sec03 Sub02:
   *   - get entries
   *   o fun08 sec03 sub02 cat01:
   *     - reallocate allele memory if needed + start loop
   *   o fun08 sec03 sub02 cat02:
   *     - get loci id
   *   o fun08 sec03 sub02 cat03:
   *     - check if need more memory for new loci
   *   o fun08 sec03 sub02 cat04:
   *     - add new loci id
   *   o fun08 sec03 sub02 cat05:
   *     - get next entry in mapped loci sam file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun08 Sec03 Sub02 Cat01:
   +   - reallocate allele memory if needed + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(! *errSCPtr)
   { /*Loop: get past header*/

      if(
            retSTPtr->numAlleleSL
         >= retSTPtr->sizeAlleleSL - 1
      ){ /*If: need more allele memory*/
         slIndex = retSTPtr->numAlleleSL >> 1;
         slIndex += retSTPtr->numAlleleSL;

         tmpSCPtr =
            (signed char *)
            realloc(
               retSTPtr->cntAryUI,
               slIndex * sizeof(unsigned int)
            );
         if(! tmpSCPtr)
            goto memErr_fun08_sec05;
         retSTPtr->cntAryUI = (unsigned int *) tmpSCPtr;

         tmpSCPtr =
            (signed char *)
            realloc(
               retSTPtr->offAryUI,
               slIndex * sizeof(unsigned int)
            );
         if(! tmpSCPtr)
            goto memErr_fun08_sec05;
         retSTPtr->offAryUI = (unsigned int *) tmpSCPtr;

         tmpSCPtr =
            (signed char *)
            realloc(
               retSTPtr->mapSamST,
               slIndex * sizeof(struct samEntry)
            );
         if(! tmpSCPtr)
            goto memErr_fun08_sec05;
         retSTPtr->mapSamST =(struct samEntry *) tmpSCPtr;

         retSTPtr->sizeAlleleSL = slIndex;

         for(
            slIndex = retSTPtr->numAlleleSL;
            slIndex < retSTPtr->sizeAlleleSL;
            ++slIndex
         ) init_samEntry(&retSTPtr->mapSamST[slIndex]);

         for(
            slIndex = retSTPtr->numAlleleSL;
            slIndex < retSTPtr->sizeAlleleSL;
            ++slIndex
         ){ /*Loop: setup samEntry structs*/
            if( setup_samEntry(&retSTPtr->mapSamST[slIndex]) )
               goto memErr_fun08_sec05;
         } /*Loop: setup samEntry structs*/
      } /*If: need more allele memory*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat02:
      +   - get loci id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSCPtr = lociStr;

      cpStr =
         retSTPtr->mapSamST[
            retSTPtr->numAlleleSL
         ].qryIdStr;

      while(*cpStr != '\0')
      { /*Loop: copy loci id*/
         if(
               cpStr[0] == def_lociEndOne_cgMLST
            && cpStr[1] == def_lociEndTwo_cgMLST
         ) break; /*at allele id*/

         *tmpSCPtr++ = *cpStr++;
      } /*Loop: copy loci id*/

      *tmpSCPtr = '\0';

      slIndex =
         find_str_ptrAry(
            retSTPtr->lociIdsST,
            lociStr
         );

      if(slIndex >= 0)
      { /*If: loci already found*/

         if(
              retSTPtr->lociStartAryUI[slIndex]
            > retSTPtr->mapSamST[
                 retSTPtr->numAlleleSL
              ].refStartUI
         ){ /*If: this allele starts sooner*/
            retSTPtr->lociStartAryUI[slIndex] =
               retSTPtr->mapSamST[
                  retSTPtr->numAlleleSL
               ].refStartUI;
         } /*If: this allele starts sooner*/

         goto nextEntry_fun08_sec03_sub02_cat05;
         /*already have loci*/

      } /*If: loci already found*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat03:
      +   - check if need more memory for new loci
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(retSTPtr->numLociSL >= retSTPtr->sizeLociSL)
      { /*If: need more memory*/
         slIndex = retSTPtr->sizeLociSL;
         slIndex += (slIndex >> 1);

         tmpSCPtr =
            (signed char *)
            realloc(
               retSTPtr->lociIndexArySI,
               slIndex * sizeof(signed int)
            );
         if(! tmpSCPtr)
            goto memErr_fun08_sec05;
         retSTPtr->lociIndexArySI=(signed int *) tmpSCPtr;

         tmpSCPtr =
            (signed char *)
            realloc(
               retSTPtr->lociStartAryUI,
               slIndex * sizeof(unsigned int)
            );
         if(! tmpSCPtr)
            goto memErr_fun08_sec05;
         retSTPtr->lociStartAryUI =
            (unsigned int *) tmpSCPtr;

         *errSCPtr =
            resize_str_ptrAry(
               retSTPtr->lociIdsST,
               slIndex
            );
         if(*errSCPtr)
            goto memErr_fun08_sec05;

         retSTPtr->sizeLociSL = slIndex;
      } /*If: need more memory*/
      
      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat04:
      +   - add new loci id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      slIndex =
         (signed long)
         addSort_str_ptrAry(
            lociStr,
            retSTPtr->lociIdsST
         );
      
      retSTPtr->lociIndexArySI[retSTPtr->numLociSL] = 0;
      retSTPtr->lociStartAryUI[retSTPtr->numLociSL] = 0;

      slSwap = slIndex;

      while(slSwap < retSTPtr->numLociSL)
      { /*Loop: move loci starting positions up*/
         retSTPtr->lociStartAryUI[slSwap] ^=
            retSTPtr->lociStartAryUI[slSwap + 1];

         retSTPtr->lociStartAryUI[slSwap + 1] ^=
            retSTPtr->lociStartAryUI[slSwap];

         retSTPtr->lociStartAryUI[slSwap] ^=
            retSTPtr->lociStartAryUI[slSwap + 1];

         ++slSwap;
      } /*Loop: move loci starting positions up*/

      retSTPtr->lociStartAryUI[slIndex] =
         retSTPtr->mapSamST[
            retSTPtr->numAlleleSL
         ].refStartUI;
         /*add reference start of loci*/

      ++retSTPtr->numLociSL;

      /*the index array is filled in by the sorting step*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun08 Sec03 Sub02 Cat05:
      +   - get next entry in mapped loci sam file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      nextEntry_fun08_sec03_sub02_cat05:;

         ++retSTPtr->numAlleleSL;

         *errSCPtr =
            get_samEntry(
               &retSTPtr->mapSamST[retSTPtr->numAlleleSL],
               &buffHeapStr,
               &lenBuffUL,
               (FILE *) lociMapFILE
            );
   } /*Loop: get past header*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec05:
   ^   - sort and return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortAllele_cgMLST(retSTPtr);
   *errSCPtr = 0;
   goto ret_fun08_sec05;

   memErr_fun08_sec05:;
      *errSCPtr = def_memErr_cgMLST;
      goto errClean_fun08_sec05;

   fileErr_fun08_sec05:;
      *errSCPtr = def_fileErr_cgMLST;
      goto errClean_fun08_sec05;

   errClean_fun08_sec05:;
      if(retSTPtr)
         freeHeap_allele_cgMLST(retSTPtr);
      retSTPtr = 0;
      goto ret_fun08_sec05;

   ret_fun08_sec05:;
      if(buffHeapStr)
        free(buffHeapStr);
      buffHeapStr = 0;

      return retSTPtr;
} /*getLoci_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun09: scanLoci_allele_cgMLST
|   - search loci for best matching allele
| Input:
|   - alleleSTPtr:
|     o poitner to allele_cgMLST struct with loci and
|       alleles
|   - samSTPtr:
|     o pointer to samEntry struct with sequence to search
|   - indelLenUI:
|     o minimum length to count an indel as different
|   - minQUC:
|     o minium q-score to count snp as different
|   - minOverlapF:
|     o min % overlap between allele and sequence (0 to 1)
|   - posUIPtr:
|     o unsigned long pointer with position at in samSTPtr
|       sequence
|   - distSLPtr:
|     o pointer to signed long to hold edit distance to
|       returned allele (set to -1 if nothing found)
| Output:
|   - Modifies:
|     o posUIPtr to have end of found allele
|       * not incurmented if -1 (no loci) is returned
|     o distSLPtr to have best (lowest) edit distance or
|       -1 if could not find best allele
|       -2 if did not have enough overlap
|   - Returns:
|     o index of closest allele (mapSamST in alleleSTPtr)
|     o -1 if sequence has no loci
|     o -2 if found loci, but did not have enough overlap
\-------------------------------------------------------*/
signed long
scanLoci_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr, /*has loci/alleles*/
   struct samEntry *samSTPtr,  /*sequecne to scan*/
   unsigned int indelLenUI,    /*min indel size*/
   unsigned char minQUC,       /*min SNP q-score*/
   float minOverlapF,          /*min overlap with allele*/
   unsigned int *posUIPtr,     /*position in seq*/
   signed long *distSLPtr      /*best alleles edit dist*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - search loci for best matching allele
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - find next loci in read
   '   o fun09 sec03:
   '     - get loci id
   '   o fun09 sec04:
   '     - find closest allele in loci
   '   o fun09 sec05:
   '     - set up return and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long indexSL = 0; /*index of current allele on*/
   signed long bestIndexSL = -1;/*best index; start none*/

   signed long edDistSL = 0;     /*edit distance*/
   signed long bestDistSL = 0;   /*best edit distance*/
   float overlapF = 0;

   signed char lociStr[256];     /*loci id*/
   signed char *cpStr = 0;
   signed char *dupStr = 0;

   struct samEntry *samSTNoFree = 0;
   struct res_edDist edDistST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - find next loci in read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_res_edDist(&edDistST);


   /*find closest loci (by coordiante)*/
   indexSL =
      findStart_allele_cgMLST(
         alleleSTPtr,
         samSTPtr->refStartUI + *posUIPtr
      );

   samSTNoFree = &alleleSTPtr->mapSamST[indexSL];

   if(samSTNoFree->refStartUI > samSTPtr->refEndUI)
      goto noLoci_fun09_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - get loci id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cpStr = samSTNoFree->qryIdStr;
   dupStr = lociStr;

   while(*cpStr != '\0')
   { /*Loop: copy loci id*/
      if(
            cpStr[0] == def_lociEndOne_cgMLST
         && cpStr[1] == def_lociEndTwo_cgMLST
      ) break; /*at allele id*/

      *dupStr++ = *cpStr++;
   } /*Loop: copy loci id*/

   *dupStr = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - find closest allele in loci
   ^   o fun09 sec04 sub01:
   ^     - check if still on same loci + start loop
   ^   o fun09 sec04 sub02:
   ^     - check if have enough overlap on loci allele
   ^   o fun09 sec04 sub03:
   ^     - get edit distance and see if new best
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec04 Sub01:
   *   - check if still on same loci + start loop
   \*****************************************************/

   while(indexSL < alleleSTPtr->numAlleleSL)
   { /*Loop: compare sequence to alleles*/
      cpStr = samSTNoFree->qryIdStr;
      dupStr = lociStr;

      while(*cpStr == *dupStr)
      { /*Loop: compare loci id (are same?)*/
         if(
               cpStr[0] == def_lociEndOne_cgMLST
            && cpStr[1] == def_lociEndTwo_cgMLST
         ) goto getEdDist_fun09_sec04_sub02;

         else if(cpStr[0] == '\0')
            goto getEdDist_fun09_sec04_sub02;

         else
            *dupStr++ = *cpStr++;
      } /*Loop: compare loci id (are same?)*/

      break; /*no longer on matching loci*/

      /*****************************************************\
      * Fun09 Sec04 Sub02:
      *   - check if have enough overlap on loci allele
      \*****************************************************/

      getEdDist_fun09_sec04_sub02:;

      if(samSTPtr->refStartUI > samSTNoFree->refStartUI)
      { /*If: query is missing allele starting bases*/
         overlapF =
              samSTPtr->refStartUI
            - samSTNoFree->refStartUI;
      } /*If: query is missing allele starting bases*/

      else
         overlapF = 0;

      if(samSTPtr->refEndUI < samSTNoFree->refEndUI)
      { /*If: query is missing allele end bases*/
         overlapF +=
              samSTNoFree->refEndUI
            - samSTPtr->refEndUI;
      } /*If: query is missing allele end bases*/

      if(
            (overlapF / samSTNoFree->alnReadLenUI)
          < minOverlapF
      ){ /*If: not enough overlap*/ 

         if(bestIndexSL == -1)
            goto noOverlap_fun09_sec05; /*first loci*/
         else
            goto nextIndex_fun09_sec05; /*next loci*/

      }  /*If: not enough overlap*/ 

      /*****************************************************\
      * Fun09 Sec04 Sub03:
      *   - get edit distance and see if new best
      \*****************************************************/

      edDistSL =
         readCmpDist_edDist(
            samSTPtr,    /*query (sequence)*/
            samSTNoFree, /*reference (allele)*/
            indelLenUI,  /*min indel length to count*/
            minQUC,      /*min snp q-score to count snp*/
            0,           /*already checked overlap*/
            0,           /*min depth for profile (skip)*/
            500,         /*window size (ignored)*/
            &edDistST    /*extra return data (ignored)*/
         ); /*find distance*/

      if(edDistSL < bestDistSL)
      { /*If: new allele is closer*/
         bestDistSL = edDistSL;
         bestIndexSL = indexSL;
      } /*If: new allele is closer*/

      nextIndex_fun09_sec05:;
         ++indexSL;
   } /*Loop: compare sequence to alleles*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - set up return and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(bestIndexSL < 0)
      goto noLoci_fun09_sec05;

   *distSLPtr = bestDistSL;
      /*add edit distance*/

   *posUIPtr =
      alleleSTPtr->mapSamST[bestIndexSL].refEndUI;
      /*move to end of loci*/

   goto ret_fun09_sec05;

   noLoci_fun09_sec05:;
      *distSLPtr = -1;
      bestIndexSL = -1;
      goto ret_fun09_sec05;

   noOverlap_fun09_sec05:;
      *distSLPtr = -2;
      bestIndexSL = -2;

      if(
            alleleSTPtr->mapSamST[bestIndexSL].refEndUI
         >= samSTPtr->refEndUI
      ) goto noLoci_fun09_sec05;
        /*no more loci in sequence*/

      else
         *posUIPtr =
            alleleSTPtr->mapSamST[bestIndexSL].refEndUI;
           /*in this case overlap issue with start, there
           `   could be more loci in sequence
           */

      /*move to end of loci*/
      goto ret_fun09_sec05;

   ret_fun09_sec05:;
      /*not needed, but for future proof*/
      freeStack_res_edDist(&edDistST);
      return bestIndexSL;
} /*scanLoci_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun10: seqToLoci_allele_cgMLST
|   - add all loici in sequence to allele_cgMLST table
| Input:
|   - alleleSTPtr:
|     o poitner to allele_cgMLST struct with loci and
|       allele table
|   - samSTPtr:
|     o pointer to samEntry struct with sequence to search
|   - maxDistSL:
|     o maximum edit distance to count loci as same
|   - indelLenUI:
|     o minimum length to count an indel as different
|   - minQUC:
|     o minium q-score to count snp as different
|   - minOverlapF:
|     o min % overlap between allele and sequence (0 to 1)
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noLoci_cgMLST if sequence has no loci
\-------------------------------------------------------*/
signed char
seqToLoci_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr, /*has loci/alleles*/
   struct samEntry *samSTPtr,  /*sequecne to scan*/
   signed long maxDistSL,      /*max edit distace*/
   unsigned int indelLenUI,    /*min indel size*/
   unsigned char minQUC,       /*min SNP q-score*/
   float minOverlapF           /*min overlap with allele*/
){

   unsigned int posUI = 0;  /*position at in sequence*/
   signed long indexSL = 0; /*index of best allele*/
   signed long edDistSL = 0;/*edit distance, best allele*/
   signed long numLociSL = 0;

   do{ /*find all alleles*/
      indexSL =
         scanLoci_allele_cgMLST(
            alleleSTPtr,
            samSTPtr,
            indelLenUI,
            minQUC,
            minOverlapF,
            &posUI,
            &edDistSL
         );

      if(indexSL < 0)
      { /*If: no/partiall loci found*/
         if(indexSL < -1)
            continue; /*missing start of loci*/
         else
            break; /*no more loci in sequence*/
      } /*If: no/partiall loci found*/

      ++numLociSL;

      if(edDistSL <= maxDistSL)
         ++(alleleSTPtr->cntAryUI[indexSL]);
      else
         ++(alleleSTPtr->offAryUI[indexSL]);
   } while(indexSL >= 0); /*find all alleles*/

   if(numLociSL)
     return 0;
   else
     return def_noLoci_cgMLST;
} /*seqToLoci_allele_cgMLST*/

/*-------------------------------------------------------\
| Fun11: plin_allele_cgMLST
|   - prints lineage for the cgMLST results
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct with table to get
|       lineage from
|   - idStr:
|     o has id of sequence to print out
|   - minDepthUI:
|     o min depth to print result
|   - minPercF:
|     o min percent depth to not print as negative, so is
|       a cofident result
|   - pHeadBl:
|     o print the loci header (1), 0 do not
|   - outFILE:
|     o pointer to FILE to print table in alleleSTPtr to
| Output:
|   - Prints:
|     o table (cntAryUI and offAryUI) to outFILE
|       * number means identified allele and had enough
|         support
|       * *number value means loci had enough depth, but
|         not allele had low % suppport
|       * NA means not enough depth
|       * M or *M means id is missing
|       * Ux or *Ux means it is unqiue and it is closest
|         to allele number x
\-------------------------------------------------------*/
void
plin_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr, /*table to print*/
   signed char *idStr,                /*id to print out*/
   unsigned int minDepthUI,           /*min print depth*/
   float minPercF,          /*percent to print confident*/
   signed char pHeadBl,               /*print header*/
   void *outFILE                      /*output file*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - prints lineage for the cgMLST results
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - print header
   '   o fun11 sec03:
   '     - print best allele for each loci
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long lociSL = 0;   /*loci working on*/
   signed long alleleSL = 0; /*allele working on*/

   signed char lociStr[256];   /*loci name*/
   signed char alleleStr[256]; /*allele id*/
   signed char *cpStr = 0;
   signed char *dupStr = 0;

   unsigned int bestIndexUI = 0;/*best index*/
   unsigned int bestCntUI = 0;  /*highest number of hits*/
   unsigned int bestOffIndexUI = 0; /*best off index*/
   unsigned int bestOffCntUI = 0;   /*best off count*/

   float numMapF = 0;   /*number reads mapped to alleles*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - print header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(pHeadBl)
   { /*If: printing the header*/

      fprintf(
         (FILE *) outFILE,
         "id"
      );

      for(
         lociSL = 0;
         lociSL < alleleSTPtr->numLociSL;
         ++lociSL
      ) { /*Loop: print loci ids*/
         fprintf(
            (FILE *) outFILE,
            "\t%s",
            alleleSTPtr->lociIdsST->strAry[lociSL]
         );
      }  /*Loop: print loci ids*/

   } /*If: printing the header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - print best allele for each loci
   ^   o fun11 sec03 sub01:
   ^     - print sequence/map id
   ^   o fun11 sec03 sub02:
   ^     - print alleles
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - print sequence/map id
   \*****************************************************/

   if(idStr)
   { /*If: have id to print*/
      fprintf(
         (FILE *) outFILE,
         "%s%s",
         str_endLine,
         idStr
      );
   } /*If: have id to print*/

   else
   { /*Else: print NA*/
      fprintf(
         (FILE *) outFILE,
         "%sNA",
         str_endLine
      );
   } /*Else: print NA*/


   /*****************************************************\
   * Fun11 Sec03 Sub02:
   *   - print alleles
   *   o fun11 Sec03 Sub02 Cat01:
   *     - loop though loci and 0 counts
   *   o fun11 sec03 sub02 Cat02:
   *     - loop though alleles and get ids
   *   o fun11 sec03 sub02 Cat03:
   *     - find best allele and total reads for loci
   *   o fun11 sec03 sub02 Cat04:
   *     - see if had enough depth for best loci allele
   *   o fun11 sec03 sub02 Cat05:
   *     - extract best allele id for loci id and print
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun11 Sec03 Sub02 Cat01:
   +   - loop though loci and 0 counts
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   for(
      lociSL = 0;
      lociSL < alleleSTPtr->numLociSL;
      ++lociSL
   ){ /*Loop: print detected allele for loci*/

      bestIndexUI = 0;
      bestCntUI = 0;
      bestOffIndexUI = 0;
      bestOffCntUI = 0;

      numMapF = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun11 Sec03 Sub02 Cat02:
      +   - loop though alleles and get ids
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      for(
         alleleSL =
             (signed long) alleleSTPtr->lociIndexArySI;
         alleleSL < alleleSTPtr->numAlleleSL;
         ++alleleSL
      ){ /*Loop: find best allele*/

         /*copyt the loci id*/
         cpStr = alleleSTPtr->mapSamST[lociSL].qryIdStr;
         dupStr = lociStr;

         while(*cpStr != '\0')
         { /*Loop: copy loci id*/
            if(
                  cpStr[0] == def_lociEndOne_cgMLST
               && cpStr[1] == def_lociEndTwo_cgMLST
            ) break; /*at allele id*/

            *dupStr++ = *cpStr++;
         } /*Loop: copy loci id*/

         *dupStr = '\0';


         if(
            eqlNull_ulCp(
               lociStr,
               alleleSTPtr->lociIdsST->strAry[lociSL]
            )
         ) break; /*on next loci*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun11 Sec03 Sub02 Cat03:
         +   - find best allele and total reads for loci
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(alleleSTPtr->cntAryUI[alleleSL] > bestCntUI)
         { /*If: new best for this allele*/
            bestIndexUI = alleleSL;
            bestCntUI = alleleSTPtr->cntAryUI[alleleSL];
         } /*If: new best for this allele*/

         if(alleleSTPtr->cntAryUI[alleleSL] >bestOffCntUI)
         { /*If: new best for this allele*/
            bestOffIndexUI = alleleSL;
            bestOffCntUI =alleleSTPtr->cntAryUI[alleleSL];
         } /*If: new best for this allele*/
          

         /*get total reads for this allele*/
         numMapF += alleleSTPtr->cntAryUI[alleleSL];
         numMapF += alleleSTPtr->offAryUI[alleleSL];
      }  /*Loop: find best allele*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun11 Sec03 Sub02 Cat04:
      +   - see if had enough depth for best loci allele
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(numMapF < (float) minDepthUI)
      { /*If: not enough depth*/
         fprintf(
            (FILE *) outFILE,
            "\tNA"
         ); /*print out allele id*/

         continue;
      } /*If: not enough depth*/


      else if(
            (((float) bestCntUI ) / numMapF) < minPercF
         && (((float) bestOffCntUI ) / numMapF) < minPercF
      ){ /*Else If: not enough read depth*/
         dupStr = alleleStr;
         *dupStr++ = '*';
      }  /*Else If: not enough read depth*/

      else
         dupStr = alleleStr; /*have enough depth*/


      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun11 Sec03 Sub02 Cat05:
      +   - extract best allele id for loci id and print
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(bestOffCntUI > bestCntUI)
      { /*If: off allele mappings best*/
         *dupStr++ = 'U';

         cpStr =
           alleleSTPtr->mapSamST[bestOffIndexUI].qryIdStr;
      } /*If: off allele mappings best*/

      else
      { /*Else: on target allele as good or better*/
         cpStr =
            alleleSTPtr->mapSamST[bestIndexUI].qryIdStr;
      } /*Else: on target allele as good or better*/


      while(
             cpStr[0] != def_lociEndOne_cgMLST
         &&  cpStr[1] != def_lociEndTwo_cgMLST
      ){ /*Loop: find start of allele id*/
         if(*cpStr == '\0')
            break; /*no id*/
         else
            ++cpStr;
      }  /*Loop: find start of allele id*/

      
      if(*cpStr == '\0')
      { /*If: no id was input*/
         *dupStr++ = 'M'; /*no id*/
         *dupStr = '\0';
      } /*If: no id was input*/

      else
      { /*Else: found allele id*/
         cpStr_ulCp(
            dupStr,
            cpStr
         );
      } /*Else: found allele id*/


      fprintf(
         (FILE *) outFILE,
         "\t%s",
         alleleStr
      ); /*print out allele id*/

   } /*Loop: print detected allele for loci*/
} /*plin_allele_cgMLST*/

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
: Copyright (c) 2025 jeremyButtler
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
