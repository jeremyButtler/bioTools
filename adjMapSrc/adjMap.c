/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' adjMap SOF: Start Of File
'   - has functions to adjust alignment in one sam file to 
'     another reference
'   o header:
'     - included libraries
'   o fun01: adjMap
'     - addjust mapping coordinates between two seqeunces
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

#include "adjMap.h"

#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/numToStr.h"
#include "../genLib/seqST.h"
#include "../genLib/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/dirMatrix.h"
#include "../genAln/needle.h"
#include "../genAln/water.h"

/*no .h files*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*using max/min macro*/
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o std #include <stdio.h>
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: adjMap
|   - addjust mapping coordinates between two seqeunces
| Input:
|   - qrySTPtr:
|     o pointer to samEntry struct with the sequence to
|       adjust
|   - refSTPtr:
|     o pointer to samEntry struct with the reference
|   - minOverlapF:
|     o minimum percent of overlap between read and new
|       reference to keep
|   - alnSetPtr:
|     o alnSet struct with alignment settings for dealing
|       with shared softmasked and insertion regions
| Output:
|   - Modifies:
|     o qrySTPtr to have the adjusted alignment
|   - Returns:
|     o 0 for no errors
|     o def_noOverlap_adjMap for no overlaps or when
|       the percent overlap < minOverlapF
|       - (ref end - ref start) / (aligned ref length)
|     o def_noMap_adjMap if query is unmapped
|     o def_noSeq_adjMap if query has no sequence
|     o def_diffRef_adjMap if query is mapped to a
|       different reference then refSTPtr
|     o def_memErr_adjMap for memory errors
\-------------------------------------------------------*/
signed char
adjMap(
   struct samEntry *qrySTPtr,
   struct samEntry *refSTPtr,
   float minOverlapF,
   struct alnSet *alnSetPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - addjust mapping coordinates between two seqeunces
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - initialize and check if have overlap
   '   o fun01 sec03:
   '     - allocate memory for temporary variables
   '   o fun01 sec05:
   '     - deal with starting softmasked bases
   '   o fun01 sec06:
   '     - adjust cigar
   '   o fun01 Sec07:
   '     - deal with softmasking at end
   '   o fun01 sec08:
   '     - add new cigar entry
   '   o fun01 sec09:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0; /*return value*/

   schar *qrySeqStr = 0;
   sint qryCigPosSI = 0;
   sint qryCigNtSI = 0;
   sint qryPosSI = 0;

   schar *refSeqStr = 0;
   sint refCigPosSI = 0;
   sint refCigNtSI = 0;
   sint refPosSI = 0;
   uint refStartUI = 0; /*new reference start*/
   uint ignoreUI = 0;   /*when need for function*/

   schar *cigTypeHeapStr = 0;
   sint *cigHeapArySI = 0;
   uint lenCigUI = 0;
   uint posUI = 0;
   sint tmpSI = 0;

   struct dirMatrix matrixStackST;
   struct seqST qryStackST;
   struct seqST refStackST;

   slong scoreSL = 0;    /*new alignment score*/
   slong tmpSL = 0;
   float percScoreF = 0; /*for percent overlap*/

   /*extra stat recording variables*/
   uint numAnonUI = 0;
   uint numMaskUI = 0;

   schar indexBl = 0; /*lets me know to convert seq back*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - initialzie and check if have overlap
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_seqST(&qryStackST);
   init_seqST(&refStackST);
   init_dirMatrix(&matrixStackST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - check if can adjust coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qrySTPtr->flagUS & 4)
      goto noMap_fun01_sec09_sub04;

   if(
         ! qrySTPtr->seqStr
      || qrySTPtr->seqStr[0] == '*'
   ) goto noSeq_fun01_sec09_sub05;

   if(
      eql_charCp(
         qrySTPtr->refIdStr,
         refSTPtr->refIdStr,
         (schar) '\0'
      )
   ) goto differntRef_fun01_sec09_sub06;

   /*check if have overlap*/

   if(qrySTPtr->refStartUI > refSTPtr->refEndUI)
      goto noOverlap_fun01_sec09_sub03;

   if(qrySTPtr->refEndUI < refSTPtr->refStartUI)
      goto noOverlap_fun01_sec09_sub03;

   /*see if meet minimum percent overalp*/

   percScoreF =
      (float)
      min_genMath(
         qrySTPtr->refEndUI,
         refSTPtr->refEndUI
      ); /*first ending coordiante*/

   percScoreF -=
      (float)
      max_genMath(
         qrySTPtr->refStartUI,
         refSTPtr->refStartUI
      ); /*last start coordinate*/
      /*first end - last start = overlap length*/

   percScoreF /= refSTPtr->alnReadLenUI;
   /*percent aligned (by reference length)*/

   if(percScoreF < minOverlapF)
      goto noOverlap_fun01_sec09_sub03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - memory allocation and setup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   seqToIndex_alnSet(refSTPtr->seqStr);
   seqToIndex_alnSet(qrySTPtr->seqStr);
   indexBl = 1;

   lenCigUI = qrySTPtr->readLenUI;
      /*bit oversized, but always works*/

   cigTypeHeapStr =
      calloc(
         lenCigUI + 1,
         sizeof(schar)
      );

   if(! cigTypeHeapStr)
      goto memErr_fun01_sec09_sub02;

   cigHeapArySI =
      calloc(
         lenCigUI + 1,
         sizeof(sint)
      );

   if(! cigHeapArySI)
      goto memErr_fun01_sec09_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - deal with starting softmasked bases
   ^   o fun01 sec05 sub01:
   ^     - check if query is not softmasked
   ^   o fun01 sec05 sub02:
   ^     - check if reference is not softmasked
   ^   o fun01 sec05 sub03:
   ^     - query/ref have softmasking case (always align)
   ^   o fun01 sec05 sub04:
   ^     - align masked bases to region (all call)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec05 Sub01:
   *   - check if query is not softmasked
   *   o fun01 sec05 sub01 cat01:
   *     - if check (no softmask) & find ref position
   *   o fun01 sec05 sub01 cat02:
   *     - find first query base in reference
   *   o fun01 sec05 sub01 cat03:
   *     - align reference softmasked region
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun01 Sec05 Sub01 Cat01:
   +   - if check (query no softmask) & find ref position
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   refSeqStr = refSTPtr->seqStr;
   qrySeqStr = qrySTPtr->seqStr;

   refCigNtSI = refSTPtr->cigArySI[0];
   qryCigNtSI = qrySTPtr->cigArySI[0];

   if(qrySTPtr->cigTypeStr[0] != 'S')
   { /*If: no query softmasking at start*/

      if(qrySTPtr->refStartUI > refSTPtr->refStartUI)
      { /*If: query comes last*/
         qryPosSI = refSTPtr->refStartUI;

         findRefPos_samEntry(
            refSTPtr,
            &refCigPosSI,
            &refCigNtSI,
            qrySTPtr->refStartUI,
            &qryPosSI,     /*using as tempory varaible*/
            &refPosSI
         );

         qryPosSI = 0;

         goto adjMainAln_fun01_sec06_sub01;
      } /*If: query comes last*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec05 Sub01 Cat02:
      +   - find first query base in reference
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      refPosSI = qrySTPtr->refStartUI;

      findRefPos_samEntry(
         qrySTPtr,
         &qryCigPosSI,
         &qryCigNtSI,
         refSTPtr->refStartUI,
         &refPosSI,     /*using as tempory varaible*/
         &qryPosSI
      );

      refPosSI = 0;

      if(refSTPtr->cigArySI[0] != 'S')
         goto adjMainAln_fun01_sec06_sub01;

      refPosSI += refSTPtr->cigArySI[0];
      refCigPosSI += (refCigPosSI == 0);
      refCigPosSI = 1;
      refCigNtSI = refSTPtr->cigArySI[1];

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec05 Sub01 Cat03:
      +   - align reference softmasked region
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSI =
         min_genMath(
            qryPosSI,
            refPosSI
         );

      refStackST.seqStr = refSeqStr;
      refStackST.offsetUL = 0;
      refStackST.endAlnUL = refSTPtr->cigArySI[0] - 1;

      qryStackST.seqStr = qrySeqStr;
      qryStackST.offsetUL = (ulong) qryPosSI - tmpSI;
      qryStackST.endAlnUL = qryPosSI - 1;

      if(qryStackST.offsetUL > 0)
         --qryStackST.offsetUL;

      goto alnMasked_fun01_sec05_sub04;
   } /*If: no query softmasking at start*/

   /*****************************************************\
   * Fun01 Sec05 Sub02:
   *   - check if reference is not softmasked
   *   o fun01 sec05 sub02 cat01:
   *     - if reference has no softmasking; qry before ref
   *   o fun01 sec05 sub02 cat02:
   *     - reference is before query (need to align)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun01 Sec05 Sub02 Cat01:
   +   - if reference has no softmasking (qry before ref)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else if(refSTPtr->cigTypeStr[0] != 'S')
   { /*Else If: no reference softmasking at start*/

      /*check if query softmasked bases are before ref*/
      if(qrySTPtr->refStartUI < refSTPtr->refStartUI)
      { /*If: query is before reference*/
         cigTypeHeapStr[0] = 'S';
         cigHeapArySI[0] += qrySTPtr->cigArySI[0];

         qryPosSI += qrySTPtr->cigArySI[0];
         qryCigNtSI = qrySTPtr->cigArySI[1];
         qryCigPosSI = 1; /*get off softmask*/

         posUI = 1;
      } /*If: query is before reference*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec05 Sub02 Cat01:
      +   - reference is before query (need to align)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: query is after reference*/
         findRefPos_samEntry(
            refSTPtr,
            &refCigPosSI,
            &refCigNtSI,
            qrySTPtr->refStartUI,
            &qryPosSI,     /*using as tempory varaible*/
            &refPosSI
         );

         qryPosSI = 0;

         if(qrySTPtr->cigTypeStr[0] == 'S')
         { /*If: have softmasking*/
            qryPosSI += qrySTPtr->cigArySI[0];
            qryCigPosSI = 1;
            qryCigNtSI = qrySTPtr->cigArySI[1];
         } /*If: have softmasking*/

         tmpSI =
            min_genMath(
               qryPosSI,
               refPosSI
            );

         refStackST.seqStr = refSeqStr;
         refStackST.offsetUL = (ulong) refPosSI - tmpSI;
         refStackST.endAlnUL = refPosSI - 1;

         if(refStackST.offsetUL > 0)
            --refStackST.offsetUL;

         qryStackST.seqStr = qrySeqStr;
         qryStackST.offsetUL = 0;
         qryStackST.endAlnUL = qrySTPtr->cigArySI[0] - 1;

         goto alnMasked_fun01_sec05_sub04;
      } /*Else: query is after reference*/
   } /*Else If: no reference softmasking at start*/

   /*****************************************************\
   * Fun01 Sec05 Sub03:
   *   - query/ref have softmasking case (always align)
   *   o fun01 sec05 sub03 cat01:
   *     - see if query is before reference
   *   o fun01 sec05 sub03 cat02:
   *     - set up for reference first alignment
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun01 Sec05 Sub03 Cat01:
   +   - both softmasked; see if query is before reference
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: have two softmasked regions*/
      if(qrySTPtr->refStartUI < refSTPtr->refStartUI)
      { /*If: query comes first*/
         findRefPos_samEntry(
            qrySTPtr,
            &qryCigPosSI,
            &qryCigNtSI,
            refSTPtr->refStartUI,
            &refPosSI,     /*using as tempory varaible*/
            &qryPosSI
         ); /*find first reference base in query*/

         refPosSI = 0;

         if(refSTPtr->cigTypeStr[0] == 'S')
         { /*If: have softmasking*/
            refPosSI += refSTPtr->cigArySI[0];
            refCigPosSI += (refCigPosSI == 0);
            refCigPosSI = 1;
            refCigNtSI = refSTPtr->cigArySI[1];
         } /*If: have softmasking*/

         tmpSI =
            min_genMath(
               qryPosSI,
               refPosSI
            );

         refStackST.seqStr = refSeqStr;
         refStackST.offsetUL = 0;
         refStackST.endAlnUL = refSTPtr->cigArySI[0] - 1;

         qryStackST.seqStr = qrySeqStr;
         qryStackST.offsetUL = (ulong) qryPosSI - tmpSI;
         qryStackST.endAlnUL = qryPosSI;

         if(qryStackST.offsetUL > 0)
            --qryStackST.offsetUL;

         goto alnMasked_fun01_sec05_sub04;
      } /*If: query comes first*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec05 Sub03 Cat02:
      +   - set up for reference first alignment
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: reference comes first*/
         findRefPos_samEntry(
            refSTPtr,
            &refCigPosSI,
            &refCigNtSI,
            qrySTPtr->refStartUI,
            &qryPosSI,     /*using as tempory varaible*/
            &refPosSI
         ); /*find first reference base in query*/

         qryPosSI = 0;

         if(qrySTPtr->cigTypeStr[0] == 'S')
         { /*If: have softmasking*/
            qryPosSI += qrySTPtr->cigArySI[0];
            qryCigPosSI = 1;
            qryCigNtSI = qrySTPtr->cigArySI[1];
         } /*If: have softmasking*/

         tmpSI =
            min_genMath(
               qryPosSI,
               refPosSI
            );

         refStackST.seqStr = refSeqStr;
         refStackST.offsetUL = (ulong) refPosSI - tmpSI;
         refStackST.endAlnUL = refPosSI - 1;

         if(refStackST.offsetUL > 0)
            --refStackST.offsetUL;

         qryStackST.seqStr = qrySeqStr;
         qryStackST.offsetUL = 0;
         qryStackST.endAlnUL = qrySTPtr->cigArySI[0] - 1;

         goto alnMasked_fun01_sec05_sub04;
      } /*Else: reference comes first*/
   } /*Else: have two softmasked regions*/

   /*****************************************************\
   * Fun05 Sec05 Sub04:
   *   - align masked bases to region (all call)
   \*****************************************************/

   /*If: I hit this point, then I do not need to align*/
   goto adjMainAln_fun01_sec06_sub01;

   /*this would normally be repative code; so reducing
   `  it down with a goto instead of a function that
   `  takes a very large number of parameters
   */
   alnMasked_fun01_sec05_sub04:;

   scoreSL =
      water(
         &qryStackST,
         &refStackST,
         &matrixStackST,
         alnSetPtr
      );

   if(matrixStackST.errSC)
      goto memErr_fun01_sec09_sub02;

   posUI =
      getCig_dirMatrix(
         &matrixStackST,
         0,            /*use index in matrix*/
         0,            /*not reverse complement*/
         &qryStackST,
         &refStackST,
         &cigTypeHeapStr,
         &cigHeapArySI,
         posUI,
         &lenCigUI,
         &refStartUI,
         &numAnonUI,
         &numMaskUI,
         1,           /*want deletions if ref is short*/
         alnSetPtr
      ); /*build the cigar*/

   if(cigHeapArySI[posUI] == 'S')
   { /*If: softmasking at end (no alignment)*/
      for(
         qryPosSI = 1;
         qryPosSI <= (sint) posUI;
         ++qryPosSI
      ) cigHeapArySI[0] += cigHeapArySI[qryPosSI];

      cigTypeHeapStr[0] = 'S';
      cigTypeHeapStr[1] = 0;
      posUI = 1;
      scoreSL = 0;
   } /*If: softmasking at end (no alignment)*/

   if((sint) posUI < 0)
      goto memErr_fun01_sec09_sub02;

   refStackST.seqStr = 0;
   refStackST.offsetUL = 0;
   refStackST.endAlnUL = 0;

   qryStackST.seqStr = 0;
   qryStackST.offsetUL = 0;
   qryStackST.endAlnUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec06:
   ^   - adjust sequende alingments
   ^   o fun01 sec06 sub01:
   ^     - check if have masking (finished) + start loop
   ^   o fun01 sec06 sub02:
   ^     - check alignment type
   ^   o fun01 sec06 sub03:
   ^     - move to next cigar entries (if needed)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec06 Sub01:
   *   - check if have masking (finished) + start loop
   \*****************************************************/

   adjMainAln_fun01_sec06_sub01:;

   /*this only happens if both sequences are at the
   `   same coordiante
   */
   if(refSTPtr->cigTypeStr[refCigPosSI] == 'S')
   { /*If: have softmasking at start*/
      ++refCigPosSI;
       refCigNtSI = refSTPtr->cigArySI[refCigPosSI];
   } /*If: have softmasking at start*/

   if(qrySTPtr->cigTypeStr[qryCigPosSI] == 'S')
   { /*If: have softmasking at start*/
      ++qryCigPosSI;
      qryCigNtSI = qrySTPtr->cigArySI[qryCigPosSI];
   } /*If: have softmasking at start*/

   while(
         refCigPosSI < (sint) refSTPtr->lenCigUI
      && qryCigPosSI < (sint) qrySTPtr->lenCigUI
   ){ /*Loop: adjust alignments*/

      if(refSTPtr->cigTypeStr[refCigPosSI] == 'S')
         break; /*finished*/

      if(refSTPtr->cigTypeStr[refCigPosSI] == 'H')
         break; /*finished*/

      if(qrySTPtr->cigTypeStr[qryCigPosSI] == 'S')
         break; /*finished*/

      if(qrySTPtr->cigTypeStr[qryCigPosSI] == 'H')
         break; /*finished*/

      /**************************************************\
      * Fun01 Sec06 Sub02:
      *   - check alignment type
      *   o fun01 sec06 sub02 note01:
      *     - values for cigar entry combinations
      *   o fun01 sec06 sub02 cat01:
      *     - check if both are matches + start switch
      *   o fun01 sec06 sub02 cat02:
      *     - check if have match/mismatch case
      *   o fun01 sec06 sub02 cat03:
      *     - check if unkown match/mismatch case
      *   o fun01 sec06 sub02 cat04:
      *     - check if deletion and insertion
      *   o fun01 sec06 sub02 cat05:
      *     - check if deletion and base
      *   o fun01 sec06 sub02 cat06:
      *     - insertion with match/mismatch cases
      \**************************************************/

      /*`````````````````````````````````````````````````\
      ` Fun01 Sec06 Sub02 Note01:
      `   - table of cigar combinations. Softmaksing is
      `     excluded because is only at ends. So, loop
      `     ends with softmasking.
      `
      `   | entry 1   | value | entry 2   | value | sum |
      `   |:----------|:-----:|:----------|:-----:|:---:|
      `   | M (base)  |  77   | M (base)  |   77  | 154 |
      `   | = (match) |  61   | = (match) |   61  | 122 |
      `   | X (snp)   |  88   | X (snp)   |   88  | 176 |
      `   | I (ins)   |  73   | I (ins)   |   73  | 146 |
      `   | D (del)   |  68   | D (del)   |   68  | 136 |
      `   | X (snp)   |  88   | M (base)  |   77  | 165 |
      `   | = (match) |  61   | M (base)  |   77  | 138 |
      `   | = (match) |  61   | X (snp)   |   88  | 149 |
      `   | D (del)   |  68   | M (base)  |   77  | 145 |
      `   | D (del)   |  68   | = (match) |   61  | 129 |
      `   | D (del)   |  68   | X (snp)   |   88  | 156 |
      `   | I (ins)   |  68   | M (base)  |   77  | 150 |
      `   | I (ins)   |  68   | = (match) |   61  | 134 |
      `   | I (ins)   |  68   | X (snp)   |   88  | 161 |
      `   | I (ins)   |  68   | D (del)   |   68  | 141 |
      \`````````````````````````````````````````````````*/
 
      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec06 Sub02 Cat01:
      +   - check if both are matches + start switch
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      switch(
           refSTPtr->cigTypeStr[refCigPosSI]
         + qrySTPtr->cigTypeStr[qryCigPosSI]
      ){ /*Switch: check reference type*/
         case 122: /*=/=*/
         /*Case: both are matches to orignal ref*/
            tmpSI =
               min_genMath(
                  refCigNtSI,
                  qryCigNtSI
               );

            refCigNtSI -= tmpSI;
            qryCigNtSI -= tmpSI;

            if(cigTypeHeapStr[posUI] != '=')
            { /*If: no previous mismatches*/
                if(cigTypeHeapStr[posUI])
                   ++posUI;

                cigTypeHeapStr[posUI] = '=';
                cigHeapArySI[posUI] = 0;
            } /*If: no previous mismatches*/

            cigHeapArySI[posUI] += tmpSI;

            while(tmpSI > 0)
            { /*Loop: find new score*/
               scoreSL +=
                  getScore_alnSet(
                     qrySeqStr[qryPosSI++],
                     refSeqStr[refPosSI++],
                     alnSetPtr
                  );

               --tmpSI;
            } /*Loop: find new score*/

            break;
         /*Case: both are matches to orignal ref*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec06 Sub02 Cat02:
         +   - check if have match/mismatch case
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 149: /*=/X*/
         /*Case: query is snp to new ref*/
            tmpSI =
               min_genMath(
                  refCigNtSI,
                  qryCigNtSI
               );

            refCigNtSI -= tmpSI;
            qryCigNtSI -= tmpSI;

            if(cigTypeHeapStr[posUI] != 'X')
            { /*If: no previous mismatches*/
                if(cigTypeHeapStr[posUI])
                   ++posUI;

                cigTypeHeapStr[posUI] = 'X';
                cigHeapArySI[posUI] = 0;
            } /*If: no previous mismatches*/

            cigHeapArySI[posUI] += tmpSI;

            while(tmpSI > 0)
            { /*Loop: find new score*/
               scoreSL +=
                  getScore_alnSet(
                     qrySeqStr[qryPosSI++],
                     refSeqStr[refPosSI++],
                     alnSetPtr
                  );

               --tmpSI;
            } /*Loop: find new score*/

            break;
         /*Case: query is snp to new ref*/

         case 136: /*D/D*/
         /*Case: both are deletions to orignal ref*/
            tmpSI =
               min_genMath(
                  refCigNtSI,
                  qryCigNtSI
               );

            refCigNtSI -= tmpSI;
            qryCigNtSI -= tmpSI;

            break;
         /*Case: both are deletions to orignal ref*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec06 Sub02 Cat03:
         +   - check if unkown match/mismatch case
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 176: /*X/X*/
         case 165: /*X/M*/
         case 154: /*M/M*/
         case 138: /*=/M*/
         /*Case: match/unkown; need to see if have match*/
            tmpSI =
               min_genMath(
                  refCigNtSI,
                  qryCigNtSI
               );

            refCigNtSI -= tmpSI;
            qryCigNtSI -= tmpSI;
            
            while(tmpSI > 0)
            { /*Loop: find if match or SNP*/
               scoreSL +=
                  getScore_alnSet(
                     qrySeqStr[qryPosSI],
                     refSeqStr[qryPosSI],
                     alnSetPtr
                  );

               if(
                     refSeqStr[refPosSI++]
                  == qrySeqStr[qryPosSI++]
               ){ /*If: match*/
                  if(cigTypeHeapStr[posUI] != '=')
                  { /*If: no previous mismatches*/
                     if(cigTypeHeapStr[posUI])
                        ++posUI;

                      cigTypeHeapStr[posUI] = '=';
                      cigHeapArySI[posUI] = 0;
                  } /*If: no previous mismatches*/

                  ++cigHeapArySI[posUI];
               } /*If: match*/

               else
               { /*Else: SNP*/
                  if(cigTypeHeapStr[posUI] != 'X')
                  { /*If: no previous mismatches*/
                     if(cigTypeHeapStr[posUI])
                        ++posUI;

                      cigTypeHeapStr[posUI] = 'X';
                      cigHeapArySI[posUI] = 0;
                  } /*If: no previous mismatches*/

                  ++cigHeapArySI[posUI];
               } /*Else: SNP*/

               --tmpSI;
            } /*Loop: find if match or SNP*/

            break;
         /*Case: match/unkown; need to see if have match*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec06 Sub02 Cat04:
         +   - check if deletion and insertion
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 141: /*I/D*/
         /*Case: insertion and deletion*/

            if(refSTPtr->cigTypeStr[refCigPosSI] == 'D')
            { /*If: reference was deletion*/
               if(cigTypeHeapStr[posUI] != 'I')
               { /*If: have insertion*/
                  scoreSL += alnSetPtr->gapSS;

                  scoreSL +=
                     alnSetPtr->extendSS * (qryCigNtSI-1);

                  if(cigTypeHeapStr[posUI])
                     ++posUI;

                  cigTypeHeapStr[posUI] = 'I';
                  cigHeapArySI[posUI] = 0;
               } /*If: have insertion*/

               else
                  scoreSL +=
                     alnSetPtr->extendSS * qryCigNtSI;

               qryPosSI += qryCigNtSI;
               cigHeapArySI[posUI] = qryCigNtSI;
               qryCigNtSI = 0;
            } /*If: reference was deletion*/

            else
            { /*Else: reference was insertion*/
               if(cigTypeHeapStr[posUI] != 'D')
               { /*If: have deletion*/
                  scoreSL += alnSetPtr->gapSS;

                  scoreSL +=
                     alnSetPtr->extendSS * (refCigNtSI-1);
 
                  if(cigTypeHeapStr[posUI])
                     ++posUI;

                  cigTypeHeapStr[posUI] = 'D';
                  cigHeapArySI[posUI] = 0;
               } /*If: have deletion*/

               else
               { /*Else: already applied opening*/
                  scoreSL +=
                     alnSetPtr->extendSS * refCigNtSI;
               } /*Else: already applied opening*/

               refPosSI += refCigNtSI;
               cigHeapArySI[posUI] = refCigNtSI;
               refCigNtSI = 0;
            } /*Else: reference was insertion*/

            break;
         /*Case: insertion and deletion*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec06 Sub02 Cat05:
         +   - check if deletion and base
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 145: /*D/M*/
         case 129: /*D/=*/
         case 156: /*D/X*/
         /*Case: deletion with match/mismatch*/
            tmpSI =
               min_genMath(
                  refCigNtSI,
                  qryCigNtSI
               );

            refCigNtSI -= tmpSI;
            qryCigNtSI -= tmpSI;

            if(refSTPtr->cigTypeStr[refCigPosSI] == 'D')
            { /*If: reference was deletion*/

               if(cigTypeHeapStr[posUI] != 'I')
               { /*If: have insertion*/
                  scoreSL += alnSetPtr->gapSS;

                  scoreSL +=
                     alnSetPtr->extendSS * (tmpSI - 1);

                  if(cigTypeHeapStr[posUI])
                     ++posUI;

                  cigTypeHeapStr[posUI] = 'I';
                  cigHeapArySI[posUI] = 0;
               } /*If: have insertion*/

               else
                  scoreSL += alnSetPtr->extendSS * tmpSI;

               qryPosSI += tmpSI;
            } /*If: reference was deletion*/

            else
            { /*Else: query was deletion*/
               if(cigTypeHeapStr[posUI] != 'D')
               { /*If: have insertion*/
                  scoreSL += alnSetPtr->gapSS;

                  scoreSL +=
                     alnSetPtr->extendSS * (tmpSI - 1);

                  if(cigTypeHeapStr[posUI])
                     ++posUI;

                  cigTypeHeapStr[posUI] = 'D';
                  cigHeapArySI[posUI] = 0;
               } /*If: have insertion*/

               else
                  scoreSL += alnSetPtr->extendSS * tmpSI;

               refPosSI += tmpSI;
            } /*Else: query was deletion*/

            cigHeapArySI[posUI] += tmpSI;
            break;
         /*Case: deletion with match/mismatch*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec06 Sub02 Cat06:
         +   - insertion with match/mismatch cases
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 150: /*I/M*/
         case 134: /*I/=*/
         case 161: /*I/X*/
         /*Case: insertion with match/mismatch*/

            if(refSTPtr->cigTypeStr[refCigPosSI] == 'I')
            { /*If: reference was insertion*/

               if(cigTypeHeapStr[posUI] != 'D')
               { /*If: not a deletion*/
                  scoreSL += alnSetPtr->gapSS;

                  scoreSL +=
                     alnSetPtr->extendSS * (refCigNtSI-1);

                  if(cigTypeHeapStr[posUI])
                     ++posUI;

                  cigTypeHeapStr[posUI] = 'D';
                  cigHeapArySI[posUI] = 0;
               } /*If: not a deletion*/

               else
                  scoreSL +=
                     alnSetPtr->extendSS * refCigNtSI;

               cigHeapArySI[posUI] += refCigNtSI;
               refPosSI += refCigNtSI;
               refCigNtSI = 0;
            } /*If: reference was insertion*/

            else
            { /*Else: query was insertion*/

               if(cigTypeHeapStr[posUI] != 'I')
               { /*If: not a deletion*/
                  scoreSL += alnSetPtr->gapSS;

                  scoreSL +=
                     alnSetPtr->extendSS * (qryCigNtSI-1);

                  if(cigTypeHeapStr[posUI])
                     ++posUI;

                  cigTypeHeapStr[posUI] = 'I';
                  cigHeapArySI[posUI] = 0;
               } /*If: not a deletion*/

               else
                  scoreSL +=
                     alnSetPtr->extendSS * qryCigNtSI;

               cigHeapArySI[posUI] += qryCigNtSI;
               qryPosSI += qryCigNtSI;
               qryCigNtSI = 0;
            } /*Else: query was insertion*/

            break;
         /*Case: insertion with match/mismatch*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun01 Sec06 Sub02 Cat07:
         +   - insertion/insertion case (need to align)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 146: /*I/I*/
         { /*Case: insertion/insertion case*/
            refStackST.seqStr = &refSeqStr[refPosSI];
            refStackST.offsetUL = 0;

            refStackST.endAlnUL =
               (ulong) refSTPtr->cigArySI[refCigPosSI];

            qryStackST.seqStr = &qrySeqStr[qryPosSI];
            qryStackST.offsetUL = 0;

            qryStackST.endAlnUL =
               (ulong) qrySTPtr->cigArySI[qryCigPosSI];

            scoreSL +=
               needle(
                  &qryStackST,
                  &refStackST,
                  &matrixStackST,
                  alnSetPtr
               );

            if(matrixStackST.errSC)
               goto memErr_fun01_sec09_sub02;

            posUI =
               getCig_dirMatrix(
                  &matrixStackST,
                  0,            /*use index in matrix*/
                  0,            /*not reverse complement*/
                  &qryStackST,
                  &refStackST,
                  &cigTypeHeapStr,
                  &cigHeapArySI,
                  posUI,
                  &lenCigUI,
                  &ignoreUI,  /*needle does not softmask*/
                  &numAnonUI,
                  &numMaskUI,
                  0,          /*needle aligns end to end*/
                  alnSetPtr
               ); /*build the cigar*/

            if((sint) posUI < 0)
               goto memErr_fun01_sec09_sub02;

            qryStackST.seqStr = 0;
            qryStackST.offsetUL = 0;
            qryStackST.endAlnUL = 0;

            refStackST.seqStr = 0;
            refStackST.offsetUL = 0;
            refStackST.endAlnUL = 0;

            refPosSI += refSTPtr->cigArySI[refCigPosSI];
            qryPosSI += qrySTPtr->cigArySI[qryCigPosSI];

            refCigNtSI = 0;
            qryCigNtSI = 0;
         } /*Case: insertion/insertion case*/
      } /*Switch: check reference type*/

      /**************************************************\
      * Fun01 Sec06 Sub03:
      *   - move to next cigar entries (if needed)
      \**************************************************/

      if(refCigNtSI == 0)
      { /*If: move to next reference cigar entry*/
          ++refCigPosSI;
          refCigNtSI = refSTPtr->cigArySI[refCigPosSI];
      } /*If: move to next reference cigar entry*/

      if(qryCigNtSI == 0)
      { /*If: move to next query cigar entry*/
          ++qryCigPosSI;
          qryCigNtSI = qrySTPtr->cigArySI[qryCigPosSI];
      } /*If: move to next query cigar entry*/
   } /*Loop: adjust alignments*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec07:
   ^   - deal with softmasking at end
   ^   o fun01 sec07 sub01:
   ^     - at least one sequence was completly aligned
   ^   o fun01 sec07 sub02:
   ^     - need to align softmasked end
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec07 Sub01:
   *   - at least one sequence was completly aligned
   \*****************************************************/

   if(qryCigPosSI >= (sint) qrySTPtr->lenCigUI)
      goto addCig_fun01_sec08;

   else if(refCigPosSI >= (sint) refSTPtr->lenCigUI)
   { /*Else If: end of reference*/
      ++posUI;
      cigTypeHeapStr[posUI] = 'S';

      qryPosSI = qrySTPtr->readLenUI - qryPosSI;
      cigHeapArySI[posUI] = qryPosSI;
      numMaskUI += qryPosSI;

      goto addCig_fun01_sec08;
   } /*Else If: end of reference*/

   /*****************************************************\
   * Fun01 Sec07 Sub02:
   *   - need to align softmasked end
   *   o fun01 sec07 sub02 cat01:
   *     - set up for alignment (find sortest alignment)
   *   o fun01 sec07 sub02 cat02:
   *     - align softmasked positions
   *   o fun01 sec07 sub02 cat03:
   *     - get cigar for alignment and clean up
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun01 Sec07 Sub02 Cat01:
   +   - set up for alignment (find sortest alignment)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: have unaligned bases*/
      tmpSI =
         min_genMath(
            qrySTPtr->readLenUI - qryPosSI,
            refSTPtr->readLenUI - refPosSI
         ); /*find fewest remaing bases*/

      refStackST.seqStr = refSeqStr;
      refStackST.offsetUL = refPosSI;
      refStackST.endAlnUL = (ulong) refPosSI + tmpSI;

      qryStackST.seqStr = qrySeqStr;
      qryStackST.offsetUL = qryPosSI;
      qryStackST.endAlnUL = (ulong) qryPosSI + tmpSI;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec07 Sub02 Cat02:
      +   - align softmasked positions
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      tmpSL =
         water(
            &qryStackST,
            &refStackST,
            &matrixStackST,
            alnSetPtr
         );

      if(matrixStackST.errSC)
         goto memErr_fun01_sec09_sub02;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun01 Sec07 Sub02 Cat03:
      +   - get cigar for alignment and clean up
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      refCigPosSI =
         (sint)
         getCig_dirMatrix(
            &matrixStackST,
            0,            /*use index in matrix*/
            0,            /*not reverse complement*/
            &qryStackST,
            &refStackST,
            &cigTypeHeapStr,
            &cigHeapArySI,
            posUI,
            &lenCigUI,
            &ignoreUI,  /*should be next base*/
            &numAnonUI,
            &numMaskUI,
            0,          /*do not want deletions at end*/
            alnSetPtr
         ); /*build the cigar*/

      if((sint) posUI < 0)
         goto memErr_fun01_sec09_sub02;

      if(cigTypeHeapStr[posUI + 1] == 'S')
      { /*If: soft masking at start of alignment*/
         ++posUI;

         cigHeapArySI[posUI] =
            (sint) qrySTPtr->readLenUI - qryPosSI;
      } /*If: soft masking at start of alignment*/

      else
      { /*Else: have alignment*/
         posUI = (uint) refCigPosSI;
         scoreSL += tmpSL; /*keeping alignment*/

         tmpSI= qrySTPtr->readLenUI - qryStackST.endAlnUL;
         --tmpSI; /*readLenUI is index 1, but endAlnUL is
                  ` index 0. So, subtract 1 to get index 1
                  */

         if(tmpSI > 0)
         { /*If: have softmasking at end*/
            if(cigTypeHeapStr[posUI] != 'S')
            { /*If: need new cigar entry*/
               ++posUI;
               cigTypeHeapStr[posUI] = 'S';
               cigHeapArySI[posUI] = 0;
            } /*If: need new cigar entry*/

            cigHeapArySI[posUI] += tmpSI;
         } /*If: have softmasking at end*/

      } /*Else: have alignment*/

      refStackST.seqStr = 0;
      refStackST.offsetUL = 0;
      refStackST.endAlnUL = 0;

      qryStackST.seqStr = 0;
      qryStackST.offsetUL = 0;
      qryStackST.endAlnUL = 0;
   } /*Else: have unaligned bases*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec08:
   ^   - add new cigar entry and adjust stats
   ^   o fun01 sec08 sub01:
   ^     - add the new cigar entry
   ^   o fun01 sec08 sub02:
   ^     - adjust starting positions
   ^   o fun01 sec08 sub03:
   ^     - find number of matches, snp, ins, and del
   ^   o fun01 sec08 sub05:
   ^     - add in new referece id and update/rm extra
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec08 Sub01:
   *   - add the new cigar entry
   \*****************************************************/

   addCig_fun01_sec08:;

   ++posUI;

   if(qrySTPtr->lenCigBuffUI < posUI + 1)
   { /*If: old cigar entry to small*/
      if(qrySTPtr->cigTypeStr)
         free(qrySTPtr->cigTypeStr);

      qrySTPtr->cigTypeStr = 0;
      qrySTPtr->cigTypeStr = cigTypeHeapStr;
      cigTypeHeapStr = 0;

      if(qrySTPtr->cigArySI)
         free(qrySTPtr->cigArySI);

      qrySTPtr->cigArySI = 0;

      qrySTPtr->cigArySI = cigHeapArySI;
      cigHeapArySI = 0;

      qrySTPtr->lenCigBuffUI = lenCigUI;
      qrySTPtr->lenCigUI = posUI;
   } /*If: old cigar entry to small*/

   else
   { /*Else: copying cigar over*/
      cpLen_ulCp(
         qrySTPtr->cigTypeStr,
         cigTypeHeapStr,
         posUI * sizeof(schar)
      );

      cpLen_ulCp(
         (schar *) qrySTPtr->cigArySI,
         (schar *) cigHeapArySI,
         posUI * sizeof(sint)
      );

      qrySTPtr->lenCigUI = posUI;
   } /*Else: copying cigar over*/

   /*****************************************************\
   * Fun01 Sec08 Sub02:
   *   - adjust starting positions
   \*****************************************************/

   qrySTPtr->lenCigUI = posUI;

   if(refStartUI > 0)
      qrySTPtr->refStartUI = refStartUI;
    
   else if(qrySTPtr->refStartUI > refSTPtr->refStartUI)
      qrySTPtr->refStartUI -= refSTPtr->refStartUI;
   else
      qrySTPtr->refStartUI = refSTPtr->refStartUI;

   /*****************************************************\
   * Fun01 Sec08 Sub03:
   *   - find number of matches, snp, ins, and del
   \*****************************************************/

   qrySTPtr->numMaskUI = 0;

   qrySTPtr->numMatchUI = 0;
   qrySTPtr->numSnpUI = 0;
   qrySTPtr->numInsUI = 0;
   qrySTPtr->numDelUI = 0;

   for(
      posUI = 0;
      posUI < qrySTPtr->lenCigUI;
      ++posUI
   ){ /*Loop: ajdust error counts*/
      switch(qrySTPtr->cigTypeStr[posUI])
      { /*Switch: count error events*/
         case 'S':
            qrySTPtr->numMaskUI +=
               qrySTPtr->cigArySI[posUI];

            break;

         case '=':
            qrySTPtr->numMatchUI +=
               qrySTPtr->cigArySI[posUI];

            break;

         case 'X':
            qrySTPtr->numSnpUI +=
               qrySTPtr->cigArySI[posUI];

            break;

         case 'I':
            qrySTPtr->numInsUI +=
               qrySTPtr->cigArySI[posUI];

            break;

         case 'D':
            qrySTPtr->numDelUI +=
               qrySTPtr->cigArySI[posUI];

            break;

         /*case 'M': is not possible (was removed)*/
      } /*Switch: count error events*/
   } /*Loop: ajdust error counts*/

   /*****************************************************\
   * Fun01 Sec08 Sub04:
   *   - find end position and aligned length
   \*****************************************************/

   qrySTPtr->alnReadLenUI =
        qrySTPtr->numMatchUI
      + qrySTPtr->numSnpUI
      + qrySTPtr->numDelUI;

   qrySTPtr->refEndUI = qrySTPtr->refStartUI;
   qrySTPtr->refEndUI += qrySTPtr->alnReadLenUI;

   qrySTPtr->refEndUI -= (qrySTPtr->alnReadLenUI > 0);
      /*for index 1 when alnigned read length > 0*/

   /*****************************************************\
   * Fun01 Sec08 Sub05:
   *   - add in new referece id and update/rm extra
   \*****************************************************/

  posUI =
     cpDelim_ulCp(
        qrySTPtr->refIdStr,
        refSTPtr->qryIdStr,
        0,
        0
     );

   qrySTPtr->lenRefIdUC = (uchar) posUI;

   posUI = 0;

   qrySTPtr->extraStr[posUI++] = 'N';
   qrySTPtr->extraStr[posUI++] = 'M';
   qrySTPtr->extraStr[posUI++] = ':';
   qrySTPtr->extraStr[posUI++] = 'i';
   qrySTPtr->extraStr[posUI++] = ':';

   posUI +=
      numToStr(
         &qrySTPtr->extraStr[posUI],
         qrySTPtr->numSnpUI
           + qrySTPtr->numInsUI
           + qrySTPtr->numDelUI
      ); /*get edit distance*/

   qrySTPtr->extraStr[posUI++] = '\t';
   qrySTPtr->extraStr[posUI++] = 'A';
   qrySTPtr->extraStr[posUI++] = 'S';
   qrySTPtr->extraStr[posUI++] = ':';
   qrySTPtr->extraStr[posUI++] = 'i';
   qrySTPtr->extraStr[posUI++] = ':';

   posUI +=
      numToStr(
         &qrySTPtr->extraStr[posUI],
         scoreSL / def_scoreAdj_alnDefs
      ); /*get edit distance*/

   qrySTPtr->lenExtraUI = posUI;

   if(refSTPtr->flagUS & 16)
      revCmp_samEntry(qrySTPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec09:
   ^   - clean up
   ^   o fun01 sec09 sub01:
   ^     - no error cleanup
   ^   o fun01 sec09 sub02:
   ^     - memory error cleanup
   ^   o fun01 sec09 sub03:
   ^     - no overlap error
   ^   o fun01 sec09 sub04:
   ^     - no map error
   ^   o fun01 sec09 sub05:
   ^     - no seqeunce error
   ^   o fun01 sec09 sub06:
   ^     - different reference error
   ^   o fun01 sec09 sub07:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec09 Sub01:
   *   - no errors cleanup
   \*****************************************************/

   errSC = 0;
   goto cleanUp_fun01_sec09_sub07;

   /*****************************************************\
   * Fun01 Sec09 Sub02:
   *   - memory error cleanup
   \*****************************************************/

   memErr_fun01_sec09_sub02:;
   errSC = def_memErr_adjMap;
   goto cleanUp_fun01_sec09_sub07;

   /*****************************************************\
   * Fun01 Sec09 Sub03:
   *   - no overlap error
   \*****************************************************/

   noOverlap_fun01_sec09_sub03:;
   errSC = def_noOverlap_adjMap;
   goto cleanUp_fun01_sec09_sub07;

   /*****************************************************\
   * Fun01 Sec09 Sub04:
   *   - no map error
   \*****************************************************/

   noMap_fun01_sec09_sub04:;
   errSC = def_noMap_adjMap;
   goto cleanUp_fun01_sec09_sub07;

   /*****************************************************\
   * Fun01 Sec09 Sub05:
   *   - no sequence error
   \*****************************************************/

   noSeq_fun01_sec09_sub05:;
   errSC = def_noSeq_adjMap;
   goto cleanUp_fun01_sec09_sub07;

   /*****************************************************\
   * Fun01 Sec09 Sub06:
   *   - different reference error
   \*****************************************************/

   differntRef_fun01_sec09_sub06:;
   errSC = def_diffRef_adjMap;
   goto cleanUp_fun01_sec09_sub07;

   /*****************************************************\
   * Fun01 Sec09 Sub07:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_fun01_sec09_sub07:;

   /*these values were pointers to pointers, so do not
   `  want to free sequences
   */
   qryStackST.seqStr = 0;
   qryStackST.offsetUL = 0;
   qryStackST.endAlnUL = 0;

   refStackST.seqStr = 0;
   refStackST.offsetUL = 0;
   refStackST.endAlnUL = 0;

   /*clean up*/
   if(indexBl)
   { /*If: have sequences converted to index's*/
      indexToSeq_alnSet(qrySTPtr->seqStr);
      indexToSeq_alnSet(refSTPtr->seqStr);
      indexBl = 0;
   } /*If: have sequences converted to index's*/

   if(cigTypeHeapStr)
      free(cigTypeHeapStr);

   cigTypeHeapStr = 0;

   if(cigHeapArySI)
      free(cigHeapArySI);

   cigHeapArySI = 0;

   freeStack_seqST(&qryStackST);
   freeStack_seqST(&refStackST);
   freeStack_dirMatrix(&matrixStackST);

   return errSC;
} /*adjMap*/
