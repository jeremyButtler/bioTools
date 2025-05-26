/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' hirschberg SOF: Start Of File
'  o header:
'    - included libraries
'  o .c fun01: alnOneNt_hirschberg
'    - align a single base to a sequence
'  o .c Fun02: recursive_hirschberg
'    - does the recurisve part of a hirschberg alignment
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "../genBio/samEntry.c"
#include "hirschberg.h"
#include "alnSet.h"

/*.h files only*/
#include "../genLib/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdio.h>
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/endLine.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*these are for the alignment arrays, not the final
`   output
*/
#define def_gap_hirschberg 1
#define def_base_hirschberg 2

/*-------------------------------------------------------\
| Fun01: alnOneNt_hirschberg
|  - align a single base to a sequence
| Input:
|  - baseSC:
|    o Base to align to a sequence
|  - baseIndexSL:
|    o Position of base in its seqequence (index 0)
|  - seqStr:
|    o Sequence to align the base to
|  - startOfSeqSL:
|    o Fist base to align in seqStr (index 0)
|  - lenSeqSL:
|    o Number of bases to align in seqStr (index 1)
|  - baseAlnStr:
|    o Holds the alignment of baseC's sequence
|  - seqAlnStr:
|    o Holds the alignment of seqStr
|  - settings:
|    o Pointer to alnSet structure with the settings for
|      the alignment
| Output:
|  - Modifies:
|    o baseAlnStr to hold the alignment for the single
|      base (baseC)
|    o seqAlnStr to hold the alignment for the sequence
|      (seqStr)
\-------------------------------------------------------*/
void
alnOneNt_hirschberg(
   signed char baseSC,      /*single base to align*/
   signed long baseIndexSL, /*index baseSC is at*/
   signed char *seqStr,     /*seq to align baseSC to*/
   signed long startOfSeqUL,/*1st base to align; seqStr*/
   signed long lenSeqUL,    /*number bases in seqStr*/
   signed char *baseAlnStr, /*holds baseC alignment*/
   signed char *seqAlnStr,  /*holds seqStr alignment*/
   struct alnSet *settings  /*setttings to use*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '  - Align a single base to a sequence
   '  o fun01 sec01:
   '    - variable declerations
   '  o fun01 sec02:
   '    - find the reference bases position on the query
   '  o fun01 sec03:
   '    - fill in insertions and reference base position
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^  - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long insScoreSL = 0;   /*score for a insertion*/
   signed long matchScoreSL = 0; /*score for a match*/
   signed long lastInsSL = 0;    /*score for next snp*/

   signed long slBase = 0;       /*for the for loop*/
   signed long snpIndexSL = 0;   /*index of last match*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^  - find the reference bases position on the query
   ^  o fun01 sec02 sub01:
   ^    - find the first scores for the loop
   ^  o fun01 sec02 sub02:
   ^    - find the remaing scores
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec02 Sub01:
   *  - find the first scores for the loop
   \*****************************************************/

   lastInsSL = 0;
   insScoreSL = settings->gapSS;

   /*****************************************************\
   * Fun01 Sec02 Sub02:
   *  - Find the remaing scores
   \*****************************************************/
 
   for(
      slBase = startOfSeqSL;
      slBase < startOfSeqSL + lenSeqSL;
      ++ulBase
   ){ /*Loop: align baseC to seqStr*/
     matchScoreSL =
         lastInsSL
       + getBaseScore(seqStr[slBase], baseSC, settings);

     #ifdef NOEXTEND
        insScoreSL += settings->gapSS;
     #else
        insScoreSL += settings->extendSS;
     #endif

     lastInsSL = insScoreSL;

     if(matchScoreSL > insScoreSL)
        snpIndexSL = slBase;
   } /*Loop: align baseC to seqStr*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^  - Fill in the insertions and reference base position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /* A series of deletions and insertions are prefered
   `  over matches and smps. In this case put the base
   `  at the first base. There is no good position
   */
   if(snpIndexSL == 0)
      snpIndexSL = startOfSeqSL;

   /*add in the insertions at the start*/
   for(
      slBase = startOfSeqSL;
      slBase < snpIndexSL;
      ++slBase
   ) seqAlnStr[slBase] = def_gap_hirschberg;
   
   /*Add in the position of the base*/
   baseAlnStr[baseIndexSL] = def_base_hirschberg;
   seqAlnStr[slBase] = def_base_hirschberg;

   /*Finish adding in the insertions at the end*/
   for(
      slBase = slBase + 1;
      slBase < startOfSeqSL + lenSeqSL;
      ++slBase
   ) seqAlnStr[slBase] = def_gap_hirschberg;

   return;
} /*alnOneNt_hirschberg*/


/*-------------------------------------------------------\
| Fun02: recursive_hirschberg
|  - does the recurisve part of a hirschberg alignment
| Input
|  - refSeqStr:
|    o reference sequence to align
|  - refStartUL:
|    o First base to align in the reference (index 0)
|  - refLenUL:
|    o Number of bases to align in the reference (index 1)
|  - qrySeqStr:
|    o query sequence to align
|  - qryStartUL:
|    o First base to align in the query (index 0)
|  - qryLenUL:
|    o Number of bases to align in the query (index 1)
|  - forScoreRowL:
|    o Row holding the forward (1st half of query) scores
|    o Must be the size of the full length reference
|  - revScoreRowL:
|    o Row holding the reverse (last half of query) scores
|    o Must be the size of the full length reference
|  - refAlnST:
|    o Holds the reference alignment and is a temporary
|      row for finding directions
|    o if -DHIRSCHTWOBIT is two bit array, else char array
|  - qryAlnST:
|    o Holds the query alignment
|  - settings:
|    o Pointer to alnSet structure with the settings for
|      the alignment
| Output:
|  - Modifies:
|    o refAlnST to hold the reference alignment
|    o qryAlnST to hold the query alignment
\-------------------------------------------------------*/
void
recursive_hirschberg(
  signed char *refSeqStr,/*reference sequence*/
  signed long refStartSL,/*1st reference base to align*/
  signed long refLenSL,  /*number of ref bases to align*/

  signed char *qrySeqStr,/*query sequence*/
  signed long qryStartSL,/*1st query base to align*/
  signed long qryLenSL,  /*number of query bases to align*/

  signed long *forScoreRowSL, /*holds final forward row*/
  signed long *revScoreRowSL, /*finding reverse scores*/
  
  signed char *refAlnStr, /*output reference alignment*/
  signed char *qryAlnStr, /*output query alignment*/

  struct alnSet *settings /*Settings for alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '  - does the recursive part of a Hirschberg alignment
   '  o fun02 sec01:
   '    - Variable declerations
   '  o fun02 sec02:
   '    - Check if on a leaf (final part of alignment
   '  o fun02 sec03:
   '    - Get scores
   '  o fun02 sec04:
   '    - Find the midpoint
   '  o fun02 sec05:
   '    - Run the next hirschberg alignment
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun02 Sec01:
   ^    - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long forGapColSL = 0;
   signed long revGapColSL = 0;
   signed long midPointSL = 0;
   signed long slFor = 0;/*Loop iterator (forward score)*/

   signed long slGapOn = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^  - Check if on a leaf (final part of alignment
   ^  o fun02 sec02 sub01:
   ^    - Handle cases were I have just insertions
   ^  o fun02 sec02 sub02:
   ^    - Handle cases were I have just deletions
   ^  o fun02 sec02 sub03:
   ^    - Handle cases were I have to align last ref base
   ^  o fun02 sec02 sub04:
   ^  - Handle cases were I have to align last query base
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *  - Handle cases were I have just insertions
   \*****************************************************/

   if(refLenSL == 0)
   { /*If all remaing bases are query insertions*/
    for(
       slGapOn = qryStartSL;
       slGapOn < qryLenSL + qryStartSL;
       ++slGapOn
     ) qryAlnStr[slGapOn] = def_gap_hirschberg;

     return; /*Nothing else to do*/
   } /*If all remaing bases are query insertions*/

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *  - Handle cases were I have just deletions
   \*****************************************************/

   if(qryLenSL == 0)
   { /*If all remaing bases are query deletions*/
    for(
       slGapOn = refStartSL;
       slGapOn < refLenSL + refStartSL;
       ++slGapOn
    ) refAlnStr[slGapOn] = def_gap_hirschberg;

     return; /*Nothing else to do*/
   } /*If all remaing bases are query deletions*/

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *  - Handle cases were I have to align last ref base
   \*****************************************************/

   if(refLenSL == 1)
   { /*If I have to align the last reference base*/
     if(qryLenSL == 0)
     { /*If bases are aligned (one ref & one query)*/
        refAlnStr[refStartSL] = def_gap_hirschberg;
        return; 
     } /*If bases are aligned (one ref & one query)*/

     if(qryLenSL == 1)
     { /*If bases are aligned (one ref & one query)*/
        qryAlnST[qryStartSL] = def_base_hirschberg;
        refAlnST[refStartSL] = def_base_hirschberg;
        return;
     } /*If bases are aligned (one ref & one query)*/

     alnOneNt_hirschberg(
       *(refSeqStr + refStartUL),/*ref base*/
       refStartUL,             /*Position of ref base*/
       qrySeqStr,              /*first base of query*/
       qryStartUL,             /*positoin of query*/
       qryLenUL,               /*Length of the query*/
       refAlnST,               /*Array to hold alignment*/
       qryAlnST,               /*Array to hold alignment*/
       settings                /*Has Scoring variables*/
     );

     return; /*This base is now aligned*/
   } /*If I have to align the last reference base*/

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *  - Handle cases were I have to align last query base
   \*****************************************************/

   if(qryLenSL == 1)
   { /*If I have to align the last query base*/
     if(refLenSL == 0)
     { /*If bases are aligned (one ref & one query)*/
        qryAlnStr[qryStartUL] = def_gap_hirschberg;
        return; /*Finished*/
     } /*If bases are aligned (one ref & one query)*/

     if(refLenSL == 1)
     { /*If bases are aligned (one ref & one query)*/
        qryAlnStr[qryStartSL] = def_base_hirschberg;
        refAlnStr[refStartSL] = def_base_hirschberg;
        return; /*Finished*/
     } /*If bases are aligned (one ref & one query)*/

     alnOneNt_hirschberg(
       *(qrySeqStr + qryStartUL),/*ref base*/
       qryStartUL,             /*Position of ref base*/
       refSeqStr,              /*first base of reference*/
       refStartUL,             /*positoin of query*/
       refLenUL,               /*Length of the query*/
       qryAlnST,               /*Array to hold alignment*/
       refAlnST,               /*Array to hold alignment*/
       settings                /*Has Scoring variables*/
     );

     return; /*Finshed aligning this query base*/
   } /*If I have to align the last query base*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^  - Get scores
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    forGapColSL = 
      scoreHirschFor(
        refSeqStr,       /*Entire reference sequence*/
        refStartSL,      /*Starting base of ref target*/
        refLenSL,        /*length of ref target region*/
        qrySeqStr,       /*Query seq with coordinates*/
        qryStartSL,      /*Starting base of query target*/
        qryLenSL / 2,    /*Length of query target region*/
        forScoreRowSL,   /*Array of scores to fill*/
        refAlnStr,       /*direction row for gap extend*/
        settings         /*setttings to use*/
    ); /*Get the scores for the forward direction*/
    /*For -DNOEXTEND, refAlnST is ignored*/
    /*if I added another array for the directions, I
    `   could multithread this section, but I typically
    `   do not multithread now days
    */

    revGapColSL = 
      scoreHirschRev(
        refSeqStr,     /*Entire reference sequence*/
        refStartSL,    /*Starting base of ref target*/
        refLenSL,      /*length of ref target region*/
        qrySeqStr,     /*Query seq with coordinates*/
        qryStartSL + (qryLenUL / 2),/*new query start*/
        qryLenSL - (qryLenUL / 2),  /*New query length*/
        revScoreRowSL,  /*Array of scores to fill*/
        refAlnStr,       /*direction row for gap extend*/
        settings       /* setttings to use*/
      ); /*Get the scores for the reverse direction*/
      /*For -DNOGAPOPEN, dirRow is ignored*/
      /* I can get away with queryLen/2 here, because 
      `  queryLen is index 1 and the function takes in
      `  an lenth 1 argument
      `  I made this section thread safe by using refAlnST
      `    for the forward score and dirRow for the
      `    reverse row. So, you could run the forward and
      `    reverse scoring step in parrallel
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun02 Sec04:
   ^   - Find the midpoint
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   forScoreRowSL[refStartSL + refLenSL -1] += revGapColSL;
   midPointSL = refStartSL + refLenSL - 1;

   for(
      slFor = refStartSL;
      slFor < refStartSL + refLenSL - 1;
      ++slFor
   ){ /*Loop; add up all scores*/
     forScoreRowSL[slFor] += revScoreRowSL[slFor + 1];
       /*The reverse row is already reversed*/

     if(forScoreRowSL[slFor] > forScoreRowSL[midPointSL])
        midPointSL = slFor;
   } /*Loop; add up all scores*/

   forGapColSL += revScoreRowSL[refStartSL];

   if(forGapColSL > forScoreRowL[midPointSL])
      midPointSL = 0;
   else
      midPointSL = midPointSL + 1 - refStartSL;


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun02 Sec05:
   ^    - Run the Hirschberg alignment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   recursive_hirschberg(
     refSeqStr,      /*Full reference sequence*/
     refStartSL,     /*Full reference sequence*/
     midPointSL,     /*Length of new reference sequence*/
     qrySeqStr,      /*Full query sequence*/
     qryStartSL,     /*Start of queyr target region*/
     qryLenSL / 2,   /*Length of query target region*/
     forScoreRowSL,  /*For scoring*/
     revScoreRowSL,  /*Has last line of scores*/
     refAlnStr,      /*direction row for gap extend*/
     qryAlnStr,      /*Holds the alignment codes*/
     settings        /*Settings for the alignment*/
   );

   recursive_hirschberg(
     refSeqStr,             /*Full reference sequence*/
     refStartSL + midPointSL,/*New reference start*/
     refLenSL - midPointSL, /*New reference end*/
     qrySeqStr,             /*Full query sequence*/
     qryStartSL + (qryLenSL / 2),/*New query start*/
     qryLenSL - (qryLenSL / 2),  /*New query length*/
     forScoreRowSL,         /*For scoring*/
     revScoreRowSL,         /*Has last line of scores*/
     refAlnStr,             /*Holds reference alingment*/
     qryAlnStr,             /*Holds query alingment*/
     settings               /*Settings for alignment*/
   );

   return;
} /*recursive_hirschberg*/

/*-------------------------------------------------------\
| Fun03: sam_hirschberg
|  - does hischberg alignment
| Input:
|  - refST:
|    o Pointer to seqStruct structure with the reference
|      sequence and the first base (offsetUL) and last
|      base (endAlnUL) to align (both are index 0)
|  - qryST:
|    o Pointer to seqStruct structure with the query
|      sequence and the first base (offsetUL) and last
|      base (endAlnUL) to align (both are index 0)
|  - samSTPtr:
|    o pointer to samEntry struct to store alignment in
|  - settings:
|    o Pointer to alnSet structure with the settings for
|      the alignment
| Output:
|  - Returns:
|    o a samEntry structure with the alignment
|    o 0 For memory errors
\-------------------------------------------------------*/
signed char
sam_hirschberg(
  struct seqStruct *refST, /*reference sequence to align*/
  struct seqStruct *qryST, /*qeury sequence to align*/
  struct samEntry *samSTPtr, /*gets alignment*/
  struct alnSet *settings  /*settings for alignment*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC: sam_hirschberg
   '  - hischberg alignment and returns samEntry struct
   '  o fun03 sec01:
   '    - variable declerations
   '  o fun03 sec02:
   '    - memory allocation (set up for Hirschberg)
   '  o fun03 sec03:
   '    - run the hirschberg alignment
   '  o fun03 sec04:
   '    - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun03 Sec01:
   ^    - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   ulong lenRefUL = refST->endAlnUL - refST->offsetUL + 1;
   ulong lenQryUL = qryST->endAlnUL - qryST->offsetUL + 1;
     /*+ 1 to convert to index 1 (values are index 0)*/

   long *forwardScoreRowL = 0;
   long *reverseScoreRowL = 0;

   struct alnStruct *alnST = 0;

   char *refAln = 0;
   char *qryAln = 0;
   char *dirRow = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun03 Sec02:
   ^   - Memory allocation (set up for Hirschberg)
   ^   o fun03 sec02 sub01:
   ^     - Initalize the ouput alignment structure 
   ^   o fun03 sec02 sub02:
   ^     - Initalize the scoring rows
   ^   o fun03 sec02 sub03:
   ^     - Initalize the direction rows
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *  - Initalize the ouput alignment structure 
   \*****************************************************/

   refAln = calloc(lenRefUL + 1, sizeof(char));
   if(refAln == 0) return 0;
   *(refAln + lenRefUL) = defEndAlnFlag;

   dirRow = calloc(lenRefUL + 1, sizeof(char));

   if(dirRow == 0)
   { /*If I could not make another direction row*/
      free(refAln);
      refAln = 0;
      return 0;
   } /*If I could not make another direction row*/

   qryAln = calloc(lenQryUL + 1, sizeof(char));

   if(qryAln == 0)
   { /*If I could not make another direction row*/
      free(refAln);
      refAln = 0;

      free(dirRow);
      dirRow = 0;

      return 0;
   } /*If I could not make another direction row*/

   /*****************************************************\
   * Fun03 Sec02 Sub02:
   *  - Initalize the scoring rows
   \*****************************************************/

   forwardScoreRowL = malloc(sizeof(long) * lenRefUL);

   if(forwardScoreRowL == 0)
   { /*If had a memory allocatoin error*/
     free(refAln);
     refAln = 0;

     free(qryAln);
     qryAln = 0;

     free(dirRow);
     dirRow = 0;

     return 0;
   } /*If had a memory allocatoin error*/

   reverseScoreRowL = malloc(sizeof(long) * lenRefUL);

   if(reverseScoreRowL == 0)
   { /*If had a memory allocatoin error*/
     free(refAln);
     refAln = 0;

     free(qryAln);
     qryAln = 0;

     free(dirRow);
     dirRow = 0;

     free(forwardScoreRowL);
     return 0;
   } /*If had a memory allocatoin error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun03 Sec03:
   ^    - Run the Hirschberg alignment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Sending in offset values, because alignment array is
   ` sized to the alignmnet region
   */
   HirschbergFun(
     refST->seqCStr + refST->offsetUL,
     0,                /*1st reference base to align*/
     lenRefUL,         /*Length of ref region to align*/
     qryST->seqCStr + qryST->offsetUL,
     0,                /*1st query base to align*/
     lenQryUL,         /*length of query target region*/
     forwardScoreRowL, /*For scoring*/
     reverseScoreRowL, /*For scoring*/
     refAln,     /*Holds the reference alignment*/
     qryAln,     /*Holds the query alignment*/
     dirRow,     /*Direction row for thread safe scoring*/
     settings    /*Settings for the alignment*/
   );
     /*dirRow becomes a dummy variable for -DNOGAPOPEN*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^  Fun03 Sec04:
   ^    - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   free(forwardScoreRowL);
   free(reverseScoreRowL);
 
   free(dirRow);
   dirRow = 0;


   alnST =
     hirschToAlnST(refST,qryST,settings,refAln,qryAln);

   free(refAln);
   refAln = 0;

   free(qryAln);
   qryAln = 0;

   return alnST; /*Is 0 if twoBitAlnToAlnSt failed*/
} /*Hirschberg*/

#endif
