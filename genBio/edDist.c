/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' edDist SOF: Start Of File
'   - has functions to find edit distances (does some
'     filtering)
'   o header:
'     - included libraries
'   o .c fun01: isTransNt_edDist
'     - checks to see if query base is a transversion
'     - not used (manually inlined), but here for future
'   o fun02: readCmpDist_edDist
'     - gets edit distances between the query & reference
'   o fun03: dist_edDist
'     - gets edit distances for reference
'   o fun04: distProb_edDist
'     - finds probablity of reads matching by chance
'   o fun05: addReadToDepth_edDist
'     - adds a read to a depth profile
'   o fun06: mkDepthProfile_edDist
'     - finds depth of every base in samEntry reference
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

#include "edDist.h"

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "seqST.h"
#include "samEntry.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*min .h macro only*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c   #include "../genLib/base10str.h"
!   o .c   #include "../genLib/numToStr.h"
!   o .c   #include "../genLib/strAry.h"
!   o .h   #include "ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: isTransNt_edDist
|   - checks to see if query base is a transversion
| Input:
|   - qryUC:
|     o query nucleotide to check
|   - refUC:
|     o reference nucleotide to check
| Output:
|   - Returns:
|     o -1 if was a transversion
|     o 0 if was a transition
\-------------------------------------------------------*/
signed char
isTransNt_edDist(
   unsigned char qryUC,
   unsigned char refUC
){
    switch(qryUC & (~32))
    { /*switch, reverse complement*/
        case 'A':
        case 'G':
        case 'R': /*A/G*/
        /*Case: query is A or G*/
           switch(refUC & (~32))
           { /*Switch: query is A, G or R (A/G)*/
              case 'C':
                 return -1;
              case 'T':
                 return -1;
              case 'Y': /*C/T*/
                 return -1;
              default:
                 return 0;
           } /*Switch: query is A, G or Y (C/T)*/

           return 0;
        /*Case: query is G*/

        case 'T':
        case 'U':
        case 'C':
        case 'Y': /*T/C*/
        /*Case: query is C, T/U, or Y (T/C)*/ 
           switch(refUC & (~32))
           { /*Switch: query is C*/
              case 'A':
                 return -1;
              case 'G':
                 return -1;
              case 'R': /*A/G*/
                 return -1;
              default:
                 return 0;
           } /*Switch: query is C*/

           return 0;
        /*Case: query is C, T/U, or R (A/G)*/

        default:
           return 0;
    } /*switch, reverse complement*/

    return 0;
} /*isTransNt_edDist*/

/*-------------------------------------------------------\
| Fun02: readCmpDist_edDist
|   - gets edit distances between two reads mapped to the
|     same reference
|   - deletions and insertions are only counted if they
|     execed a minimum length.
| Input:
|   - qrySTPtr:
|     o pointer to samEntry structure with read (query) to
|       find the edit distance for
|   - refSTPtr:
|     o pointer to samEntry structure with reference to
|       compare query (qrySTPtr) to
|   - indelLenUI:
|     o minimum length for a indel to count as an event
|   - minQUC:
|     o minimum q-score to not discard an snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - minDepthUI:
|     o minimum depth for reference base to keep
|       difference if using a depth profile (depthAryUI)
|   - depthAryUI:
|     o unsigned int array with read depths for each
|       reference base (use 0 to not use)
|   - noTranBl:
|     o 1 ignore transitions (a->g, c->t, t->c, g->a)
|     o 0 keep transitions
|   - overlapUI:
|     o pointer to unsigned int to hold nubmer of
|       reference base covered by query
|   - numIndelUI:
|     o pointer to unisigned int to hold the number of
|       indels in edit distance
|   - indelEventsUI:
|     o pointer to unsigned int to hold the number of
|       indel events (times a group of indels occured)
| Output:
|   - Modifies:
|     o numIndelUI to have number indels kept
|     o indelEventsUI to have number of indel events
|     o overlapUI to hold number of overlapped bases
|   - Returns:
|     o edit distance between query and ref
|     o negative value if could not find distance
|       - def_noOveralp_edDist if reads to not overlap
|       - def_diffRef_edDist if reads have different ref
|       - def_noSeq_edDist if one read is missing sequence
|       - def_noMap_edDist if one read is unmapped
\-------------------------------------------------------*/
signed long
readCmpDist_edDist(
   struct samEntry *qrySTPtr, /*read for edit distance*/
   struct samEntry *refSTPtr, /*ref to compare*/
   unsigned int indelLenUI,   /*min indel length*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   unsigned int minDepthUI,   /*min depth if profiling*/
   unsigned int *depthAryUI,  /*depth profile*/
   signed char noTranBl,        /*1: ignore transitions*/
   unsigned int *overlapUI,   /*overlap length*/
   unsigned int *numIndelUI,  /*number indels kept*/
   unsigned int *indelEventsUI /*nubmer indel events*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - gets edit distances between the query & reference
   '   o fun02 sec01:
   '     - variable declerations
   '   o fun02 sec02:
   '     - check if can get edit distance
   '   o fun02 sec03:
   '     - initialize and check if have q-score entreis
   '   o fun02 sec04:
   '     - find start of overlap between query & reference
   '   o fun02 sec05:
   '     - find edit distance
   '   o fun02 sec06:
   '     - return the edit distance
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siQryCig = 0;  /*query cigar position*/
   sint siRefCig = 0;  /*reference cigar position*/
   sint refValSI = 0;
   sint qryValSI = 0;

   sint tmpSI = 0;

   uint uiQry = 0;     /*query nucleotide on*/
   uint uiRef = 0;     /*reference nucleotide on*/

   float overlapF = 0;
   slong distSL = 0;    /*edit distance*/

   schar qryQBl = 0;
   schar refQBl = 0;
   uchar qUC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - check if can get edit distance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *overlapUI = 0;
   *indelEventsUI = 0;
   *numIndelUI = 0;

   if(qrySTPtr->flagUS & 4)
      goto noMap_fun02_sec06;

   if(refSTPtr->flagUS & 4)
      goto noMap_fun02_sec06;

   if(qrySTPtr->refEndUI < refSTPtr->refStartUI)
      goto noOverlap_fun02_sec06;

   if(qrySTPtr->refStartUI > refSTPtr->refEndUI)
      goto noOverlap_fun02_sec06;

   overlapF =
     (float)
     min_genMath(
        qrySTPtr->refEndUI,
        refSTPtr->refEndUI
     );

   overlapF -=
     (float)
     max_genMath(
        qrySTPtr->refStartUI,
        refSTPtr->refStartUI
     );

   if(
             overlapF / (float) refSTPtr->alnReadLenUI
           < minOverlapF
      &&
             overlapF / (float) qrySTPtr->alnReadLenUI
           < minOverlapF
   ) goto noOverlap_fun02_sec06;

   if(
         ! qrySTPtr->seqStr
      || *qrySTPtr->seqStr == '*' 
      || *qrySTPtr->seqStr == '\0'
   ) goto noSeq_fun02_sec06;

   if(
         ! refSTPtr->seqStr
      || *refSTPtr->seqStr == '*' 
      || *refSTPtr->seqStr == '\0'
   ) goto noSeq_fun02_sec06;

   if(
      eql_ulCp(
         qrySTPtr->refIdStr,
         refSTPtr->refIdStr,
         0,
         '\0'
      )
   ) goto diffRef_fun02_sec06;
     /*reads are mapped to different references*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - initialize and check if have q-score entreis
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


   if(
         qrySTPtr->qStr[0] == '\0'
      ||
         (
             qrySTPtr->qStr[0] == '*'
          && qrySTPtr->qStr[1] == '\0'
         )
   ) qryQBl = 0;

   else
      qryQBl = 1;

   if(
         refSTPtr->qStr[0] == '\0'
      ||
         (
             refSTPtr->qStr[0] == '*'
          && refSTPtr->qStr[1] == '\0'
         )
   ) refQBl = 0;

   else
      refQBl = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - find start of overlap between query & reference
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


   refValSI = refSTPtr->cigArySI[0];
   qryValSI = qrySTPtr->cigArySI[0];

   if(qrySTPtr->refStartUI > refSTPtr->refStartUI)
   { /*If: move the reference foward*/
      uiQry = refSTPtr->refStartUI;

      findRefPos_samEntry(
         refSTPtr,
         &siRefCig,
         &refValSI,
         qrySTPtr->refStartUI,
         (sint *) &uiQry, /*reference pos (discard)*/
         (sint *) &uiRef
      ); /*set reference to first query base*/

      uiQry = 0;
   } /*If: move reference foward*/

   else if(qrySTPtr->refStartUI < refSTPtr->refStartUI)
   { /*Else If: move query foward*/
      uiRef = qrySTPtr->refStartUI;

      findRefPos_samEntry(
         qrySTPtr,
         &siQryCig,
         &qryValSI,
         refSTPtr->refStartUI, /*end query position*/
         (sint *) &uiRef, /*query pos (discard)*/
         (sint *) &uiQry  /*end query position*/
      ); /*set reference to first query base*/

      uiRef = 0;
   } /*Else If: move query foward*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec05:
   ^   - find edit distance
   ^   o fun02 sec05 sub01:
   ^     - get past masking at start
   ^   o fun02 sec05 sub02:
   ^     - start loop and check if at end (hard/soft mask)
   ^   o fun02 sec05 note01:
   ^     - table of cigar entries added together
   ^   o fun02 sec05 sub03:
   ^     - start switch/check match/possible snp combos
   ^   o fun02 sec05 sub04:
   ^     - check matches or same error type
   ^   o fun02 sec05 sub05:
   ^     - handel deletion cases
   ^   o fun02 sec05 sub06:
   ^     - handel insertion cases
   ^   o fun02 sec05 sub07:
   ^     - check if move to next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec05 Sub01:
   *   - get past masking at start
   \*****************************************************/

   if(refSTPtr->cigTypeStr[siRefCig] == 'S')
   { /*If: have softmasked region*/
      uiRef += refValSI;
      ++siRefCig;
      refValSI = refSTPtr->cigArySI[siRefCig];
   } /*If: have softmasked region*/

   if(qrySTPtr->cigTypeStr[siQryCig] == 'S')
   { /*If: have softmasked region*/
      uiQry += qryValSI;
      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*If: have softmasked region*/

   if(refSTPtr->cigTypeStr[siRefCig] == 'H')
   { /*If: have hardmasked region*/
      ++siRefCig;
      refValSI = refSTPtr->cigArySI[siRefCig];
   } /*If: have hardmasked region*/

   if(qrySTPtr->cigTypeStr[siQryCig] == 'H')
   { /*If: have hardmaksed region*/
      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*If: have hardmasked region*/

   /*****************************************************\
   * Fun02 Sec05 Sub02:
   *   - start loop and check if at end (hard/soft mask)
   \*****************************************************/

   while(
         siQryCig < (sint) qrySTPtr->lenCigUI
      && siRefCig < (sint) refSTPtr->lenCigUI
   ){ /*Loop: get edit distance*/
      if(refSTPtr->cigTypeStr[siRefCig] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[siQryCig] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(refSTPtr->cigTypeStr[siRefCig] == 'H')
         break; /*hard masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[siQryCig] == 'H')
         break; /*hard masking only at ends (finished)*/

      /*`````````````````````````````````````````````````\
      ` Fun02 Sec05 Note01:
      `   - table of cigar entries added together
      \`````````````````````````````````````````````````*/

      /*Values:
      ` =: 61 (match)
      ` X: 88 (snp)
      ` M: 77 (match/snp)
      ` D: 68 (deletion)
      ` I: 73 (insertion)
      ` M + M: 154
      ` = + =: 122
      ` X + X: 176 (need to check if snps agree/disagree)
      ` I + I: 146 (check size; rm low q-scores)
      ` D + D: 136 (check size)
      `
      ` X + M: 165 (need to check if agree/disagree)
      ` = + M: 138 (need to check if agree/disagree)
      ` = + X: 149 (disagree, check q-scores)
      `
      ` D + M: 145 (disagree, check size)
      ` D + =: 129 (disagree, check size)
      ` D + X: 156 (disagree, check siz)
      `
      ` I + M: 150
      ` I + =: 134
      ` I + X: 161
      ` I + D: 141
      */

      /**************************************************\
      * Fun02 Sec05 Sub03:
      *   - start switch/check match/possible snp combos
      *   o fun02 sec05 sub03 cat01:
      *     - get number shared snps/matchs + start loop
      *   o fun02 sec05 sub03 cat02:
      *     - match, anonymous and q-score filter
      *   o fun02 sec05 sub03 cat03:
      *     - depth array filtering (if using)
      *   o fun02 sec05 sub03 cat04:
      *     - transition filtering (if using)
      *   o fun02 sec05 sub03 cat05:
      *     - move to next nucleotide
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun02 Sec05 Sub03 Cat01:
      +   - get number shared snps/matchs + start loop
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      switch(
           refSTPtr->cigTypeStr[siRefCig]
         + qrySTPtr->cigTypeStr[siQryCig]
      ){ /*Switch: find mutation combination*/
         /*cases were I have to compare sequences*/
         case 154: /*M/M*/
         case 176: /*X/X*/
         case 165: /*X/M*/
         case 138: /*=/M*/
         case 149: /*=/X*/
         /*Case: known snp or could be snp*/
            tmpSI =
               min_genMath(
                  refValSI,
                  qryValSI
               );

            *overlapUI += tmpSI;

            while(tmpSI > 0)
            { /*Loop: count number of SNPs*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun02 Sec05 Sub03 Cat02:
               +   - match, anonymous and q-score filter
               \++++++++++++++++++++++++++++++++++++++++*/

               if(
                     refSTPtr->seqStr[uiRef]
                  == qrySTPtr->seqStr[uiQry]
               ) goto nextNt_fun02_sec05_sub03_cat05;

               else if(refSTPtr->seqStr[uiRef] == 'N')
                  goto nextNt_fun02_sec05_sub03_cat05;

               else if(qrySTPtr->seqStr[uiQry] == 'N')
                  goto nextNt_fun02_sec05_sub03_cat05;

               if(qryQBl)
               { /*If: have query q-score entry*/
                  qUC =
                       qrySTPtr->qStr[uiQry]
                     - def_adjQ_samEntry;

                  if(qUC < minQUC)
                     goto nextNt_fun02_sec05_sub03_cat05;
               } /*If: have query q-score entry*/

               if(refQBl)
               { /*If: have reference q-score entry*/
                  qUC =
                       refSTPtr->qStr[uiRef]
                     - def_adjQ_samEntry;

                  if(qUC < minQUC)
                     goto nextNt_fun02_sec05_sub03_cat05;
               } /*If: have reference q-score entry*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun02 Sec05 Sub03 Cat03:
               +   - depth array filtering (if using)
               \++++++++++++++++++++++++++++++++++++++++*/

               if(depthAryUI)
               { /*If: using a depth profile*/
                  if(depthAryUI[uiRef] < minDepthUI)
                     goto nextNt_fun02_sec05_sub03_cat05;
               } /*If: using a depth profile*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun02 Sec05 Sub03 Cat04:
               +   - transition filtering (if using)
               \++++++++++++++++++++++++++++++++++++++++*/

               if(noTranBl)
               { /*If: checking transitions*/
                 switch(qrySTPtr->seqStr[uiQry] & (~32))
                 { /*switch, reverse complement*/
                   case 'A':
                   case 'G':
                   case 'R': /*A/G*/
                   /*Case: query is A or G*/
                    switch(
                        refSTPtr->seqStr[uiRef]
                      & (~32)
                    ){ /*Switch: query A, G or R (A/G)*/
                      case 'C': break; /*transversion*/
                      case 'T': break; /*transversion*/
                      case 'Y': break; /*CT transversion*/ 
                      default:
                      goto nextNt_fun02_sec05_sub03_cat05;
                    } /*Switch: query is A, G or Y (C/T)*/

                     break; /*is transversion*/
                   /*Case: query is A or G*/

                   case 'T':
                   case 'U':
                   case 'C':
                   case 'Y': /*T/C*/
                   /*Case: query is C, T/U, or Y (T/C)*/ 
                     switch(
                         refSTPtr->seqStr[uiRef]
                       & (~32)
                     ){ /*Switch: query c or t*/
                      case 'A': break; /*transversion*/
                      case 'G': break; /*transversion*/
                      case 'R': break; /*AG transversion*/
                      default:
                      goto nextNt_fun02_sec05_sub03_cat05;
                     } /*Switch: query c or t*/

                     break; /*is transversion*/
                   /*Case: query is C, T/U, or R (A/G)*/

                   default:
                      goto nextNt_fun02_sec05_sub03_cat05;
                 } /*switch, reverse complement*/
               } /*If: checking transitions*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun02 Sec05 Sub03 Cat05:
               +   - move to next nucleotide
               \++++++++++++++++++++++++++++++++++++++++*/

               ++distSL;

               nextNt_fun02_sec05_sub03_cat05:;

               ++uiQry;
               ++uiRef;

               --refValSI;
               --qryValSI;

               --tmpSI;
            } /*Loop: count number of SNPs*/

            break;
         /*Case: known snp or could be snp*/

         /***********************************************\
         * Fun02 Sec05 Sub04:
         *   - check matches or same error type
         \***********************************************/

         /*cases I treat as same*/
         case 122: /*=/=*/
         case 146: /*I/I*/
         case 136: /*D/D*/
         /*Case: treat as same*/
            tmpSI =
               min_genMath(
                  refValSI,
                  qryValSI
               );

            refValSI -= tmpSI;
            qryValSI -= tmpSI;

            if(refSTPtr->cigTypeStr[siRefCig] != 'D')
            { /*If: not deletion event*/
               uiRef += tmpSI;
               uiQry += tmpSI;
               *overlapUI += tmpSI;
            } /*If: not deletion event*/

            break;
         /*Case: treat as same*/
 
         /***********************************************\
         * Fun02 Sec05 Sub05:
         *   - handel deletion cases
         \***********************************************/

         /*deletion cases*/
         case 145: /*D/M*/
         case 129: /*D/=*/
         case 156: /*D/X*/
         /*Case: deletion (only one sequence)*/
            tmpSI =
               min_genMath(
                  refValSI,
                  qryValSI
               );

            if(tmpSI >= (sint) indelLenUI)
            { /*If: keeping deletion event*/
               if(depthAryUI)
               { /*If: using a depth profile*/
                  if(refSTPtr->cigTypeStr[siRefCig] =='I')
                  { /*If: ref is insertion*/
                     if(depthAryUI[uiRef] >= minDepthUI)
                        distSL += tmpSI;
                  } /*If: ref is insertion*/

                  else
                     distSL += tmpSI;
               } /*If: using a depth profile*/

               else
                  distSL += tmpSI;

               ++(*indelEventsUI);
               *numIndelUI += tmpSI;
            } /*If: keeping deletion event*/

            if(refSTPtr->cigTypeStr[siRefCig] == 'D')
               uiQry += tmpSI;

            else
            { /*Else: query was a deletion*/
               uiRef += tmpSI;
               *overlapUI += tmpSI;
            } /*Else: query was a deletion*/

            qryValSI -= tmpSI;
            refValSI -= tmpSI;

            break;
         /*Case: deletion (only one sequence)*/

         /***********************************************\
         * Fun02 Sec05 Sub06:
         *   - handel insertion cases
         \***********************************************/

         case 150: /*I/M*/
         case 134: /*I/=*/
         case 161: /*I/X*/
         case 141: /*I/D*/
         /*Case: insertion (only one sequence)*/

            if(refSTPtr->cigTypeStr[siRefCig] == 'I')
            { /*If: reference had insertion*/
               tmpSI = refSTPtr->cigArySI[siRefCig];
               uiRef += tmpSI;
               refValSI = 0;
            } /*If: reference had insertion*/

            else
            { /*Else: query had insertion*/
               tmpSI = qrySTPtr->cigArySI[siQryCig];
               uiQry += tmpSI;
               qryValSI = 0;
            } /*Else: query had insertion*/

            if(tmpSI >= (sint) indelLenUI)
            { /*If: keeping query deletion*/
               if(depthAryUI)
               { /*If: using a depth profile*/
                  if(refSTPtr->cigTypeStr[siRefCig] =='I')
                  { /*If: ref is insertion*/
                     if(depthAryUI[uiRef] >= minDepthUI)
                        distSL += tmpSI;
                  } /*If: ref is insertion*/

                  else
                     distSL += tmpSI;
               } /*If: using a depth profile*/

               else
                  distSL += tmpSI;

               ++(*indelEventsUI);
               *numIndelUI += tmpSI;
            } /*If: keeping query deletion*/

            if(qrySTPtr->cigTypeStr[siRefCig] == 'D')
               *overlapUI += tmpSI;

            break;
         /*Case: insertion (only one sequence)*/
      } /*Switch: find mutation combination*/

      /**************************************************\
      * Fun02 Sec05 Sub07:
      *   - check if move to next cigar entry
      \**************************************************/

      if(refValSI == 0)
      { /*If: on next cigar entry*/
         ++siRefCig;
         refValSI = refSTPtr->cigArySI[siRefCig];
      } /*If: on next cigar entry*/

      if(qryValSI == 0)
      { /*If: on next cigar entry*/
         ++siQryCig;
         qryValSI = qrySTPtr->cigArySI[siQryCig];
      } /*If: on next cigar entry*/
   } /*Loop: get edit distance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec06:
   ^   - return the edit distance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun02_sec06;

   noOverlap_fun02_sec06:;
   distSL = def_noOverlap_edDist;
   goto ret_fun02_sec06;

   diffRef_fun02_sec06:;
   distSL = def_diffRef_edDist;
   goto ret_fun02_sec06;

   noSeq_fun02_sec06:;
   distSL = def_noSeq_edDist;
   goto ret_fun02_sec06;

   noMap_fun02_sec06:;
   distSL = def_noMap_edDist;
   goto ret_fun02_sec06;

   ret_fun02_sec06:;
   return distSL;
} /*readCmpDist_edDist*/

/*-------------------------------------------------------\
| Fun03: dist_edDist
|   - gets edit distances for mapped reference
|   - deletions and insertions are only counted if they
|     execed a minimum length.
| Input:
|   - qrySTPtr:
|     o pointer to samEntry structure with read (query) to
|       find the edit distance for
|   - refSTPtr:
|     o reference sequence, if provided checks 'M' cases
|     o use 0 to not check
|   - noTranBl:
|     o 1 ingore transversion (refSTPtr must not be 0)
|     o 0 keeps transversios (refSTPtr can be 0)
|   - indelLenUI:
|     o minimum length for a indel to count as an event
|   - minQUC:
|     o minimum q-score to not discard an snp
|   - numIndelUI:
|     o pointer to unisigned int to hold the number of
|       indels in edit distance
|   - indelEventsUI:
|     o pointer to unsigned int to hold the number of
|       indel events (times a group of indels occured)
| Output:
|   - Modifies:
|     o numIndelUI to have number indels kept
|     o indelEventsUI to have number of indel events
|     o overlapUI to hold number of overlapped bases
|   - Returns:
|     o edit distance for query and mapped reference
|     o negative value if could not find distance
|       - def_noSeq_edDist if one read is missing sequence
|       - def_noMap_edDist if one read is unmapped
\-------------------------------------------------------*/
signed long
dist_edDist(
   struct samEntry *qrySTPtr, /*read for edit distance*/
   struct seqST *refSTPtr,    /*has reference sequence*/
   signed char noTranBl,       /*1: ignore transversions*/
   unsigned int indelLenUI,   /*min indel length*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   unsigned int *numIndelUI,  /*number indels kept*/
   unsigned int *indelEventsUI /*nubmer indel events*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets edit distances between the query & reference
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if can get edit distance
   '   o fun03 sec03:
   '     - initialize and check if have q-score entreis
   '   o fun03 sec04:
   '     - find edit distance
   '   o fun03 sec06:
   '     - return the edit distance
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siQryCig = 0;  /*query cigar position*/
   sint qryValSI = 0;
   uint uiQry = 0;     /*query nucleotide on*/
   uint uiRef = 0;

   slong distSL = 0;    /*edit distance*/

   schar qryQBl = 0;
   uchar qUC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if can get edit distance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *indelEventsUI = 0;
   *numIndelUI = 0;

   if(qrySTPtr->flagUS & 4)
      goto noMap_fun03_sec06;

   if(
         ! qrySTPtr->seqStr
      || *qrySTPtr->seqStr == '*' 
      || *qrySTPtr->seqStr == '\0'
   ) goto noSeq_fun03_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - initialize and check if have q-score entreis
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         qrySTPtr->qStr[0] == '\0'
      ||
         (
             qrySTPtr->qStr[0] == '*'
          && qrySTPtr->qStr[1] != '\0'
         )
   ) qryQBl = 0;

   else
      qryQBl = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - find edit distance
   ^   o fun03 sec04 sub01:
   ^     - get past masking at start
   ^   o fun03 sec04 sub02:
   ^     - start loop and check if at end (hard/soft mask)
   ^   o fun03 sec04 note01:
   ^     - table of cigar entries added together
   ^   o fun03 sec04 sub03:
   ^     - ignore matches (or check if have snps)
   ^   o fun03 sec04 sub04:
   ^     - check if keeping snp
   ^   o fun03 sec04 sub05:
   ^     - handel deletion cases
   ^   o fun03 sec04 sub06:
   ^     - handel insertion cases
   ^   o fun03 sec04 sub07:
   ^     - check if move to next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - get past masking at start
   \*****************************************************/

   if(qrySTPtr->cigTypeStr[siQryCig] == 'S')
   { /*If: have softmasked region*/
      uiQry += qryValSI;
      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*If: have softmasked region*/

   if(qrySTPtr->cigTypeStr[siQryCig] == 'H')
   { /*If: have hardmaksed region*/
      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*If: have hardmasked region*/

   /*****************************************************\
   * Fun03 Sec04 Sub02:
   *   - start loop and check if at end (hard/soft mask)
   \*****************************************************/

   while(siQryCig < (sint) qrySTPtr->lenCigUI)
   { /*Loop: get edit distance*/
      if(qrySTPtr->cigTypeStr[siQryCig] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[siQryCig] == 'H')
         break; /*hard masking only at ends (finished)*/

      /**************************************************\
      * Fun03 Sec04 Sub03:
      *   - ignore matches (or check if have snps)
      \**************************************************/

      switch(qrySTPtr->cigTypeStr[siQryCig])
      { /*Switch: find mutation combination*/
         /*cases were I have to compare sequences*/
         case 'M':
         /*Case: snp or match*/
            if(refSTPtr)
               goto snp_fun03_sec04_sub04_cat01;
            else
               goto match_fun03_sec04_sub03;

         /*Case: snp or match*/

         case '=':
         /*Case: likley match (or no way to check)*/
            match_fun03_sec04_sub03:;
            uiQry += qryValSI;
            uiRef += qryValSI;
            break;
         /*Case: likley match (or no way to check)*/

         /***********************************************\
         * Fun03 Sec04 Sub04:
         *   - check if keeping snp
         *   o fun03 sec04 sub04 cat01:
         *     - check snp quality score + start loop
         *   o fun03 sec04 sub04 cat02:
         *     - ignore transitions (if ignoring)
         *   o fun03 sec04 sub04 cat03:
         *     - count differnce (if kept) & move on
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun03 Sec04 Sub04 Cat01:
         +   - check snp quality score + start loop
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'X':
         /*Case: snp*/
            snp_fun03_sec04_sub04_cat01:;

            while(qryValSI > 0)
            { /*Loop: count number of SNPs*/
               if(qryQBl)
               { /*If: have query q-score entry*/
                  qUC =
                       qrySTPtr->qStr[uiQry]
                     - def_adjQ_samEntry;

                  if(qUC < minQUC)
                     goto nextNt_fun03_sec04_sub03_cat03;
               } /*If: have query q-score entry*/

              /*+++++++++++++++++++++++++++++++++++++++++\
              + Fun03 Sec04 Sub04 Cat02:
              +   - ignore transitions (if ignoring)
              \+++++++++++++++++++++++++++++++++++++++++*/

               if(refSTPtr && noTranBl)
               { /*If: have reference*/
                  if(noTranBl)
                    switch(qrySTPtr->seqStr[uiQry] & ~32)
                    { /*switch, reverse complement*/
                      case 'A':
                      case 'G':
                      case 'R': /*A/G*/
                      /*Case: query is A or G*/
                       switch(
                           refSTPtr->seqStr[uiRef] & ~32
                       ){ /*Switch: query A, G or R (A/G)*/
                         case 'C': break;
                         case 'T': break;
                         case 'Y': break;
                         default: goto nextNt_fun03_sec04_sub03_cat03;
                       } /*Switch: query is A, G or Y (C/T)*/

                        break; /*is transversion*/
                      /*Case: query is A or G*/

                      case 'T':
                      case 'U':
                      case 'C':
                      case 'Y': /*T/C*/
                      /*Case: query is C, T/U, or Y (T/C)*/ 
                        switch(
                            refSTPtr->seqStr[uiRef]
                          & (~32)
                        ){ /*Switch: query c or t*/
                         case 'A': break; /*transversion*/
                         case 'G': break; /*transversion*/
                         case 'R': break; /*AG transversion*/
                         default: goto nextNt_fun03_sec04_sub03_cat03;
                        } /*Switch: query c or t*/

                        break; /*is transversion*/
                      /*Case: query is C, T/U, or R (A/G)*/

                      default: goto nextNt_fun03_sec04_sub03_cat03;
                    } /*switch, reverse complement*/
               } /*If: have reference*/

              /*+++++++++++++++++++++++++++++++++++++++++\
              + Fun03 Sec04 Sub04 Cat03:
              +   - count differnce (if kept) & move on
              \+++++++++++++++++++++++++++++++++++++++++*/

               ++distSL;

               nextNt_fun03_sec04_sub03_cat03:;
               ++uiQry;
               ++uiRef;
               --qryValSI;
            } /*Loop: count number of SNPs*/

            break;
         /*Case: snp*/

         /***********************************************\
         * Fun03 Sec04 Sub05:
         *   - handel deletion cases
         \***********************************************/

         /*deletion cases*/
         case 'D':
         /*Case: deletion*/
            if(qryValSI >= (sint) indelLenUI)
            { /*If: keeping deletion event*/
               distSL += qryValSI;
               ++(*indelEventsUI);
               *numIndelUI += qryValSI;
            } /*If: keeping deletion event*/

            uiRef += qryValSI;
            break;
         /*Case: deletion*/

         /***********************************************\
         * Fun03 Sec04 Sub06:
         *   - handel insertion cases
         \***********************************************/

         case 'I':
         /*Case: insertion*/
            uiQry += qryValSI;

            if(qryValSI >= (sint) indelLenUI)
            { /*If: keeping query deletion*/
               distSL += qryValSI;
               ++(*indelEventsUI);
               *numIndelUI += qryValSI;
            } /*If: keeping query deletion*/

            break;
         /*Case: insertion*/
      } /*Switch: find mutation combination*/

      /**************************************************\
      * Fun03 Sec04 Sub07:
      *   - check if move to next cigar entry
      \**************************************************/

      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*Loop: get edit distance*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec06:
   ^   - return the edit distance
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun03_sec06;

   noSeq_fun03_sec06:;
   distSL = def_noSeq_edDist;
   goto ret_fun03_sec06;

   noMap_fun03_sec06:;
   distSL = def_noMap_edDist;
   goto ret_fun03_sec06;

   ret_fun03_sec06:;
   return distSL;
} /*dist_edDist*/

/*-------------------------------------------------------\
| Fun04: percDist_edDist
|   - gives a rough idea on precentage of difference from
|     error
|   - not great, but allows lumping reads together
| Input:
|   - distSL:
|     o edit distance to find probablity for
|   - overlapUI:
|     o length of alignment (overlap between query & ref)
|   - percErrF:
|     o expected percent of errors (0 to 1) in reads
|     o if doing read to read comparsions; double this
| Output:
|   - Returns:
|     o probablity of edit distance due to chance
\-------------------------------------------------------*/
signed int
percDist_edDist(
   signed long distSL,
   unsigned int overlapUI,
   float percErrF
){
   return 100 * (distSL / (overlapUI * percErrF));
} /*percDist_edDist*/

/*-------------------------------------------------------\
| Fun05: addReadToDepth_edDist
|   - adds a read to a depth profile
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - qrySTPtr:
|     o query to add to profile
|   - minQUC:
|     o minimum q-score to keep snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - depthAryUI:
|     o unsigned int array of read depths (must be length
|       of reference)
| Output:
|   - Modifies:
|     o depthAryUI to have query added
|   - Returns:
|     o 0 for no errors
|     o 1 if read was not added (failed filters)
\-------------------------------------------------------*/
signed char
addReadToDepth_edDist(
   struct samEntry *refSTPtr, /*reference samEntry*/
   struct samEntry *qrySTPtr, /*query samEntry*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   unsigned int *depthAryUI   /*array of read depths*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - adds a read to a depth profile
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - filter reads
   '   o fun05 sec03:
   '     - prepare for comparisions
   '   o fun05 sec04:
   '     - add depths
   '   o fun05 sec05:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   float overlapF = 0;  /*finding overlap*/

   sint siQryCig = 0;  /*query cigar position*/
   sint siRefCig = 0;  /*reference cigar position*/
   sint refValSI = 0;  /*number reference bases in cigar*/
   sint qryValSI = 0;  /*number query bases in cigar*/

   sint tmpSI = 0;

   uint uiQry = 0;     /*query nucleotide on*/
   uint uiRef = 0;     /*reference nucleotide on*/
   uchar qUC = 0;      /*for q-score finding*/

   schar qryQBl = 0;    /*tells if query has q-score*/
   schar refQBl = 0;    /*tells if ref has q-score*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - filter reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qrySTPtr->extraStr[0] == '@')
      goto discard_fun05_sec05;

   if(qrySTPtr->flagUS & 4)
      goto discard_fun05_sec05;
   
   if(qrySTPtr->refEndUI < refSTPtr->refStartUI)
      goto discard_fun05_sec05;

   if(qrySTPtr->refStartUI > refSTPtr->refEndUI)
      goto discard_fun05_sec05;

   if(qrySTPtr->refEndUI < refSTPtr->refStartUI)
      goto discard_fun05_sec05;

   overlapF =
     (float)
     min_genMath(
        qrySTPtr->refEndUI,
        refSTPtr->refEndUI
     );

   overlapF -=
     (float)
     max_genMath(
        qrySTPtr->refStartUI,
        refSTPtr->refStartUI
     );

   if(
             overlapF / (float) refSTPtr->alnReadLenUI
           < minOverlapF
      &&
             overlapF / (float) qrySTPtr->alnReadLenUI
           < minOverlapF
   ) goto discard_fun05_sec05;

   if(
         ! qrySTPtr->seqStr
      || *qrySTPtr->seqStr == '*' 
      || *qrySTPtr->seqStr == '\0'
   ) goto discard_fun05_sec05;

   if(
         ! refSTPtr->seqStr
      || *refSTPtr->seqStr == '*' 
      || *refSTPtr->seqStr == '\0'
   ) goto discard_fun05_sec05;

   if(
      eql_ulCp(
         qrySTPtr->refIdStr,
         refSTPtr->refIdStr,
         0,
         '\0'
      )
   ) goto discard_fun05_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - prepare for comparisions
   ^   o fun05 sec03 sub01:
   ^     - check if have q-score entries
   ^   o fun05 sec03 sub02:
   ^     - find start positions
   ^   o fun05 sec03 sub03:
   ^     - move past softmasking
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - check if have q-score entries
   \*****************************************************/

   if(
         qrySTPtr->qStr[0] == '\0'
      ||
         (
             qrySTPtr->qStr[0] == '*'
          && qrySTPtr->qStr[1] == '\0'
         )
   ) qryQBl = 0;

   else
      qryQBl = 1;

   if(
         refSTPtr->qStr[0] == '\0'
      ||
         (
             refSTPtr->qStr[0] == '*'
          && refSTPtr->qStr[1] == '\0'
         )
   ) refQBl = 0;

   else
      refQBl = 1;

   /*****************************************************\
   * Fun05 Sec03 Sub02:
   *   - find start positions
   \*****************************************************/

   refValSI = refSTPtr->cigArySI[0];
   qryValSI = qrySTPtr->cigArySI[0];

   if(qrySTPtr->refStartUI > refSTPtr->refStartUI)
   { /*If: move the reference foward*/
      uiQry = refSTPtr->refStartUI;

      findRefPos_samEntry(
         refSTPtr,
         &siRefCig,
         &refValSI,
         qrySTPtr->refStartUI,
         (sint *) &uiQry, /*reference pos (discard)*/
         (sint *) &uiRef
      ); /*set reference to first query base*/

      uiQry = 0;
   } /*If: move reference foward*/

   else if(qrySTPtr->refStartUI < refSTPtr->refStartUI)
   { /*Else If: move query foward*/
      uiRef = qrySTPtr->refStartUI;

      findRefPos_samEntry(
         qrySTPtr,
         &siQryCig,
         &qryValSI,
         refSTPtr->refStartUI, /*end query position*/
         (sint *) &uiRef, /*query pos (discard)*/
         (sint *) &uiQry  /*end query position*/
      ); /*set reference to first query base*/

      uiRef = 0;
   } /*Else If: move query foward*/

   /*****************************************************\
   * Fun05 Sec03 Sub03:
   *   - move past softmasking
   \*****************************************************/

   if(refSTPtr->cigTypeStr[siRefCig] == 'S')
   { /*If: have softmasked region*/
      uiRef += refValSI;
      ++siRefCig;
      refValSI = refSTPtr->cigArySI[siRefCig];
   } /*If: have softmasked region*/

   if(qrySTPtr->cigTypeStr[siQryCig] == 'S')
   { /*If: have softmasked region*/
      uiQry += qryValSI;
      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*If: have softmasked region*/

   if(refSTPtr->cigTypeStr[siRefCig] == 'H')
   { /*If: have hardmasked region*/
      ++siRefCig;
      refValSI = refSTPtr->cigArySI[siRefCig];
   } /*If: have hardmasked region*/

   if(qrySTPtr->cigTypeStr[siQryCig] == 'H')
   { /*If: have hardmaksed region*/
      ++siQryCig;
      qryValSI = qrySTPtr->cigArySI[siQryCig];
   } /*If: have hardmasked region*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - add depths
   ^   o fun05 sec04 sub01:
   ^     - start loop + break if softmasking at end
   ^   o fun05 sec04 note01:
   ^     - table of cigar entries added together
   ^   o fun05 sec04 sub02:
   ^     - start switch/check match/possible snp combos
   ^   o fun05 sec04 sub03:
   ^     - check ins/ins or del/del
   ^   o fun05 sec04 sub04:
   ^     - handel deletion cases
   ^   o fun05 sec04 sub05:
   ^     - handel insertion cases
   ^   o fun05 sec04 sub06:
   ^     - check if move to next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec04 Sub01:
   *   - start loop + break if softmasking at end
   \*****************************************************/

   while(
         siQryCig < (sint) qrySTPtr->lenCigUI
      && siRefCig < (sint) refSTPtr->lenCigUI
   ){ /*Loop: add depths*/
      if(refSTPtr->cigTypeStr[siRefCig] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[siQryCig] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(refSTPtr->cigTypeStr[siRefCig] == 'H')
         break; /*hard masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[siQryCig] == 'H')
         break; /*hard masking only at ends (finished)*/

      /*`````````````````````````````````````````````````\
      ` Fun05 Sec04 Note01:
      `   - table of cigar entries added together
      \`````````````````````````````````````````````````*/

      /*Values:
      ` =: 61 (match)
      ` X: 88 (snp)
      ` M: 77 (match/snp)
      ` D: 68 (deletion)
      ` I: 73 (insertion)
      ` M + M: 154
      ` = + =: 122
      ` X + X: 176 (need to check if snps agree/disagree)
      ` I + I: 146 (check size; rm low q-scores)
      ` D + D: 136 (check size)
      `
      ` X + M: 165 (need to check if agree/disagree)
      ` = + M: 138 (need to check if agree/disagree)
      ` = + X: 149 (disagree, check q-scores)
      `
      ` D + M: 145 (disagree, check size)
      ` D + =: 129 (disagree, check size)
      ` D + X: 156 (disagree, check siz)
      `
      ` I + M: 150
      ` I + =: 134
      ` I + X: 161
      ` I + D: 141
      */

      /**************************************************\
      * Fun05 Sec04 Sub02:
      *   - start switch/check match/possible snp combos
      *   o fun05 sec04 sub02 cat01:
      *     - find number shared snps/matchs + check case
      *   o fun05 sec04 sub02 cat02:
      *     - find snps passing min quality scores
      *   o fun05 sec04 sub02 cat03:
      *     - check if SNP (passed) is a transversion
      *   o fun05 sec04 sub02 cat04:
      *     - move to next SNP/match
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec04 Sub02 Cat01:
      +   - find number shared snps/matchs + check case
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      switch(
           refSTPtr->cigTypeStr[siRefCig]
         + qrySTPtr->cigTypeStr[siQryCig]
      ){ /*Switch: find mutation combination*/
         /*cases were I have to compare sequences*/
         case 154: /*M/M*/
         case 176: /*X/X*/
         case 165: /*X/M*/
         case 138: /*=/M*/
         case 149: /*=/X*/
         case 122: /*=/=*/
         /*Case: match or snp*/
            tmpSI =
               min_genMath(
                  refValSI,
                  qryValSI
               );

            while(tmpSI > 0)
            { /*Loop: add depth for matches*/
               if(
                     refSTPtr->seqStr[uiRef]
                  != qrySTPtr->seqStr[uiQry]
               ) goto nextNt_fun05_sec04_sub02_cat02;

               else if(refSTPtr->seqStr[uiRef] == 'N')
                  goto nextNt_fun05_sec04_sub02_cat02;

               else if(qrySTPtr->seqStr[uiQry] == 'N')
                  goto nextNt_fun05_sec04_sub02_cat02;

               if(qryQBl)
               { /*If: have query q-score entry*/
                  qUC =
                       qrySTPtr->qStr[uiQry]
                     - def_adjQ_samEntry;

                  if(qUC < minQUC)
                     goto nextNt_fun05_sec04_sub02_cat02;
               } /*If: have query q-score entry*/

               if(refQBl)
               { /*If: have reference q-score entry*/
                  qUC =
                       refSTPtr->qStr[uiRef]
                     - def_adjQ_samEntry;

                  if(qUC < minQUC)
                     goto nextNt_fun05_sec04_sub02_cat02;
               } /*If: have reference q-score entry*/

               ++depthAryUI[uiRef];

               nextNt_fun05_sec04_sub02_cat02:;

               ++uiQry;
               ++uiRef;

               --refValSI;
               --qryValSI;

               --tmpSI;
            } /*Loop: add depth for matches*/

            break;
         /*Case: match or snp*/

         /***********************************************\
         * Fun05 Sec04 Sub03:
         *   - check ins/ins or del/del
         \***********************************************/

         /*cases I treat as same*/
         case 146: /*I/I*/
         case 136: /*D/D*/
         /*Case: treat as same*/
            tmpSI =
               min_genMath(
                  refValSI,
                  qryValSI
               );

            refValSI -= tmpSI;
            qryValSI -= tmpSI;

            if(refSTPtr->cigTypeStr[siRefCig] != 'D')
            { /*If: not deletion event*/
               uiQry += tmpSI;

               while(tmpSI > 0)
               { /*Loop: add in depths*/
                  ++depthAryUI[uiRef];
                  --tmpSI;
                  ++uiRef;
               } /*Loop: add in depths*/
            } /*If: not deletion event*/

            break;
         /*Case: treat as same*/
 
         /***********************************************\
         * Fun05 Sec04 Sub04:
         *   - handel deletion cases
         \***********************************************/

         /*deletion cases*/
         case 145: /*D/M*/
         case 129: /*D/=*/
         case 156: /*D/X*/
         /*Case: deletion (only one sequence)*/
            tmpSI =
               min_genMath(
                  refValSI,
                  qryValSI
               );

            if(refSTPtr->cigTypeStr[siRefCig] == 'D')
               uiQry += tmpSI;
            else
               uiRef += tmpSI;

            qryValSI -= tmpSI;
            refValSI -= tmpSI;

            break;
         /*Case: deletion (only one sequence)*/

         /***********************************************\
         * Fun05 Sec04 Sub05:
         *   - handel insertion cases
         \***********************************************/

         case 150: /*I/M*/
         case 134: /*I/=*/
         case 161: /*I/X*/
         case 141: /*I/D*/
         /*Case: insertion (only one sequence)*/

            if(refSTPtr->cigTypeStr[siRefCig] == 'I')
            { /*If: reference had insertion*/
               tmpSI = refSTPtr->cigArySI[siRefCig];
               uiRef += tmpSI;
               refValSI = 0;
            } /*If: reference had insertion*/

            else
            { /*Else: query had insertion*/
               tmpSI = qrySTPtr->cigArySI[siQryCig];
               uiQry += tmpSI;
               qryValSI = 0;
            } /*Else: query had insertion*/

            break;
         /*Case: insertion (only one sequence)*/
      } /*Switch: find mutation combination*/

      /**************************************************\
      * Fun05 Sec04 Sub06:
      *   - check if move to next cigar entry
      \**************************************************/

      if(refValSI == 0)
      { /*If: on next cigar entry*/
         ++siRefCig;
         refValSI = refSTPtr->cigArySI[siRefCig];
      } /*If: on next cigar entry*/

      if(qryValSI == 0)
      { /*If: on next cigar entry*/
         ++siQryCig;
         qryValSI = qrySTPtr->cigArySI[siQryCig];
      } /*If: on next cigar entry*/
   } /*Loop: add depths*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec05:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   discard_fun05_sec05:;
   return 1;
} /*addReadToDepth_edDist*/

/*-------------------------------------------------------\
| Fun06: mkDepthProfile_edDist
|   - finds the depth of every base in samEntry reference
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - minQUC:
|     o minimum q-score to keep snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - samSTPtr:
|     o for reading each line in the sam file
|   - buffStrPtr:
|     o pointer to c-string with buffer for reading file
|   - lenBuffULPtr:
|     o pointer to unsigned long to hold buffStrPtr size
|   - samFILE:
|     o sam file to scan
|   - errSCPtr:
|     o pointer to signed char to hold error type
| Output:
|   - Modifies:
|     o samSTPtr to have last entry in file
|     o samFILE to point to start
|     o buffStrPtr to have last line in sam file and is
|       resized if needed
|     o lenBuffULPtr to new buffStrPtr size (if resized)
|     o errSCPtr to:
|       - 0 for no errors
|       - def_memErr_edDist for memory errors
|       - def_fileErr_edDist for memory errors
|   - Returns:
|     o unsigned int array with depths
|     o 0 for memory error
\-------------------------------------------------------*/
unsigned int *
mkDepthProfile_edDist(
   struct samEntry *refSTPtr, /*reference samEntry*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   struct samEntry *samSTPtr, /*for reading sam file*/
   signed char **buffStrPtr,  /*for reading sam file*/
   unsigned long *lenBuffULPtr, /*size of buffStrPtr*/
   void *samFILE,
   signed char *errSCPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun06 sec01:
   '     - variable declarations
   '   o fun06 sec02:
   '     - allocate memory and get first line
   '   o fun06 sec03:
   '     - get depths
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int *depthHeapAryUI = 0; /*depth array*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - allocate memory and get first line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   depthHeapAryUI =
      calloc(
         (refSTPtr->readLenUI + 9),
         sizeof(uint)
      );

   if(! depthHeapAryUI)
      goto memErr_fun06_sec04_sub02;

   *errSCPtr =
      get_samEntry(
         samSTPtr,
         buffStrPtr,
         lenBuffULPtr,
         samFILE
      );

   if(*errSCPtr)
   { /*If: had error*/
      if(*errSCPtr == def_memErr_samEntry)
         goto memErr_fun06_sec04_sub02;

      else
         goto fileErr_fun06_sec04_sub03;
   } /*If: had error*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - get depths
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(! *errSCPtr)
   { /*Loop: get depths*/

      addReadToDepth_edDist(
         refSTPtr,
         samSTPtr,
         minQUC,
         minOverlapF,
         depthHeapAryUI
      );

      *errSCPtr =
         get_samEntry(
            samSTPtr,
            buffStrPtr,
            lenBuffULPtr,
            samFILE
         );
   } /*Loop: get depths*/

   if(*errSCPtr == def_memErr_samEntry)
      goto memErr_fun06_sec04_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec04:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun06_sec04_sub05;

   memErr_fun06_sec04_sub02:;
   *errSCPtr = def_memErr_edDist;
   goto errCleanUp_fun06_sec04_sub04;

   fileErr_fun06_sec04_sub03:;
   *errSCPtr = def_fileErr_edDist;
   goto errCleanUp_fun06_sec04_sub04;

   errCleanUp_fun06_sec04_sub04:;
   if(depthHeapAryUI)
      free(depthHeapAryUI);
   depthHeapAryUI = 0;
   goto ret_fun06_sec04_sub05;

   ret_fun06_sec04_sub05:;

   fseek(
      samFILE,
      0,
      SEEK_SET
   );

   return depthHeapAryUI;
} /*mkDepthProfile_edDist*/
   
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
