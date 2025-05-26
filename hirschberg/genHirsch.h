/*########################################################
# Name: generalHirsch.h
# Use:
#  - Holds functions used shared by non-twoBitAry
#    Hirschbergs
# Libraries:
#  - "../general/genMath.h"           (No .c File)
#  - "../general/alnStruct.h"         (No .c File)
#  - "../general/genAln.h"            (No .c File)
#  o "../general/seqStruct.h"         (No .c File)
#  o "../general/alnSetStruct.h"      (No .c File)
#  o "../general/alnSeqDefaults.h"    (No .c File)
#  o "../general/strToNum.h"          (No .c File)
#  o "../general/dataTypeShortHand.h" (No .c File)
# C Standard Libraries:
#  o <time.h>
#  o <stdlib.h>
#  o <stdio.h>
#  o <string.h>
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of Functions
'  o header:
'    - Includes and definitions
'  o fun01 positionSingleRefBase:
'    - Align a single reference base to a query sequence
'  o fun02 twoBitAlnToAlnST:
'    - Converts a two bit array with an alignment to an
'      alnStruct structure
'  o set-01: maxGap (non-vector)
'    - Finds the best score and determines if the score
'      was an gap (del/ins) or snp/match.
'    - Each macro in the set prioritizes insertions,
'      deletions, and snps differently.
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - Includes and definitions
\-------------------------------------------------------*/

#ifndef GENERAL_HIRSCH_H
#define GENERAL_HIRSCH_H

#include "../general/alnStruct.h"
#include "../general/genAln.h"

/*-------------------------------------------------------\
| Fun02: hirschToAlnST
|  - Converts an alignment from HirschbergFun to an
|    alnStruct structure
| Input:
|  - refST:
|    o Pointer to seqStruct with the reference sequence
|      and the first base to align in the reference
|      sequence (refST->offsetUL; as index 0)
|  - qryST:
|    o Pointer to seqStruct with the query sequence and
|      the first base to align in the query sequence
|      (qryST->offsetUL; as index 0)
|  - settings:
|    o Pointer to alnSet structure with the match matrix
|      used to determine if bases were a match or snp
|  - refAlignment:
|    o Has the reference alignment output by HirschbergFun
|    o if -DHIRSCHTWOBIT is two bit array, else char array
|  - qryAlignment:
|    o Has the query alignment output by HirschbergFun
|    o if -DHIRSCHTWOBIT is two bit array, else char array
| Output:
|  - Returns:
|    o 0: for error
|    o pointer to alnStruct with alignment
\-------------------------------------------------------*/
static struct alnStruct * hirschToAlnST(
  struct seqStruct *refST, /*Reference sequence & offset*/
  struct seqStruct *qryST, /*Query sequence & offset*/
  struct alnSet *settings, /*Has matrix to find if match*/
  char *refAlignment,      /*has reference alignment*/
  char *qryAlignment       /*has query alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC: hirschToAlnST
   '  - Converts an alignment from HirschbergFun to an
   '    alnStruct structure
   '  o fun02 sec01:
   '    - Variable declerations
   '  o fun02 sec02:
   '    - Allocate memory for the alnStruct
   '  o fun02 sec03:
   '    - Copy the alignment to the alnStruct
   '  o fun02 sec04:
   '    - Add softmasking to the alnStruct
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   char *refAlnStr = 0;
   char *qryAlnStr = 0;

   char *refBaseStr = 0;
   char *qryBaseStr = 0;

   uchar refBitUC = 0;
   uchar qryBitUC = 0;
   uchar matchBl = 0;

   ulong refIndexUL = 0;
   ulong qryIndexUL = 0;

   long refFirstAlnBaseL = -1;
   long refLastAlnBaseL = 0;
   long qryFirstAlnBaseL = -1;
   long qryLastAlnBaseL = 0;

   long numDelsL = 0;
   long numInssL = 0;
   long numSnpsL = 0;
   long numMatchesL = 0;

   struct alnStruct *alnST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^  - Allocate memory for the alnStruct
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   alnST = malloc(sizeof(alnStruct));
   if(alnST == 0) return 0; /*Memory error*/
   initAlnST(alnST);

   refAlnStr = calloc(refST->lenSeqUL + 1, sizeof(char));

   if(refAlnStr == 0)
   { /*If had a memor error*/
      freeAlnST(alnST); /*1 for freeing heap memory*/
      alnST = 0;
      return 0;
   } /*If had a memor error*/

   alnST->refAlnStr = refAlnStr;

   qryAlnStr = calloc(qryST->lenSeqUL + 1, sizeof(char));

   if(qryAlnStr == 0)
   { /*If: had a memory allocation error*/
     freeAlnST(alnST); /*1 for freeing heap memory*/
     alnST = 0;
     refAlnStr = 0;
     return 0;
   } /*If: had a memory allocation error*/

   alnST->qryAlnStr = qryAlnStr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^  - Copy the alignment to the alnStruct
   ^  o fun02 sec03 sub01:
   ^    - Move to first bases & get the first aligned base
   ^  o fun02 sec03 sub02:
   ^    - Start loop and check if have insertions
   ^  o fun02 sec03 sub03:
   ^    - Check for stop and snps/matches
   ^  o fun02 sec03 sub04:
   ^    - Check if have deletions
   ^  o fun02 sec03 sub05:
   ^    - Record the end and start of the alignmnent
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *  - Move to first bases and get the first aligned base
   \*****************************************************/

   /*Each alignment starts at the offset*/
   refIndexUL = refST->offsetUL;
   qryIndexUL = qryST->offsetUL;

   refAlnStr += refIndexUL;
   qryAlnStr += qryIndexUL;

   refBaseStr = refST->seqCStr + refST->offsetUL;
   qryBaseStr = qryST->seqCStr + qryST->offsetUL;

   refBitUC = (uchar) *refAlignment;
   qryBitUC = (uchar) *qryAlignment;

   /*****************************************************\
   * Fun02 Sec03 Sub02:
   *  - Start loop and check if have insertions
   \*****************************************************/

   while(   refBitUC != defEndAlnFlag
         || qryBitUC != defEndAlnFlag
   ){ /*Loop: Add reference aligned bases to alnStruct*/
      if(qryBitUC == defGapFlag)
      { /*If: I had an insertion*/
         ++numInssL;
         *qryAlnStr = defGapFlag;

         ++qryAlignment;
         qryBitUC = (uchar) *qryAlignment;

         ++qryAlnStr;
         ++qryIndexUL;
         ++qryBaseStr;
         continue;
      } /*If: I had an insertion*/

      /***************************************************\
      * Fun02 Sec03 Sub03:
      *  - Check for stop and snps/matches
      \***************************************************/

      switch(refBitUC)
      { /*Switch: Check the error type*/
         case 0: break;

         case defSnpFlag:
         /*Case: snp or match*/
            matchBl =
             matchOrSnp(*qryBaseStr,*refBaseStr,settings);

            if(matchBl)
            { /*If: I had a match*/
               ++numMatchesL;
               *refAlnStr = defMatchFlag; 
               *qryAlnStr = defMatchFlag; 
            } /*If: I had a match*/

            else
            { /*Else: I had an snp (mismatch)*/
               ++numSnpsL;
               *refAlnStr = defSnpFlag; 
               *qryAlnStr = defSnpFlag; 
            } /*Else: I had an snp (mismatch)*/

            if(refFirstAlnBaseL < 0)/*Start of alignment*/
               refFirstAlnBaseL = refIndexUL;

            if(qryFirstAlnBaseL < 0)/*Start of alignment*/
               qryFirstAlnBaseL = qryIndexUL;

            refLastAlnBaseL=refIndexUL;/*end alignment*/
            qryLastAlnBaseL=qryIndexUL;/*end alignment*/

            ++refAlignment;
            ++qryAlignment;

            refBitUC = (uchar) *refAlignment;
            qryBitUC = (uchar) *qryAlignment;

            ++refAlnStr;
            ++qryAlnStr;

            ++refIndexUL;
            ++qryIndexUL;

            ++refBaseStr;
            ++qryBaseStr;

            break;
         /*Case: snp or match*/

         /************************************************\
         * Fun02 Sec03 Sub04:
         *  - Check for deletions
         \************************************************/

         case defGapFlag:
         /*Case: Deletions*/
            ++numDelsL;
            *refAlnStr = defGapFlag;
   
            ++refAlignment;
            refBitUC = (uchar) *refAlignment;
   
            ++refAlnStr;
            ++refIndexUL;
            ++refBaseStr;
            break;
         /*Case: Deletions*/
      } /*Switch: Check the error type*/
   } /*Loop: Add reference aligned bases to alnStruct*/

   /*****************************************************\
   * Fun02 Sec03 Sub05:
   *  - Record the end and start of the alignmnent
   \*****************************************************/

   /*Add in an end of alignment flag*/
   *refAlnStr = defEndAlnFlag;
   *qryAlnStr = defEndAlnFlag;

   /*Record the start of the alignment*/
   if(refFirstAlnBaseL >= 0)
      alnST->refStartAlnUL = refFirstAlnBaseL;
   else alnST->refStartAlnUL = refST->lenSeqUL;

   if(qryFirstAlnBaseL >= 0)
      alnST->qryStartAlnUL = qryFirstAlnBaseL;
   else alnST->qryStartAlnUL = qryST->lenSeqUL;


   /*Record the end of the alignment*/
   if(refLastAlnBaseL > 0)
      alnST->refEndAlnUL = refLastAlnBaseL;
   else alnST->refEndAlnUL = refST->lenSeqUL;

   if(qryLastAlnBaseL > 0)
      alnST->qryEndAlnUL = qryLastAlnBaseL;
   else alnST->qryEndAlnUL = qryST->lenSeqUL;

   /******************************************************\
   * Fun02 Sec03 Sub01:
   *  - Add in the alignment stats
   \******************************************************/

   alnST->numInssUL = (ulong) numInssL;
   alnST->numDelsUL = (ulong) numDelsL;
   alnST->numSnpsUL = (ulong) numSnpsL;
   alnST->numMatchesUL = (ulong) numMatchesL;

   alnST->lenAlnUL =
      (ulong) (numInssL+numDelsL+numSnpsL+numMatchesL);

   alnST->refLenUL = refST->lenSeqUL;
   alnST->qryLenUL = qryST->lenSeqUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^  - Add softmasking to the alnStruct
   ^  o fun02 sec03 sub01:
   ^    - Add soft masking to the unaligned ending bases
   ^  o fun02 sec03 sub02:
   ^    - Add soft masking to the unaligned starting bases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /******************************************************\
   * Fun02 Sec03 Sub01:
   *  - Add soft masking to the unaligned ending bases
   \******************************************************/

    while(refIndexUL < refST->lenSeqUL)
    { /*Loop: Apply mask to starting reference bases*/
       *refAlnStr = defSoftMaskFlag;
       ++refAlnStr;
       ++refIndexUL;
    } /*Loop: Apply mask to starting reference bases*/

    while(qryIndexUL < qryST->lenSeqUL)
    { /*Loop: Apply mask to starting query bases*/
       *qryAlnStr = defSoftMaskFlag;
       ++qryAlnStr;
       ++qryIndexUL;
    } /*Loop: Apply mask to starting query bases*/

   /******************************************************\
   * Fun02 Sec03 Sub02:
   *  - Add soft masking to the unaligned starting bases
   \******************************************************/

    refAlnStr = alnST->refAlnStr; 
    while(*refAlnStr == 0)
    { /*Loop: Apply mask to starting reference bases*/
       *refAlnStr = defSoftMaskFlag;
       ++refAlnStr;
    } /*Loop: Apply mask to starting reference bases*/

    qryAlnStr = alnST->qryAlnStr; 
    while(*qryAlnStr == 0)
    { /*Loop: Apply mask to starting query bases*/
       *qryAlnStr = defSoftMaskFlag;
       ++qryAlnStr;
    } /*Loop: Apply mask to starting query bases*/

   return alnST;
} /*hirschToAlnST*/

/*-------------------------------------------------------\
| Set-01: maxGap (non-vector)
|  - Finds the best score and determines if the score was
|    an gap (del/ins) or snp/match.
|  - Each macro in the set prioritizes insertions,
|    deletions, and snps differently.
| Input:
|  - retMax;
|    o Holds the maximum value
|  - isGap:
|    o Holds if kept score was an gap (-1) or an snp (0)
|  - snp:
|    o Score for an snp
|  - ins:
|    o Score for an insertion
|  - del:
|    o Score for a deletion
| Output:
|  - Modifies:
|    o isGap to hold if best score was an gap (-1) or snp
|    o retMax to hold the best scores
| Sets:
|  o set-01a: maxGapInsDelSnp
|  o set-01b: maxGapDelInsSnp
|  o set-01c: maxGapInsSnpDel
|  o set-01d: maxGapDelSnpIns
|  o set-01e: maxGapSnpInsDel
|  o set-01f: maxGapSnpDelIns
\-------------------------------------------------------*/

/*-------------------------------------------------------\
| Set-01a: maxGapInsDelSnp
\-------------------------------------------------------*/
#define maxGapInsDelSnp(retMax,isGap,snp,ins,del)\
{ /*maxGapInsDelSnp*/ \
   macroMax((isGap), (ins), (del));         /*5 op*/\
   (isGap) = -( (retMax) >= (snp) );        /*2 op*/\
   macroMax((retMax), (isGap), (snp));      /*5 op*/\
      /*Logic:
      ` retMax < snp
      `    1 if selected an insertion/deletion, else 0
      ` -(isGap == del)
      `    Changes 1 to -1 (111...1) (ins/del), else 0
      */\
} /*maxGapInsDelSnp*/

/*-------------------------------------------------------\
| Set-01b: maxGapDelInsSnp
\-------------------------------------------------------*/
#define maxGapDelInsSnp(retMax,isGap,snp,ins,del)\
{ /*maxGapDelInsSnp*/\
   macroMax((retMax), (del), (ins));    /*5 op*/\
   (isGap) = -( (retMax) >= (snp) );    /*2 op*/\
   macroMax((retMax), (retMax), (snp)); /*5 op*/\
      /*Logic:
      ` isGap == retMax
      `    1 if selected an insertion/deletion, else 0
      ` -(isGap == retMax)
      `    Changes 1 to -1 (111...1) (ins/del), else 0
      */\
} /*maxGapDelInsSnp*/

/*-------------------------------------------------------\
| Set-01c: maxGapInsSnpDel
\-------------------------------------------------------*/
#define maxGapInsSnpDel(retMax,isGap,snp,ins,del)\
{ /*maxGapInsSnpDel*/\
   macroMax((retMax), (snp), (del))          /*5 op*/\
   (isGap) = ((snp) < (retMax));             /*1 op*/\
   \
   macroMax((retMax), (ins), (retMax));      /*5 op*/\
   (isGap) = -((ins == (retMax)) | (isGap)); /*3 op*/\
      /*Logic
      ` isGap == 1
      `   is if a deletion is prefered (del > snp), else 0
      ` ins == del
      `   is 1 if a insertion is prefered, else 0
      ` (ins == del) | isGap
      `   is 1 if an insertion or deletion was prefered
      ` -((in == del) | isGap)
      `   is -1 (111...1) if an insertion or deletion
      `   was preffered, else 0
      */\
} /*maxGapInsSnpDel*/

/*-------------------------------------------------------\
| Set-01d: maxGapDelSnpIns
\-------------------------------------------------------*/
#define maxGapDelSnpIns(retMax,isGap,snp,ins,del)\
{ /*maxGapDelSnpIns*/\
   macroMax((retMax), (snp), (ins));         /*5 op*/\
   (isGap) = ((snp) < (retMax));             /*1 op*/\
   macroMax((retMax), (del), (retMax));      /*5 op*/\
   (isGap) = -((del == (retMax)) | (isGap)); /*3 op*/\
      /*Logic
      ` isGap
      `   1 if a deletion is prefered (snp <= del), else 0
      ` ins == del
      `   1 if a insertion is prefered, else 0
      ` (ins == del) | isGap
      `   is 1 if an insertion or deletion was prefered
      ` -((in == del) | isGap)
      `   is -1 (111...1) if an insertion or deletion
      `   was preffered, else 0
      */\
} /*maxGapDelSnpIns*/

/*-------------------------------------------------------\
| Set-01e: maxGapSnpInsDel
\-------------------------------------------------------*/
#define maxGapSnpInsDel(retMax,isGap,snp,ins,del)\
{\
   macroMax((retMax), (snp), (ins));   /*5 op*/\
   macroMax((retMax), (retMax), (del));/*5 op*/\
   (isGap) = -((snp) < (retMax));      /*2 op*/\
      /*Logic:
      ` snp < del
      `    is 1 if deletion selected, else 0
      ` -(snp < del)
      `    is -1 (111...1) if deletion/insertion, else 0
      */\
} /*maxGapSnpInsDel*/

/*--------------------------------------------------------\
| Set-01f: maxGapSnpDelIns
\--------------------------------------------------------*/
#define maxGapSnpDelIns(retMax,isGap,snp,ins,del)\
{\
   macroMax((retMax), (snp), (del));    /*5 op*/\
   macroMax((retMax), (retMax), (ins)); /*5 op*/\
   (isGap) = -((snp) < (retMax));       /*2 op*/\
      /*Logic:
      ` snp < del
      `    is 1 if deletion selected, else 0
      ` -(snp < del)
      `    is -1 (111...1) if deletion/insertion, else 0
      */\
} /*maxGapSnpDelIns*/

/*-------------------------------------------------------\
| Fun01: maxGapScore
| Use:
|  - Chooses which maxGapX function to call to maximize and
|    get the gaps.
| Input:
|  - retMax:
|    o Will hold the maximum score
|  - isGap:
|    o Holds if kept score was an gap (-1) or an snp (0)
|  - snp:
|    o Is the score for the snp
|  - ins:
|    o  for the insertion
|  - del:
|    o  for an deltion. Will hold the max score
|  - preflag:
|    o Flag having preferance for direction
|    o options: defInsDelSnp, defDelInsSnp, defInsSnpDel,
|               defDelSnpIns, defSnpInsDel, defSnpDelIns
| Output:
|  - Modifies:
|    o retMax to hold the maximum value
|    o isGap to if was a gap (-1) or snp/match (0)
| Variations:
|  - -DINSDELSNP:
|    o prefer insertions, then deletions, then snps
|  - -DDELINSSNP:
|    o prefer deletions, then insertions, then snps
|  - -DINSSNPDEL:
|    o prefer insertions, then snps, then deletions
|  - -DDELSNPINS:
|    o prefer deletions, then snps, then insertions
|  - -DSNPINSDEL:
|    o prefer snps, then insertions, then deletions
|  - -DSNPDELINS:
|    o prefer snps, then deletions, then insertions
|  - default:
|    o User picks direction they prefer
\-------------------------------------------------------*/

#ifdef INSDELSNP
   #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag)\
     {maxGapInsDelSnp((retMax),(isGap),(snp),(ins),(del))}

#elif defined DELINSSNP
   #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag)\
     {maxGapDelInsSnp((retMax),(isGap),(snp),(ins),(del))}

#elif defined INSSNPDEL
   #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag)\
     {maxGapInsSnpDel((retMax),(isGap),(snp),(ins),(del))}

#elif defined DELSNPINS
   #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag)\
     {maxGapDelSnpIns((retMax),(isGap),(snp),(ins),(del))}

#elif defined SNPINSDEL
   #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag)\
     {maxGapSnpInsDel((retMax),(isGap),(snp),(ins),(del))}

#elif defined SNPDELINS
   #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag)\
     {maxGapSnpdelIns((retMax),(isGap),(snp),(ins),(del))}

#else
 #define maxGapScore(retMax,isGap,snp,ins,del,prefFlag){\
  switch((prefFlag))\
  { /*Switch; get an snp/match priority*/\
   case defSnpInsDel:\
     maxGapSnpInsDel((retMax),(isGap),(snp),(ins),(del));\
     break;\
   case defSnpDelIns:\
     maxGapSnpDelIns((retMax),(isGap),(snp),(ins),(del));\
     break;\
   case defInsSnpDel:\
     maxGapInsSnpDel((retMax),(isGap),(snp),(ins),(del));\
     break;\
   case defDelSnpIns:\
     maxGapDelSnpIns((retMax),(isGap),(snp),(ins),(del));\
     break;\
   case defInsDelSnp:\
     maxGapInsDelSnp((retMax),(isGap),(snp),(ins),(del));\
     break;\
   case defDelInsSnp:\
     maxGapDelInsSnp((retMax),(isGap),(snp),(ins),(del));\
     break;\
  } /*Switch; get an snp/match priority*/\
 } /*maxGapScore; default variation*/
#endif /*Check if harcoding a preferared direction*/

#endif
