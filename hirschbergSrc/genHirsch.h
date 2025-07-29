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
