/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' profClust SOF: Start Of File
'   - clusters reads using variant profiles from reference
'   o header:
'     - included libraries
'   o fun27: addRead_var_clustST
'     - adds the variants in a read to var_clustST
'   o fun28: addReadRef_var_clustST
'     - adds the variants in a read to var_clustST
'       struct using the input reference
'   o fun29: mk_var_clustST
'     - builds an array of var_clustST struct for
'       references in sam file
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

#include "profClust.h"

#include <stdio.h>

#include "../genLib/samEntry.h"
#include "../genLib/edDist.h"
#include "../edClustSrc/edClust.h"
#include "../tbConSrc/tbCon.h"

#include "../genClust/clustST.h"

/*.h files only (no .c files)*/
#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h" /*only using .h min macro*/
#include "../tbConSrc/tbConDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/charCp.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun27: addRead_var_clustST
|   - adds the variants in a read to var_clustST struct
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with read to add
|   - varSTPtr:
|     o pointer to var_clustST struct to add read
|       variants to
|   - setSTPtr:
|     o pointer to a set_clustST struct with settings
| Output:
|   - Mofifies:
|     o varSTPtr to have variants added to arrays
\-------------------------------------------------------*/
void
addRead_var_clustST(
   struct samEntry *samSTPtr,
   struct var_clustST *varSTPtr,
   struct set_clustST *setSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun27 TOC:
   '   - adds the variants in a read to var_clustST
   '   o fun27 sec01:
   '     - variable declarations
   '   o fun27 sec02:
   '     - add variants to variant profile
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint tmpSI = 0;
   uint uiCig = 0;
   uint seqUI = 0;
   uint refUI = 0;

   uchar qUC = 0;
   schar qBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec02:
   ^   - check if read maps + setup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(samSTPtr->flagUS & 4)
      return;

   if(
         ! samSTPtr->seqStr
      || *samSTPtr->seqStr == '*' 
      || *samSTPtr->seqStr == '\0'
   ) return;

   /*see if have q-score entry*/
   if(
         samSTPtr->qStr[0] == '\0'
      ||
         (
             samSTPtr->qStr[0] == '*'
          && samSTPtr->qStr[1] == '\0'
         )
   ) qBl = 0;

   else
      qBl = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun27 Sec03:
   ^   - add variants to variant profile
   ^   o fun27 sec03 sub01:
   ^     - start loop and move past soft masking
   ^   o fun27 sec03 sub02:
   ^     - check cigar type + handel maches
   ^   o fun27 sec03 sub03:
   ^     - handel mismatches
   ^   o fun27 sec03 sub04:
   ^     - handel deletions
   ^   o fun27 sec03 sub05:
   ^     - handel insertions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun27 Sec03 Sub01:
   *   - start loop and move past soft masking
   \*****************************************************/

   for(
      uiCig = 0;
      uiCig < samSTPtr->lenCigUI;
      ++uiCig
   ){ /*Loop: find variants*/
      if(samSTPtr->cigTypeStr[uiCig] == 'S')
      { /*If: have softmasking*/
         if(uiCig != 0)
            break; /*end of cigar*/

         seqUI += samSTPtr->cigTypeStr[uiCig];
         continue; /*start of cigar*/
      } /*If: have softmasking*/

      else if(samSTPtr->cigTypeStr[uiCig] == 'H')
      { /*If: have hardmasking*/
         if(uiCig != 0)
            break; /*end of cigar*/

         continue; /*start of cigar*/
      } /*If: have hardmasking*/

      /**************************************************\
      * Fun27 Sec03 Sub02:
      *   - check cigar type + handel maches
      \**************************************************/

      switch(samSTPtr->cigTypeStr[uiCig])
      { /*Switch: check error type*/
         case '=':
         case 'M':
         /*Case: match or no idea (do not record)*/
            seqUI += samSTPtr->cigArySI[uiCig];
            refUI += samSTPtr->cigArySI[uiCig];
            break; /*match; do nothing*/
         /*Case: match or no idea (do not record)*/

         /***********************************************\
         * Fun27 Sec03 Sub03:
         *   - handel mismatches
         \***********************************************/

         case 'X':
         /*Case: mismatch*/
            tmpSI = samSTPtr->cigArySI[uiCig];

            while(tmpSI > 0)
            { /*Loop: check if keep snp*/
               qUC = samSTPtr->qStr[posUI];
               qUC -= def_adjQ_samEntry;

               if(
                        qUC
                     >= setSTPtr->set_edClustST->minSnpQUC
                  || ! qBl
               ){ /*If: keeping snp*/
                  switch(samSTPtr->seqStr[uiPos] & ~32)
                  { /*Switch: find snp type*/
                     case 'A':
                        ++varSTPtr->aAryUI[refPosUI];
                        break;

                     case 'C':
                        ++varSTPtr->cAryUI[refPosUI];
                        break;

                     case 'G':
                        ++varSTPtr->gAryUI[refPosUI];
                        break;

                     case 't':
                        ++varSTPtr->tAryUI[refPosUI];
                        break;

                     /*else anoymous (do not care)*/
                  } /*Switch: find snp type*/
               } /*If: keeping snp*/

               ++seqUI;
               ++posUI;
               --tmpSI;
            } /*Loop: check if keep snp*/
         /*Case: mismatch*/

         /***********************************************\
         * Fun27 Sec03 Sub04:
         *   - handel deletions
         \***********************************************/

         case 'D':
         /*Case: deletion*/
            if(
                  samSTPtr->cigArySI[uiCig]
               >= profSTPtr->set_edClustST->indelLenUI
            ) ++varSTPtr->delAryUI[refPosUI];
            
            ++refUI;
         /*Case: deletion*/

         /***********************************************\
         * Fun27 Sec03 Sub05:
         *   - handel insertions
         \***********************************************/

         case 'I':
         /*Case: insertion*/
            if(
                  samSTPtr->cigArySI[uiCig]
               >= profSTPtr->set_edClustST->indelLenUI
            ) ++varSTPtr->insAryUI[refPosUI];
            
            ++seqUI;
         /*Case: insertion*/
      } /*Switch: check error type*/
   } /*Loop: find variants*/
} /*addRead_var_clustST*/

/*-------------------------------------------------------\
| Fun28: addReadRef_var_clustST
|   - adds the variants in a read to var_clustST struct
|     using the input reference
| Input:
|   - qrySTPtr:
|     o pointer to samEntry struct with read to add
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - varSTPtr:
|     o pointer to var_clustST struct to add read
|       variants to
|   - setSTPtr:
|     o pointer to a set_clustST struct with settings
| Output:
|   - Mofifies:
|     o varSTPtr to have variants added to arrays
\-------------------------------------------------------*/
void
addReadRef_var_clustST(
   struct samEntry *qrySTPtr,
   struct samEntry *refSTPtr,
   struct var_clustST *varSTPtr,
   struct set_clustST *setSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun28 TOC:
   '   o fun28 sec01:
   '     - variable declarations
   '   o fun28 sec02:
   '     - see if reads overlap
   '   o fun28 sec03:
   '     - check if have q-score entry
   '   o fun28 sec04:
   '     - find start of overlap
   '   o fun28 sec06:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint qryCigUI = 0; /*query cigar entry on*/
   sint qryValSI = 0; /*number bases in qry cigar entry*/
   uint qryPosUI = 0; /*position at on query*/
   schar qryQBl = 0;  /*marks if qry has q-score entry*/

   uint refCigUI = 0; /*reference cigar entry on*/
   sint refValSI = 0; /*number bases in ref cigar entry*/
   uint refPosUI = 0; /*current position on reference*/
   schar refQBl = 0;  /*marks if ref has q-score entry*/

   uchar qUC = 0;     /*q-score for query/reference*/
   sint tmpSI = 0;
   overlapF = 0;      /*for finding ovlerlap*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec02:
   ^   - see if reads overlap
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(qrySTPtr->flagUS & 4)
      goto notAdded_fun28_sec06;

   if(refSTPtr->flagUS & 4)
      goto notAdded_fun28_sec06;

   if(qrySTPtr->refEndUI < refSTPtr->refStartUI)
      goto notAdded_fun28_sec06;

   if(qrySTPtr->refStartUI > refSTPtr->refEndUI)
      goto notAdded_fun28_sec06;

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
           < setSTPtr->set_edClustST->percOverlapF
      &&
             overlapF / (float) qrySTPtr->alnReadLenUI
           < setSTPtr->set_edClustST->percOverlapF
   ) goto notAdded_fun28_sec06;

   if(
         ! qrySTPtr->seqStr
      || *qrySTPtr->seqStr == '*' 
      || *qrySTPtr->seqStr == '\0'
   ) goto notAdded_fun28_sec06;

   if(
         ! refSTPtr->seqStr
      || *refSTPtr->seqStr == '*' 
      || *refSTPtr->seqStr == '\0'
   ) goto notAdded_fun28_sec06;

   if(
      eql_ulCp(
         qrySTPtr->refIdStr,
         refSTPtr->refIdStr,
         0,
         '\0'
      )
   ) goto notAdded_fun28_sec06;
     /*reads are mapped to different references*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec03:
   ^   - check if have q-score entry
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
   ^ Fun28 Sec04:
   ^   - find start of overlap
   ^   o fun28 sec04 sub01:
   ^     - move past soft and hard masking
   ^   o fun28 sec04 sub02:
   ^     - find first overlapping base
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun28 Sec04 Sub01:
   *   - move past soft and hard masking
   \*****************************************************/

   refValSI = refSTPtr->cigArySI[0];
   qryValSI = qrySTPtr->cigArySI[0];

   if(qrySTPtr->cigTypStr[qryCigUI] == 'S')
   { /*If: query softmasking*/
      qryPosUI += qryValSL;
      ++qryCigUI;
      qryValSI = qrySTPtr->cigArySI[qryCigUI];
   } /*If: query softmasking*/

   else if(qrySTPtr->cigTypStr[qryCigUI] == 'H')
   { /*If: query hard masking*/
      ++qryCigUI;
      qryValSI = qrySTPtr->cigArySI[qryCigUI];
   } /*If: query hard masking*/


   if(refSTPtr->cigTypStr[refCigUI] == 'S')
   { /*If: query softmasking*/
      refPosUI += refValSL;
      ++refCigUI;
      refValSI = refSTPtr->cigArySI[refCigUI];
   } /*If: query softmasking*/

   else if(refSTPtr->cigTypStr[refCigUI] == 'H')
   { /*If: query hard masking*/
      ++refCigUI;
      refValSI = refSTPtr->cigArySI[refCigUI];
   } /*If: query hard masking*/


   /*****************************************************\
   * Fun28 Sec04 Sub02:
   *   - find first overlapping base
   \*****************************************************/

   if(qrySTPtr->refStartUI > refSTPtr->refStartUI)
   { /*If: move the reference foward*/
       qryPosUI = refSTPtr->refStartUI;

      findRefPos_samEntry(
         refSTPtr,
         &qryCigSI,
         &refValSI,
         qrySTPtr->refStartUI,
         (sint *) &qryPosUI, /*reference pos (discard)*/
         (sint *) &refPosUI
      ); /*set reference to first query base*/

      qryPosUI = 0;
   } /*If: move reference foward*/

   else if(qrySTPtr->refStartUI < refSTPtr->refStartUI)
   { /*Else If: move query foward*/
      refPosUI = qrySTPtr->refStartUI;

      findRefPos_samEntry(
         qrySTPtr,
         &qryCigSI,
         &qryValSI,
         refSTPtr->refStartUI, /*end query position*/
         (sint *) &refPosUI, /*query pos (discard)*/
         (sint *) &qryPosUI  /*end query position*/
      ); /*set reference to first query base*/

      refPosUI = 0;
   } /*Else If: move query foward*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec05:
   ^   - get variant profile
   ^   o fun28 sec05 sub01:
   ^     - start loop and break at masking
   ^   o fun28 sec05 note01:
   ^     - table of cigar entries added together
   ^   o fun28 sec05 sub02:
   ^     - start switch/check match/possible snp combos
   ^   o fun28 sec05 sub03:
   ^     - check matches or same error type
   ^   o fun28 sec05 sub04:
   ^     - handel deletion cases
   ^   o fun28 sec05 sub05:
   ^     - handel insertion cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun28 Sec05 Sub01:
   *   - start loop and break at masking
   \*****************************************************/

   while(
         qryCigUI < (sint) qrySTPtr->lenCigUI
      && refCigUI < (sint) refSTPtr->lenCigUI
   ){ /*Loop: add variants to profile*/
      if(refSTPtr->cigTypeStr[refCigUI] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[qryCigUI] == 'S')
         break; /*soft masking only at ends (finished)*/

      if(refSTPtr->cigTypeStr[refCigUI] == 'H')
         break; /*hard masking only at ends (finished)*/

      if(qrySTPtr->cigTypeStr[qryCigUI] == 'H')
         break; /*hard masking only at ends (finished)*/

      /*`````````````````````````````````````````````````\
      ` Fun28 Sec05 Note01:
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
      * Fun28 Sec05 Sub02:
      *   - start switch/check match/possible snp combos
      *   o fun28 sec05 sub02 cat01:
      *     - find number shared snps/matchs + check case
      *   o fun28 sec05 sub02 cat02:
      *     - filter out matches and low quality variants
      *   o fun28 sec05 sub02 cat03:
      *     - add variant to profile
      *   o fun28 sec05 sub02 cat04:
      *     - move to next SNP/match
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun28 Sec05 Sub02 Cat01:
      +   - switch + find number shared snps/matchs
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      switch(
           refSTPtr->cigTypeStr[refCigUI]
         + qrySTPtr->cigTypeStr[qryCigUI]
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

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun28 Sec05 Sub02 Cat02:
            +   - filter out matches and low quality vars
            \+++++++++++++++++++++++++++++++++++++++++++*/

            while(tmpSI > 0)
            { /*Loop: count number of SNPs*/
               if(
                     refSTPtr->seqStr[refPosUI]
                  == qrySTPtr->seqStr[qryPosUI]
               ) goto nextNt_fun28_sec05_sub02_cat04;

               if(qryQBl)
               { /*If: have query q-score entry*/
                  qUC =
                       qrySTPtr->qStr[qryPosUI]
                     - def_adjQ_samEntry;

                  if(qUC < setSTPtr->minSnpQUC)
                     goto nextNt_fun28_sec05_sub02_cat04;
               } /*If: have query q-score entry*/

               if(refQBl)
               { /*If: have reference q-score entry*/
                  qUC =
                       refSTPtr->qStr[refPosUI]
                     - def_adjQ_samEntry;

                  if(qUC < setSTPtr->minSnpQUC)
                    goto nextNt_fun28_sec05_sub02_cat04;
               } /*If: have reference q-score entry*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun28 Sec05 Sub02 Cat03:
               +   - add variant to profile
               \++++++++++++++++++++++++++++++++++++++++*/

               switch(qrySTPtr->seqStr[qryPosUI] & ~ 32)
               { /*Switch: check variant type*/
                  case 'A':
                     ++varSTPtr->aAryUI[refPosUI];
                     break;

                  case 'C':
                     ++varSTPtr->cAryUI[refPosUI];
                     break;

                  case 'G':
                     ++varSTPtr->gAryUI[refPosUI];
                     break;

                  case 't':
                     ++varSTPtr->tAryUI[refPosUI];
                     break;

                  /*else anoymous (do not care)*/
               } /*Switch: check variant type*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun28 Sec05 Sub02 Cat04:
               +   - move to next snp/match
               \++++++++++++++++++++++++++++++++++++++++*/

               nextNt_fun28_sec05_sub02_cat04:;

               ++qryPosUI;
               ++refPosUI;

               --refValSI;
               --qryValSI;

               --tmpSI;
            } /*Loop: count number of SNPs*/

            break;
         /*Case: known snp or could be snp*/

         /***********************************************\
         * Fun28 Sec05 Sub03:
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

            if(refSTPtr->cigTypeStr[refCigUI] != 'D')
            { /*If: not deletion event*/
               refPosUI += tmpSI;
               qryPosUI += tmpSI;
            } /*If: not deletion event*/

            break;
         /*Case: treat as same*/
 
         /***********************************************\
         * Fun28 Sec05 Sub04:
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

            if(
               >= tmpSI
                  (sint)
                  setSTPtr->set_edClustST->indelLenUI
            ){ /*If: keeping deletion event*/

               /*check which case was a deletion*/
               if(refSTPtr->cigTypeStr[refCigUI] == 'D')
               { /*If: reference was deletion*/
                  qryPosUI += tmpSI;
                  varSTPtr->insAryUI[refPosUI] += tmpSI;
               } /*If: reference was deletion*/

               else
               { /*Else: query was deletion*/
                  refPosUI += tmpSI;
                  varSTPtr->delAryUI[refPosUI] += tmpSI;
               } /*Else: query was deletion*/
            } /*If: keeping deletion event*/

            qryValSI -= tmpSI;
            refValSI -= tmpSI;

            break;
         /*Case: deletion (only one sequence)*/

         /***********************************************\
         * Fun28 Sec05 Sub05:
         *   - handel insertion cases
         \***********************************************/

         case 150: /*I/M*/
         case 134: /*I/=*/
         case 161: /*I/X*/
         case 141: /*I/D*/
         /*Case: insertion (only one sequence)*/

            if(refSTPtr->cigTypeStr[refCigUI] == 'I')
            { /*If: reference had insertion*/
               tmpSI = refSTPtr->cigArySI[refCigUI];
               refPosUI += tmpSI;
               refValSI = 0;
            } /*If: reference had insertion*/

            else
            { /*Else: query had insertion*/
               tmpSI = qrySTPtr->cigArySI[qryCigUI];
               qryPosUI += tmpSI;
               qryValSI = 0;
            } /*Else: query had insertion*/

            if(tmpSI >= (sint) indelLenUI)
            { /*If: keeping query deletion*/

               if(refSTPtr->cigTypeStr[refCigUI] =='I')
                  varSTPtr->delAryUI[refPosUI] += tmpSI;
               else
                  varSTPtr->insAryUI[refPosUI] += tmpSI;

            } /*If: keeping query deletion*/

            break;
         /*Case: insertion (only one sequence)*/
      } /*Switch: find mutation combination*/

      /**************************************************\
      * Fun28 Sec05 Sub07:
      *   - check if move to next cigar entry
      \**************************************************/

      if(refValSI == 0)
      { /*If: on next cigar entry*/
         ++refCigUI;
         refValSI = refSTPtr->cigArySI[refCigUI];
      } /*If: on next cigar entry*/

      if(qryValSI == 0)
      { /*If: on next cigar entry*/
         ++qryCigUI;
         qryValSI = qrySTPtr->cigArySI[qryCigUI];
      } /*If: on next cigar entry*/
   } /*Loop: add variants to profile*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun28 Sec06:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return;

   goto notAdded_fun28_sec06:;
   return;
} /*addReadRef_var_clustST*/

/*-------------------------------------------------------\
| Fun29: mk_var_clustST
|   - builds an array of var_clustST struct for
|     references in sam file
| Input:
|   - indexSTPtr:
|     o pointer to index_edClust struct with the index
|       for the sam file and marks for discarded reads
|   - setSTPtr:
|     o pointer to set_clustST struct with settings for
|       detecting variants
|   - refSTPtr;
|     o pointer to samEntry struct with reference to
|       get varaints for
|     o or 0 if not using reference
|   - lenVarUIPtr;
|     o pointer to unsigned long to hold returned array
|       length
|   - samSTPtr:
|     o pointer to samEntry struct for reading file
|   - buffStr:
|     o c-string to use as buffer for reading sam file
|   - samFILE:
|     o sam file to build var_clustST for
\-------------------------------------------------------*/
struct var_clustST *
mk_var_clustST(
   struct index_edClust *indexSTPtr,/*index for sam file*/
   struct set_clustST *setSTPtr,/*settings*/
   struct samEntry *refSTPtr,   /*reference*/
   unsigned int *lenVarUIPtr,   /*holds ret array length*/
   struct samEntry *samSTPtr,   /*for reading samFILE*/
   signed char *buffStr,        /*for reading samFILE*/
   void *samFILE                /*has reads for profile*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun29 TOC:
   '   - builds var_clustST struct for mapped reference
   '   o fun29 Sec01:
   '     - variable declarations
   '   o fun29 Sec02:
   '     - allocate memory
   '   o fun29 Sec03:
   '     - add reads to variant array
   '   o fun29 Sec04:
   '     - remove low read depth variants
   '   o fun29 Sec05:
   '     - return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   ulong lineUL = 0;
   ulong offsetUL = 0;
   uint refNumUI = 0;

   float refOverlapF = 0;
   float qryOverlapF = 0;

   struct var_clustST *varHeapAryST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec02:
   ^   - allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! refSTPtr)
   { /*If: not building for a specific reference*/
      *lenVarUIPtr = indexSTPtr->numRefUI;

      varHeapAryST =
         malloc(
              indexSTPtr->numRefUI
            * sizeof(struct var_clustST)
         );

      if(! varHeapAryST)
         goto err_fun29_sec0x;

      for(
         lineUL = 0;
         lineUL < indexSTPtr->numRefUI;
         ++lineUL
      ) init_var_clustST(&varHeapAryST[lineUL]);
   } /*If: not building for a specific reference*/

   else
   { /*Else: user provided a reference*/
      *lenVarUIPtr = 1;

      varHeapAryST = malloc(sizeof(struct var_clustST));

      if(! varHeapAryST)
         goto err_fun29_sec0x;

      init_var_clustST(varHeapAryST);

      refNumUI =
         find_strAry(
            indexSTPtr->refIdAryStr,
            refSTPtr->refIdStr,
            indexSTPtr->numRefUI
         ); /*find assigned reference number*/
   } /*Else: user provided a reference*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec03:
   ^   - add reads to variant array
   ^   o fun29 sec03 sub01:
   ^     - filter out reads (independent of reference)
   ^   o fun29 sec03 sub02:
   ^     - filter out reads (user provided reference)
   ^   o fun29 sec03 sub03:
   ^     - get reads passing filters
   ^   o fun29 sec03 sub04:
   ^     - add reads to varaint arrays (var_clustST)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun29 Sec03 Sub01:
   *   - filter out reads (independent of reference)
   \*****************************************************/

   for(
      lineUL = 0;
      lineUL < (slong) indexSTPtr->lenSamUL;
      ++lineUL
   ){ /*Loop: find best score*/

      if(indexSTPtr->clustArySI[lineUL] < 0)
      { /*If: non-read or filtered read*/
         offsetUL += indexSTPtr->lineAryUI[lineUL];
         continue; /*not read or read already assigned*/
      } /*If: non-read or filtered read*/

      /**************************************************\
      * Fun29 Sec03 Sub02:
      *   - filter out reads (user provided reference)
      \**************************************************/

      if(refSTPtr)
      { /*If: user provided a reference*/
         if(refNumUI != indexSTPtr->refAryUI[lineUL];
         { /*If: differenty reference*/
            offsetUL += indexSTPtr->lineAryUI[lineUL];
            continue;
         } /*If: differenty reference*/

         if(
              indexSTPtr->startAryUI[lineUI]
            > refSTPtr->refEndUI
         ){ /*If: read is after reference*/
            offsetUL += indexSTPtr->lineAryUI[lineUL];
            continue;
         } /*If: read is after reference*/

         if(
              indexSTPtr->endAryUI[lineUI]
            < refSTPtr->refStartUI
         ){ /*if: read is before reference*/
            offsetul += indexstptr->linearyui[lineul];
            continue;
         } /*If: read is before reference*/

         refOverlapF =
           (float)
           min_genMath(
              indexSTPtr->endAryUI[lineUL],
              refSTPtr->refEndUI
           );

         refOverlapF -=
           (float)
           max_genMath(
              indexSTPtr->startAryUI[lineUL],
              refSTPtr->refStartUI
           );

         qryOverlapF = refOverlapF;

         refOverlapF /= (float) refSTptr->alnReadLenUI;

         qryOverlapF /=
            (float)
            (
                 indexSTPtr->endAryUI[lineUL]
               - indexSTPtr->startAryUI[lineUL]
            ); /*find percent overlap on query*/

         if(
               refOverlapF < setSTPtr->percOverlapF
            && qryOverlapF < setSTPtr->percOverlapF
         ){ /*if: to little overlap*/
            offsetul += indexstptr->linearyui[lineul];
            continue;
         } /*if: to little overlap*/
      } /*If: user provided a reference*/

      /**************************************************\
      * Fun29 Sec03 Sub03:
      *   - get reads passing filters
      \**************************************************/

      errSC =
         getRead_edClust(
            samSTPtr,
            0,         /*from current position*/
            offsetUL,
            indexSTPtr,
            buffStr,
            samFILE
         );

      if(errSC)
      { /*If: had error*/
         if(errSC == def_err_samEntry)
            goto err_fun29_sec0x;
         if(errSC == def_err_samEntry)
            goto err_fun29_sec0x;
      } /*If: had error*/

      offsetUL = 0;

      /**************************************************\
      * Fun29 Sec03 Sub04:
      *   - add reads to varaint arrays (var_clustST)
      \**************************************************/

      if(refSTPtr)
      { /*If: user provided reference*/
         addReadRef_var_clustST(
            samSTPtr,
            refSTPtr,
            varHeapAryST,
            setSTPtr
         );
      } /*If: user provided reference*/

      else
      { /*Else: using mapped reference*/
         addRead_var_clustST(
            samSTPtr,
            &varHeapAryST[ indexSTPtr->refAryUI[lineUL] ],
            setSTPtr
         );
      } /*Else: using mapped reference*/
   } /*Loop: find variants*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec04:
   ^   - remove low read depth variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(
      lineUL = 0;
      lineUL < indexSTPtr->numRefUI;
      ++lineUL
   ){ /*Loop: filter out low read depths*/
      rmLowDepth_var_clustST(
         &varHeapAryST[lineUL]
         setSTPtr->set_edClustST->minDepthUI
      );
   } /*Loop: filter out low read depths*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun29 Sec05:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun29_sec0x;

   err_fun29_sec0x:;
   if(varHeapAryST)
      freeHeapAry_var_clustST(
         varHeapAryST,
         numRefUI
      );
   goto ret_fun29_sec0x;

   ret_fun29_sec0x:;
   return varHeapAryST;
} /*mk_var_clustST*/

/*-------------------------------------------------------\
| Fun30: getProf_profClust
|   - finds a profile from a var_clustST struct
| Input:
|   - 
\-------------------------------------------------------*/

hist_clustST *
getProf_profClust(
   sturct var_clustST *varSTPtr, /*variants to use*/
   struct index_edClust *indexSTPtr,/*index for sam file*/
   struct set_clustST *setSTPtr,  /*settings*/
   struct samEntry *samSTPtr,   /*for reading samFILE*/
   signed char *buffStr,        /*for reading samFILE*/
   void *samFILE                /*has reads for profile*/
){
   /*step 1: scan variants for good read depth region*/
   /*step 2: find overlap region*/
   /*step 3: build first profile for region*/
   /*step 4: extract reads and update profile*/
} /*getProf_profClust*/
