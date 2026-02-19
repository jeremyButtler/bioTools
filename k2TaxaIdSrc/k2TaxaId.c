/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' k2TaxaId SOF: Start Of File
'   - has functions and structs to get read ids by kraken
'     organisms
'   o header:
'     - included libraries
'   o .h st01: taxa_k2TaxaId
'     - has taxa names and ids to extract by
'   o fun01: blank_taxa_k2TaxaId
'     - blanks a taxa_k2TaxaId structure
'   o fun02: init_taxa_k2TaxaId
'     - initializes a taxa_k2TaxaId structure
'   o fun04: freeHeap_taxa_k2TaxaId
'     - frees a taxa_k2TaxaId structure
'   o fun05: setup_taxa_k2TaxaId
'     - allocates memory for a taxa_k2TaxaId struc
'   o fun06: realloc_taxa_k2TaxaId
'     - reallocates memory for a taxa_k2TaxaId struc
'   o fun07: sortCodes_taxa_k2TaxaId
'     - sorts the sortCodeArySL array while keeping the
'       indexArySL in sync a taxa_k2TaxaId
'   o fun08: findCode_taxa_k2TaxaId
'     - returns index of taxa code, searches sorted list
'       and returns unsorted list index
'   o fun09: getLevel_k2TaxaId
'     - get the level in tree of a kraken entry; col 4
'   o fun10: readReport_k2TaxaId
'     - gets list of organism codes
'   o .c fun11: mkTaxaIdFile_k2TaxaIds
'     - prints a single taxa id (here to avoid clutter)
'   o fun12: pIds_k2TaxaId
'     - prints out read ids by taxa for kraken2
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

#include "k2TaxaId.h"

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genLib/genMath.h"
#include "../genLib/shellSort.h"
#include "../genLib/ptrAry.h"
#include "../genLib/fileFun.h"

/*no .c files*/
#include "../genLib/endLine.h"

/*
` Kraken works by character for level in tree, and
`   a number if a sublevel is present. The first
`   level is no number, second is 1, third is 2 ...
`   and the last is the nth level
` U: unclassified (top) (no lower levels)
` R: root (top)
`   R1: cellular entries (all entries)
`     D: domain
`       P: phylum
`         C: class
`           O: order
`             F: family
`               G: genus
`                 S: species
`   R1: other entries
`     R2: other sequences
*/

/*-------------------------------------------------------\
| Fun01: blank_taxa_k2TaxaId
|   - blanks a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId struct to blank
| Output:
|   - Modifies:
|     o numTaxaSL in taxaSTPtr to be 0
\-------------------------------------------------------*/
void
blank_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
){
   taxaSTPtr->numTaxaSL = 0;

   if(taxaSTPtr->idAryST)
      blank_str_ptrAry(taxaSTPtr->idAryST);
} /*blank_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun02: init_taxa_k2TaxaId
|   - initializes a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to initiazlize
| Output:
|   - Modifies:
|     o sets everything in taxaSTPtr to 0
\-------------------------------------------------------*/
void
init_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
){
   taxaSTPtr->codeArySL = 0;
   taxaSTPtr->idAryST = 0;
   taxaSTPtr->backTrackArySL = 0;
   taxaSTPtr->mergeAryBl = 0;
   taxaSTPtr->indexArySL = 0;
   taxaSTPtr->levelArySS = 0;
   taxaSTPtr->sizeTaxaSL = 0;

   blank_taxa_k2TaxaId(taxaSTPtr);
} /*init_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun03: freeStack_taxa_k2TaxaId
|   - frees variables in a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId with vars to free
| Output:
|   - Frees:
|     o all variables in taxaSTPtr (sets vars to 0)
\-------------------------------------------------------*/
void
freeStack_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
){
   if(! taxaSTPtr)
      return;

   if(taxaSTPtr->codeArySL)
     free(taxaSTPtr->codeArySL);

   if(taxaSTPtr->backTrackArySL)
     free(taxaSTPtr->backTrackArySL);

   if(taxaSTPtr->mergeAryBl)
     free(taxaSTPtr->mergeAryBl);

   if(taxaSTPtr->levelArySS)
     free(taxaSTPtr->levelArySS);

   if(taxaSTPtr->indexArySL)
     free(taxaSTPtr->indexArySL);

   if(taxaSTPtr->idAryST)
     freeHeap_str_ptrAry(taxaSTPtr->idAryST);

   init_taxa_k2TaxaId(taxaSTPtr);
} /*freeStack_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun04: freeHeap_taxa_k2TaxaId
|   - frees a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to free
| Output:
|   - Frees:
|     o taxaSTPtr (you must set to null (0))
\-------------------------------------------------------*/
void
freeHeap_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
){
   if(! taxaSTPtr)
      return;

   freeStack_taxa_k2TaxaId(taxaSTPtr);
   free(taxaSTPtr);
} /*freeHeap_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun05: setup_taxa_k2TaxaId
|   - allocates memory for a taxa_k2TaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to setup memory
|   - numElmSL:
|     o number of elements to allocate memory for
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_k2TaxaId for memory errors
\-------------------------------------------------------*/
signed char
setup_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed long numElmSL
){
   signed long slIndex = 0;

   /*make sure all memory is freeded first*/
   freeStack_taxa_k2TaxaId(taxaSTPtr);

   taxaSTPtr->idAryST = mk_str_ptrAry(numElmSL);

   if(! taxaSTPtr->idAryST)
      goto memErr_fun05;

   taxaSTPtr->codeArySL =
      calloc(
         numElmSL,
         sizeof(signed long)
      );

   if(! taxaSTPtr->codeArySL)
      goto memErr_fun05;


   taxaSTPtr->backTrackArySL =
      malloc(numElmSL * sizeof(signed long));
   if(! taxaSTPtr->backTrackArySL)
      goto memErr_fun05;
   for(slIndex = 0; slIndex < numElmSL; ++slIndex)
      taxaSTPtr->backTrackArySL[slIndex] = -1;
         /*so I know I am not backtracking*/


   taxaSTPtr->mergeAryBl =
      calloc(
         numElmSL,
         sizeof(signed char)
      );

   if(! taxaSTPtr->mergeAryBl)
      goto memErr_fun05;


   taxaSTPtr->levelArySS =
      calloc(
         numElmSL,
         sizeof(unsigned short)
      );

   if(! taxaSTPtr->levelArySS)
      goto memErr_fun05;



   taxaSTPtr->indexArySL =
      calloc(
         numElmSL,
         sizeof(signed long)
      );

   if(! taxaSTPtr->indexArySL)
      goto memErr_fun05;


   taxaSTPtr->sizeTaxaSL = numElmSL;
   blank_taxa_k2TaxaId(taxaSTPtr);

   return 0;

   memErr_fun05:;
      freeStack_taxa_k2TaxaId(taxaSTPtr);
      return def_memErr_k2TaxaId;
} /*setup_taxa_k2TaxaId*/


/*-------------------------------------------------------\
| Fun06: realloc_taxa_k2TaxaId
|   - reallocates memory for a taxa_k2TaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to resize
|   - numElmSL:
|     o number of elements to resize to
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to be realloced
|   - Returns:
|     o 0 for no errors
|     o def_memErr_k2TaxaId for memory errors
\-------------------------------------------------------*/
signed char
realloc_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed long numElmSL
){
   signed long *slPtr = 0;
   signed long slIter = 0;


   if(numElmSL == taxaSTPtr->numTaxaSL)
      return 0; /*nothing to do*/

   slPtr =
      realloc(
         taxaSTPtr->codeArySL,
         numElmSL * sizeof(signed long)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->codeArySL = slPtr;


    if(
       resize_str_ptrAry(
          taxaSTPtr->idAryST,
          numElmSL
       )
    ) goto memErr_fun05;


   slPtr =
      realloc(
         taxaSTPtr->backTrackArySL,
         numElmSL * sizeof(signed long)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->backTrackArySL = slPtr;


   slPtr =
      (signed long *)
      realloc(
         taxaSTPtr->mergeAryBl,
         numElmSL * sizeof(signed char)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->mergeAryBl = (signed char *) slPtr;


   slPtr =
      (signed long *)
      realloc(
         taxaSTPtr->levelArySS,
         numElmSL * sizeof(unsigned short)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->levelArySS = (signed short *) slPtr;


   slPtr =
      realloc(
         taxaSTPtr->indexArySL,
         numElmSL * sizeof(signed long)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->indexArySL = slPtr;


   for(
     slIter = taxaSTPtr->sizeTaxaSL;
     slIter < numElmSL;
     ++slIter
   ){ /*Loop: initialize arrays*/
      taxaSTPtr->indexArySL[slIter] = 0;
      taxaSTPtr->levelArySS[slIter] = 0;
      taxaSTPtr->mergeAryBl[slIter] = 0;
      taxaSTPtr->backTrackArySL[slIter] = -1;
         /*-1 so I know I am not backtracking*/
      taxaSTPtr->codeArySL[slIter] = 0;
   }  /*Loop: initialize arrays*/

   taxaSTPtr->sizeTaxaSL = numElmSL;
   return 0;

   memErr_fun05:;
      return def_memErr_k2TaxaId;
} /*realloc_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun07: sortCodes_taxa_k2TaxaId
|   - sorts the sortCodeArySL array while keeping the
|     indexArySL in sync a taxa_k2TaxaId
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to sort
| Output:
|   - Modifies:
|     o sortCodeArySL to be sorted lowest to greatest
|     o indexArySL to be in sync with sortCodeArySL
|       - idea: index is index to unsorted arrays
\-------------------------------------------------------*/
void
sortCodes_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
){
   /*Number of elements to sort (index 1)*/
   signed long endSL = taxaSTPtr->numTaxaSL - 1;

   /*for convience*/
   signed long *indexArySL = taxaSTPtr->indexArySL;
   signed long *codeArySL = taxaSTPtr->codeArySL;

   /*Number of sorting rounds*/
   signed long subSL = 0;
   signed long nextSL = 0;
   signed long lastSL = 0;
   signed long onSL = 0;

   /*Variables to incurment loops*/
   signed long slIndex = 0;
   signed long slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(endSL < 0)
      return; /*nothing to sort*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSL = 1; /*Initialzie first array*/

   while(subSL < endSL)
      subSL = (3 * subSL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subSL > 0)
   { /*Loop: all rounds*/
      for(
         slIndex = 0;
         slIndex <= subSL;
         ++slIndex
      ){ /*Loop: though sub array*/
         slElm = slIndex;

         for(
            slElm = slIndex;
            slElm + subSL <= endSL;
            slElm += subSL
         ){ /*Loop: swap elements in subarray*/
            nextSL = slElm + subSL;

            if(
                 codeArySL[ indexArySL[slElm] ]
               > codeArySL[ indexArySL[nextSL] ]
            ){ /*If I need to swap an element*/
               taxaSTPtr->indexArySL[slElm] ^=
                  taxaSTPtr->indexArySL[nextSL];
               taxaSTPtr->indexArySL[nextSL] ^=
                  taxaSTPtr->indexArySL[slElm];
               taxaSTPtr->indexArySL[slElm] ^=
                  taxaSTPtr->indexArySL[nextSL];

               lastSL = slElm;
               onSL = slElm;

               while(lastSL >= subSL)
               { /*loop: move swapped element back*/
                  lastSL -= subSL;

                  if(
                      codeArySL[ indexArySL[onSL] ]
                    > codeArySL[ indexArySL[lastSL] ]
                  ) break; /*position element*/

                  taxaSTPtr->indexArySL[onSL] ^=
                     taxaSTPtr->indexArySL[lastSL];
                  taxaSTPtr->indexArySL[lastSL] ^=
                     taxaSTPtr->indexArySL[onSL];
                  taxaSTPtr->indexArySL[onSL] ^=
                     taxaSTPtr->indexArySL[lastSL];

                  onSL = lastSL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subSL = (subSL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortCodes_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun08: findCode_taxa_k2TaxaId
|  - returns index of taxa code, searches sorted list
|    and returns unsorted list index
| Input:
|   - codeSL:
|     o taxa code to search for
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to search for code
| Output:
|  - Returns:
|    o index of taxa code
|    o -1 if taxa code not in taxaSTPtr
\-------------------------------------------------------*/
signed long
findCode_taxa_k2TaxaId(
   signed long codeSL,
   struct taxa_k2TaxaId *taxaSTPtr
){
   signed long midSL = 0;

   signed long rightSL =
      (signed long) taxaSTPtr->numTaxaSL - 1;

   signed long leftSL = 0;

   /*for convience*/
   signed long *indexArySL = taxaSTPtr->indexArySL;
   signed long *codeArySL = taxaSTPtr->codeArySL;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if( codeSL > codeArySL[ indexArySL[midSL] ])
         leftSL = midSL + 1;

     else if( codeSL < codeArySL[ indexArySL[midSL] ] )
         rightSL = midSL - 1;

     else
        return indexArySL[midSL];
   } /*Loop: Search for the querys index*/

   return -1; /*taxa code not found*/
} /*findCode_taxa_k2TaxaId*/

/*-------------------------------------------------------\
| Fun09: getLevel_k2TaxaId
|   - get the level in heirchyy of a kraken entry (col 4)
| Input:
|   - taxaStr:
|     o string with taxa assignment to get number for
| Output:
|   - Returns:
|     - taxa numuber + sub-taxa level (ex: for D1; D + 1)
|       o unclassified def_unclassifed_k2TaxaId; no level
|       o root def_root_k2TaxaId + level
|       o domain def_domain_k2TaxaId + level
|       o phylum def_phylum_k2TaxaId + level
|       o class def_class_k2TaxaId + level
|       o order def_order_k2TaxaId + level
|       o family def_family_k2TaxaId + level
|       o genus def_genus_k2TaxaId + level
|       o species def_species_k2TaxaId + level
\-------------------------------------------------------*/
signed short
getLevel_k2TaxaId(
   signed char *taxaStr
){
   switch(*taxaStr & ~32)
   { /*Switch: find taxa level*/
      case ' ':  /*empty case*/
      case '\t': /*empty case*/
         return def_unkown_k2TaxaId;
      case 'U':
         return def_unclassifed_k2TaxaId;

      case 'R':
         if(taxaStr[1] > 47)
            return def_root_k2TaxaId + taxaStr[1] - 48;
         else
            return def_root_k2TaxaId;

      case 'D':
         if(taxaStr[1] > 47)
            return def_domain_k2TaxaId + taxaStr[1] - 48;
         else
            return def_domain_k2TaxaId;

      case 'P':
         if(taxaStr[1] > 47)
            return def_phylum_k2TaxaId + taxaStr[1] - 48;
         else
            return def_phylum_k2TaxaId;

      case 'C':
         if(taxaStr[1] > 47)
            return def_class_k2TaxaId + taxaStr[1] - 48;
         else
            return def_class_k2TaxaId;

      case 'O':
         if(taxaStr[1] > 47)
            return def_order_k2TaxaId + taxaStr[1] - 48;
         else
            return def_order_k2TaxaId;

      case 'F':
         if(taxaStr[1] > 47)
            return def_family_k2TaxaId + taxaStr[1] - 48;
         else
            return def_family_k2TaxaId;

      case 'G':
         if(taxaStr[1] > 47)
            return def_genus_k2TaxaId + taxaStr[1] - 48;
         else
            return def_genus_k2TaxaId;

      case 'S':
         if(taxaStr[1] > 47)
            return def_species_k2TaxaId + taxaStr[1] -48;
         else
            return def_species_k2TaxaId;
   } /*Switch: find taxa level*/

   return def_unkown_k2TaxaId;
} /*getLevel_k2TaxaId*/

/*-------------------------------------------------------\
| Fun10: readReport_k2TaxaId
|   - gets codes of organisims for read ids print otu
| Input:
|   - startLevSS:
|     o lowest level (nearest root) to start printing for
|     o use getLevel_k2TaxaId to get this
|   - endLevSS:
|     o highest level (nearest tip) to start printing for
|     o use getLevel_k2TaxaId to get this
|   - minDepthUL:
|     o min read depth to keep an id (taxa)
|   - minPercDepthF:
|     o min percent read depth (0 to 100) to keep an id
|   - miniRepBl:
|     o 1: report is kraken2 minizer report (unique)
|     o 0: report is normal kraken2  report
|   - mergeRootBl:
|     o 1: merge lower tree (root) levels with their
|          upper (tip) levels
|     o 0: only keep ids mapping to its level
|   - mergeTipBl:
|     o 1: merge closer to tip not-targted taxa or low
|          depth taxa
|     o 0: do not merge
|   - errSCPtr:
|     o pointer to signed char to have errors
|   - inFILE:
|     o FILE pointer to kraken2 tsv report with taxons
|       to extract
| Output:
|   - Modifies:
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_k2TaxaId for memory errors
|       - def_fileErr_k2TaxaId for file errors
|       - def_noIds_k2TaxaId if no id's in file
|   - Returns:
|     o taxa_k2TaxaId with list of taxon ids and names
|       to extract
|       - negative numbers in codeArySL denote merged
|         tips (! pScrictBl) or for pScrictBl, taxa to
|         ignore
|     o 0 for no ids or error
\-------------------------------------------------------*/
struct taxa_k2TaxaId *
readReport_k2TaxaId(
   signed short startLevSS,     /*lowest level to print*/
   signed short endLevSS,       /*highest level to print*/
   unsigned long minDepthUL,    /*min depth to keep id*/
   float minPercDepthF,         /*min % depth to keep id*/
   signed char miniRepBl,       /*1: k2 minimizer report*/
   signed char mergeRootBl,     /*1: do not merge levels*/
   signed char mergeTipBl,      /*1: merge lower reads*/
   signed char *errSCPtr,
   void *inFILE                 /*kraken2 report*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   - gets codes of organisims to read ids out for
   '   o fun10 sec01:
   '     - variable declarations
   '   o fun10 sec02:
   '     - allocate memory for arrays
   '   o fun10 sec03:
   '     - get organism codes to print out
   '   o fun10 sec02:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_lenBuff_fun10 4096

   signed char buffStr[def_lenBuff_fun10 + 1];
   signed char *tmpStr = 0;

   signed long histArySL[def_maxDepth_k2TaxaId];
   signed short histLevArySS[def_maxDepth_k2TaxaId];
      /*keeps track of were at in tree*/

   unsigned long readDepthUL = 0;

   signed int depthSI = 0;     /*current depth in tree*/
   float percDepthF = 0; /*percent read depth*/
   signed int tmpSI = 1;
   signed int posSI = 1;
   signed short levelSS = 0;
      /*depth of current entry in tree*/
   signed short lastLevelSS = 0;

   #define def_expand_fun10 1028
   struct taxa_k2TaxaId *retTaxaHeapST;

   signed long tmpSL = 0; /*for getLine_fileFun*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - allocate memory for arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   retTaxaHeapST = malloc(sizeof(struct taxa_k2TaxaId));

   if(! retTaxaHeapST)
      goto memErr_fun10_sec04;

   init_taxa_k2TaxaId(retTaxaHeapST);

   *errSCPtr =
      setup_taxa_k2TaxaId(
         retTaxaHeapST,
         def_expand_fun10
      );

   if(*errSCPtr)
      goto memErr_fun10_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec03:
   ^   - get organism codes to print out
   ^   o fun10 sec03 sub01:
   ^     - move past % entry and get first line
   ^   o fun10 sec03 sub02:
   ^     - move past total reads count entry
   ^   o fun10 sec03 sub03:
   ^     - move past reads only assigned to this organim
   ^   o fun10 sec03 sub04:
   ^     - get level of organism in tree & move to name
   ^   o fun10 sec03 sub05:
   ^     - find last level or organism in tree
   ^   o fun10 sec03 sub06:
   ^     - check if in users level range
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun10 Sec03 Sub01:
   *   - move past % entry and get first line
   \*****************************************************/

   depthSI = -1; /*no position in history*/

   while(
      getLine_fileFun(
         inFILE,
         buffStr,
         def_lenBuff_fun10,
         &tmpSL
      )
   ){ /*Loop: process file*/
      tmpStr = buffStr;

      while(*tmpStr < 33)
      { /*Loop: get past white space*/
         ++tmpStr;
         if(*tmpStr == '\0')
            goto fileErr_fun10_sec04;
      } /*Loop: get past white space*/

      percDepthF = atof((char *) tmpStr);

      /*get past percentage*/
      while(*tmpStr++ > 32) ;

      while(*tmpStr < 33)
      { /*Loop: move to number reads*/
         ++tmpStr;
         if(*tmpStr == '\0')
            goto fileErr_fun10_sec04;
      } /*Loop: move to number reads*/

      /**************************************************\
      * Fun10 Sec03 Sub02:
      *   - move past total reads count entry
      \**************************************************/

      /*get past number of reads*/
      while(*tmpStr++ > 32) ;

      while(*tmpStr < 33)
      { /*Loop: move to number unique reads*/
         ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun10_sec04;
      } /*Loop: move to number unique reads*/

      /**************************************************\
      * Fun10 Sec03 Sub03:
      *   - move past reads only assigned to this organim
      \**************************************************/

      /*in some cases (viruses) kraken will assign tab
          for a lower taxonmic level ("root" and "Viruses"
          for virus database)
      */

      tmpStr +=
         strToUL_base10str(
            tmpStr,
            &readDepthUL
         );

      if(*tmpStr > 32)
         goto fileErr_fun10_sec04;
         /*incomplete conversion*/

      if(*tmpStr == '\0')
         goto fileErr_fun10_sec04;

      ++tmpStr;

      if(*tmpStr == '\0')
         goto fileErr_fun10_sec04;

      if(*tmpStr < 32)
         continue; /*unkown taxanomic level*/

      /**************************************************\
      * Fun10 Sec03 Sub04:
      *   - skip kraken2 minimizer report columns
      \**************************************************/

      if(miniRepBl)
      { /*If: need to move past kraken2 unique minimizer*/
         /*get past number taxon minimizers column*/
         while(*tmpStr > 32)
            ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun10_sec04;
 
         ++tmpStr;

         /*get past taxon unique minimizer estimate*/
         while(*tmpStr > 32)
            ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun10_sec04;
 
         ++tmpStr;
      } /*If: need to move past kraken2 unique minimizer*/
    
      /**************************************************\
      * Fun10 Sec03 Sub04:
      *   - get level of organism in tree & move to name
      \**************************************************/

      levelSS = getLevel_k2TaxaId(tmpStr);

      /*make sure not shifting from genus to domain or
      `   some other crazy shift
      */
      if(retTaxaHeapST->numTaxaSL > 0)
      { /*If: not first round*/
         if(lastLevelSS > levelSS)
         { /*If: last level needs to be reset*/
             retTaxaHeapST->mergeAryBl[
                  retTaxaHeapST->numTaxaSL - 1
               ] &= (~def_mergeUpBl_k2TaxaId);
         } /*If: last level needs to be reset*/
      } /*If: not first round*/


      lastLevelSS = levelSS;

      /*get past level entry*/
      while(*tmpStr++ > 32) ;

      while(*tmpStr < 33)
      { /*Loop: move orgainism id*/
         ++tmpStr;

         if(*tmpStr == '\0')
            goto fileErr_fun10_sec04;
      } /*Loop: move orgainism id*/

      if(levelSS == def_unkown_k2TaxaId)
         continue; /*no idea on taxa level*/
      if(levelSS == def_unclassifed_k2TaxaId)
         continue; /*unclassified read*/

      /**************************************************\
      * Fun10 Sec03 Sub05:
      *   - find last level or organism in tree
      \**************************************************/

      if(depthSI > 0)
      { /*If: have a histroy*/
         while(levelSS <= histLevArySS[depthSI])
         { /*Loop: find last level for this organism*/
            --depthSI;

            if(depthSI < 0)
               break; /*at last level*/
         } /*Loop: find last level for this organism*/
      } /*If: have a histroy*/

      ++depthSI; /*move to fi*/
      histLevArySS[depthSI] = levelSS;

      /*filtering*/
      if(
            ! mergeRootBl
         && ! mergeTipBl
      ){ /*If: not merging reads (do check later)*/
         if(levelSS <= endLevSS)
            continue; /*to high (near tip) level to keep*/

         if(readDepthUL < minDepthUL)
            continue; /*not enough read depth*/

         if(percDepthF < minPercDepthF)
            continue;
      } /*If: not merging reads (do check later)*/

      else
      { /*Else: merging reads*/

         if(depthSI)
            histArySL[depthSI] = histArySL[depthSI - 1];
            /*not assigned yet*/
         else
            histArySL[depthSI] = -1; /*no history yet*/

         /*not safe for merging*/
         /*if(readDepthUL < 1)
            continue;*/ /*no reads for taxa id*/

         if(
               ! mergeTipBl
            && levelSS <= endLevSS
         ) continue; /*to high (near tip) level to keep*/
      } /*Else: merging reads*/

      /**************************************************\
      * Fun10 Sec03 Sub06:
      *   - check if in users level range
      *   o fun10 sec03 sub06 cat01:
      *     - if check and resize arrays if needed
      *   o fun10 sec03 sub06 cat02:
      *     - copy organism code (id)
      *   o fun10 sec03 sub06 cat03:
      *     - get organism name (also remove spaces)
      *   o fun10 sec03 sub06 cat04:
      *     - merge taxa levels if merging mergeRootBl
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun10 Sec03 Sub06 Cat01:
      +   - if check and resize arrays if needed
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(levelSS >= startLevSS)
      { /*If: at desired tree depth for print*/
         if(
               retTaxaHeapST->numTaxaSL
            >= retTaxaHeapST->sizeTaxaSL
         ){ /*If: need to resize array*/
            if(
               realloc_taxa_k2TaxaId(
                  retTaxaHeapST,
                  retTaxaHeapST->sizeTaxaSL
                     + def_expand_fun10

               )
            ) goto memErr_fun10_sec04;
         } /*If: need to resize array*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat02:
         +   - copy organism code (id)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               &retTaxaHeapST->codeArySL[
                  retTaxaHeapST->numTaxaSL
               ]
            );

         if(*tmpStr > 32)
            goto fileErr_fun10_sec04;
            /*not white space*/

         /*sorted index to get codes*/
         retTaxaHeapST->indexArySL[
            retTaxaHeapST->numTaxaSL
         ] = retTaxaHeapST->numTaxaSL;

         retTaxaHeapST->levelArySS[
            retTaxaHeapST->numTaxaSL
         ] = levelSS;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat03:
         +   - get organism name (also remove spaces)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(*tmpStr < 33)
         { /*Loop: move to level*/
            ++tmpStr;

            if(*tmpStr == '\0')
               goto fileErr_fun10_sec04;
         } /*Loop: move to level*/

         add_str_ptrAry(
            tmpStr,
            retTaxaHeapST->idAryST,
            retTaxaHeapST->numTaxaSL
         ); /*add orgainism name to id array*/

         tmpStr =
            retTaxaHeapST->idAryST->strAry[
               retTaxaHeapST->numTaxaSL
            ];

         while(*tmpStr != '\0')
         { /*Loop: remove spaces*/
            if(*tmpStr == ' ')
               *tmpStr = '_';

            else if(*tmpStr < 32)
            { /*Else If: at end of c-string*/
               *tmpStr = '\0';
               break;
            } /*Else If: at end of c-string*/

            ++tmpStr;
         } /*Loop: remove spaces*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat04:
         +   - merge tip taxa into root checks
         \++++++++++++++++++++++++++++++++++++++++++++++*/

          if(mergeTipBl)
          { /*If: merging tip into the root*/
             if(levelSS > endLevSS)
             { /*If: to near tip*/
                 retTaxaHeapST->mergeAryBl[
                      retTaxaHeapST->numTaxaSL
                   ] |= def_mergeDownBl_k2TaxaId;

                  ++retTaxaHeapST->numTaxaSL;
                  continue;
             } /*If: to near tip*/

             else if(
                   readDepthUL < minDepthUL
                || percDepthF < minPercDepthF
             ){ /*Else If: to little read depth*/
                 retTaxaHeapST->mergeAryBl[
                      retTaxaHeapST->numTaxaSL
                    ] |= def_lowDepth_k2TaxaId;

                 retTaxaHeapST->mergeAryBl[
                      retTaxaHeapST->numTaxaSL
                   ] |= def_mergeDownBl_k2TaxaId;

                if(! mergeRootBl)
                { /*If: not merging towards root*/
                   ++retTaxaHeapST->numTaxaSL;
                   continue;
                } /*If: not merging towards root*/
             } /*Else If: to little read depth*/
          } /*If: merging tip into the root*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat05:
         +   - merge root taxa into tip checks
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(mergeRootBl)
         { /*If: merging root into a tip*/
            histArySL[depthSI] = retTaxaHeapST->numTaxaSL;

            if(! retTaxaHeapST->numTaxaSL)
               retTaxaHeapST->backTrackArySL[0] = -1;
            else
               retTaxaHeapST->backTrackArySL[
                  retTaxaHeapST->numTaxaSL
               ] = histArySL[depthSI - 1];

            if(levelSS > endLevSS)
            { /*Else If: near tip, but not merging tips*/
                retTaxaHeapST->mergeAryBl[
                     retTaxaHeapST->numTaxaSL
                  ] = def_skip_k2TaxaId;/*merging*/

                 ++retTaxaHeapST->numTaxaSL;
                 continue;
            } /*Else If: near tip, but not merging tips*/

            else if(! mergeRootBl)
            { /*Else If: merging tip only*/
               retTaxaHeapST->mergeAryBl[
                    retTaxaHeapST->numTaxaSL
                 ] = def_skip_k2TaxaId;/*merging*/

               ++retTaxaHeapST->numTaxaSL;
               continue; /*not merging tip into root*/
            } /*Else If: merging tip only*/

            else if(
                  readDepthUL < minDepthUL
               || percDepthF < minPercDepthF
            ){ /*If: to little read depth (merge only*/
                 retTaxaHeapST->mergeAryBl[
                      retTaxaHeapST->numTaxaSL
                    ] |= def_lowDepth_k2TaxaId;

                retTaxaHeapST->mergeAryBl[
                     retTaxaHeapST->numTaxaSL
                  ] |= def_mergeUpBl_k2TaxaId; /*merging*/

                  ++retTaxaHeapST->numTaxaSL;
                  continue;
            } /*If: to little read depth (merge only*/

            tmpSI = depthSI - 1;
            posSI = (signed int) retTaxaHeapST->numTaxaSL;
            --posSI;

            while(tmpSI >= 0)
            { /*Loop: update past depths*/
               if(posSI < 0)
                  break; /*should never happen*/

               /*if(histArySL[tmpSI] < 0)
                  break;*/ /*rest of history has no reads*/

               if(
                     levelSS
                  <= retTaxaHeapST->levelArySS[posSI]
               ) break; /*nothing to update*/

               levelSS = retTaxaHeapST->levelArySS[posSI];

               if(retTaxaHeapST->mergeAryBl[posSI])
               { /*If: is already marked as merged*/

                  --tmpSI;
                  --posSI;
                  continue; /*already marked as merge*/
               } /*If: is already marked as merged*/

               retTaxaHeapST->mergeAryBl[posSI] |=
                  def_mergeUpBl_k2TaxaId;

               --tmpSI;
               --posSI;
            } /*Loop: update past depths*/

            ++retTaxaHeapST->numTaxaSL;
            continue;
         } /*If: merging root into a tip*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat05:
         +    - move to next id (non-merbge
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(levelSS >= endLevSS)
            ++retTaxaHeapST->numTaxaSL;
            /*not merging, so only want lowest level*/
     } /*If: at desired tree depth for print*/

     /*else to high (closer to tip of tree), ignore*/
   } /*Loop: process file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*sort sorted code array for quick look ups later*/
   sortCodes_taxa_k2TaxaId(retTaxaHeapST);

   *errSCPtr = 0;
   goto ret_fun10_sec04;

   memErr_fun10_sec04:;
      *errSCPtr = def_memErr_k2TaxaId;
      goto cleanErr_fun10_sec04;

   fileErr_fun10_sec04:;
      *errSCPtr = def_fileErr_k2TaxaId;
      goto cleanErr_fun10_sec04;

   cleanErr_fun10_sec04:;
      if(retTaxaHeapST)
         freeHeap_taxa_k2TaxaId(retTaxaHeapST);
      retTaxaHeapST = 0;

      goto ret_fun10_sec04;

   ret_fun10_sec04:;
      return retTaxaHeapST;
} /*readReport_k2TaxaId*/

/*-------------------------------------------------------\
| Fun11: mkTaxaIdFile_k2TaxaIds
|   - prints a single taxa id (here to avoid clutter)
| Input:
|   - idStr:
|     o c-string with read id to print
|   - buffStr:
|     o c-string buffer to hold file name
|   - prefixStr:
|     o c-string with file prefix
|   - fileTaxaSL:
|     o file taxa code assigning taxon to
|   - fileTaxaStr:
|     o c-string with file taxa name assigning taxon to
|   - taxaSL:
|     o taxa code (number) of id's taxon
|   - taxaStr:
|     o c-string with the name of read ids taxon
| Output:
|   - Modifies:
|     o buffStr to have output file name
|   - Prints:
|     o read id to taxon id file
|   - Returns:
|     o 0 for no errors
|     o def_fileErr_k2TaxaIds for file errors
\-------------------------------------------------------*/
signed char
pTaxaId_k2TaxaIds(
   signed char *idStr,     /*read id to print*/
   signed char *buffStr,   /*buffer to store file name*/
   signed char *prefixStr, /*prefix for file name*/
   signed long fileTaxaSL, /*taxa code file printing to*/
   signed char *fileTaxaStr,/*taxa name file printing to*/
   signed long taxaSL,     /*taxa id*/
   signed char *taxaStr    /*string with taxa name*/
){
   signed char *tmpStr = buffStr;
   signed int prefixLenSI = 0;
   FILE *outFILE = 0;

   /*copy file name*/
   prefixLenSI = cpLine_ulCp(tmpStr, prefixStr);
   tmpStr += prefixLenSI;
      /*the user might have file paths in the prefix,
      `  so I need to make sure it does not become an
      `  issue in the clean up step
      */
   *tmpStr++ = '-';

   /*copy taxa code*/
   tmpStr += numToStr( tmpStr, fileTaxaSL);
   *tmpStr++ = '-';

   /*copy taxa name*/
   tmpStr += cpLine_ulCp(tmpStr, fileTaxaStr);
  
   *tmpStr++ = '.';
   *tmpStr++ = 'i';
   *tmpStr++ = 'd';
   *tmpStr++ = 's';
   *tmpStr = '\0';
  
   tmpStr = buffStr + prefixLenSI;
   while(*tmpStr)
   { /*Loop: remove invalid characters*/
      if(*tmpStr > 47 && *tmpStr < 58)
         ; /*this is a number*/
      else if(*tmpStr >= 'A' && *tmpStr <= 'Z')
         ; /*uppercase letter*/
      else if(*tmpStr >= 'a' && *tmpStr <= 'z')
         ; /*lowercase letter*/
      else if(*tmpStr == '_')
         ; /*underscore*/
      else if(*tmpStr == '-')
         ; /*dash*/
      else if(*tmpStr == '.')
         ; /*preiod for file extensions*/
      else
         *tmpStr = '_'; /*invalid character of some kind,
                        `  replace with an `_`
                        */
      ++tmpStr;
   } /*Loop: remove invalid characters*/

   outFILE = fopen((char *) buffStr, "a");
  
   if(! outFILE)
      return def_fileErr_k2TaxaId;
  
   fprintf(
      (FILE *) outFILE,
      "%s\t%li\t%s%s",
      idStr,
      taxaSL,
      taxaStr,
      str_endLine
   );
  
   fclose(outFILE);
   outFILE = 0;

   return 0;
} /*pTaxaId_k2TaxaIds*/

/*-------------------------------------------------------\
| Fun12: pIds_k2TaxaId
|   - prints out read ids by taxa for kraken2
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId struct with taxa codes
|       to extract/merge
|   - prefixStr:
|     o c-string with prefix to name files with
|   - pUnclassBl:
|     o 1: print unclassifed reads
|     o 0: do not print unclassifed reads
|   - pMergeTipsBl:
|     o 1: if trying to merge tip into a non-existent
|          tip (deeper in tree), then print the tip if
|          does not have def_lowDepth_k2TaxaId flag
|     o 0: only print non-skip, non-merge reads
|   - inFILE:
|     o FILE pointer to kraken2 tsv output with read ids
|       and their taxnomic assignments
| Output:
|   - Prints:
|     o reads to files named "prefixStr-code-taxa.ids"
|       were code is taxa code of file and taxa is taxa
|       name for code
|   - Returns:
|     o 0 for no errors
|     o 1 + line on for file errors
\-------------------------------------------------------*/
signed long
pIds_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed char *prefixStr,
   signed char pUnclassBl,
   void *inFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC:
   '   - prints out read ids by taxa for kraken2
   '   o fun12 sec01:
   '     - variable declarations
   '   o fun12 sec02:
   '     - set up unclassifed file (if printing)
   '   o fun12 sec03:
   '     - get read ids
   '   o fun12 sec04:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;

   signed long taxaSL = 0;
   signed long indexSL = 0;
   signed long oldSL = 0;

   signed short levelSS = 0; /*level taxa is at*/

   #define maxHist_fun12_k2TaxaId 32
   signed int histPosSI = 0;
   signed int histIndexSI = 0;
   signed long histArySL[maxHist_fun12_k2TaxaId]; 
      /*history of taxa merging*/

   #define idLen_fun12_k2TaxaId 256
   signed char idStr[idLen_fun12_k2TaxaId];

   signed char noClassStr[idLen_fun12_k2TaxaId];
   signed char fileStr[4096]; /*file name for ids*/
   signed char *tmpStr = 0;


   #define def_sizeBuff_fun12 (1 << 13)
   signed char buffStr[def_sizeBuff_fun12 + 9];

   FILE *unclassFILE = 0;
   signed long tmpSL = 0; /*for getLine_fileFun*/
   signed long lineSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - set up unclassifed file (if printing to)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(pUnclassBl)
   { /*If: printing unclassified reads*/
      tmpStr = noClassStr;

      tmpStr += cpLine_ulCp(tmpStr, prefixStr);
         /*set up file name*/

      *tmpStr++ = '-';

      tmpStr +=
         cpLine_ulCp(
            tmpStr,
            (signed char *) "unclassified.ids"
         ); /*set up file name*/

      unclassFILE = fopen((char *) noClassStr, "w");

      if(! unclassFILE)
         goto fileErr_fun12_sec04;
   } /*If: printing unclassified reads*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - get read ids
   ^   o fun12 sec03 sub01:
   ^     - deal with unclassified ids + start loop
   ^   o fun12 sec03 sub02:
   ^     - get read id
   ^   o fun12 sec03 sub03:
   ^     - merge taxa down (towards root)
   ^   o fun12 sec03 sub04:
   ^     - handel taxa merging cases
   ^   o fun12 sec03 sub05:
   ^     - merge taxa up (towards tip)
   ^   o fun12 sec03 sub06:
   ^     - handel not merging taxa cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec03 Sub01:
   *   - deal with unclassified ids + start loop
   \*****************************************************/

   while(
      getLine_fileFun(
         inFILE,
         buffStr,
         def_sizeBuff_fun12,
         &tmpSL
      )
   ){ /*Loop: get read ids*/
      ++lineSL;

      if(buffStr[0] == 'U')
      { /*If: unclassified read*/
         if(! pUnclassBl)
            goto nextLine_fun12_sec03_sub07;

         while(*tmpStr < 33)
         { /*Loop: move to read id*/
            ++tmpStr;
            if(*tmpStr == '\0')
               goto fileErr_fun12_sec04;
         } /*Loop: move to read id*/

         /*get the read id (no white space)*/
         tmpStr += cpWhite_ulCp(idStr, tmpStr);

         fprintf(
            (FILE *) unclassFILE,
            "%s\tunclassified%s",
            idStr,
            str_endLine
         );

         goto nextLine_fun12_sec03_sub07;
      } /*If: unclassified read*/

      /**************************************************\
      * Fun12 Sec03 Sub02:
      *   - get read id
      \**************************************************/

      tmpStr = buffStr;

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: move to read id*/
         ++tmpStr;
         if(*tmpStr == '\0')
            goto fileErr_fun12_sec04;
      } /*Loop: move to read id*/

      /*get the read id (no white space)*/
      tmpStr += cpWhite_ulCp(idStr, tmpStr);

      while(*tmpStr < 33)
      { /*Loop: move to taxa id*/
         ++tmpStr;
         if(*tmpStr == '\0')
            goto fileErr_fun12_sec04;
      } /*Loop: move to taxa id*/

      /**************************************************\
      * Fun12 Sec03 Sub03:
      *   - get taxa code and see if printing
      \**************************************************/

      /*get taxa code*/
      tmpStr += strToSL_base10str(tmpStr, &taxaSL);

      indexSL = findCode_taxa_k2TaxaId(taxaSL, taxaSTPtr);

      if(indexSL < 0)
         goto nextLine_fun12_sec03_sub07;

      /**************************************************\
      * Fun12 Sec03 Sub04:
      *   - merge taxa down (towards root)
      *   o fun12 sec03 sub04 cat01:
      *     - find taxa to merge into
      *   o fun12 sec03 sub04 cat02:
      *     - print taxa id to file
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun12 Sec03 Sub04 Cat01:
      +   - find taxa to merge into
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
        taxaSTPtr->mergeAryBl[indexSL] & def_skip_k2TaxaId
      ) goto nextLine_fun12_sec03_sub07;

      else if(
            taxaSTPtr->mergeAryBl[indexSL]
          & def_mergeDownBl_k2TaxaId
      ){ /*Else If: merging tip into deeper taxa*/
         oldSL = indexSL; /*save for later*/
         indexSL = taxaSTPtr->backTrackArySL[indexSL];

         if(indexSL < 0)
         { /*If: nothing to backtrack to*/
            indexSL = oldSL;
            levelSS = taxaSTPtr->mergeAryBl[indexSL];

            if( levelSS & def_mergeUpBl_k2TaxaId )
               goto mergeUp_fun12_sec03_sub05_cat01;
            else
               goto nextLine_fun12_sec03_sub07;
         } /*If: nothing to backtrack to*/

         while(indexSL >= 0)
         { /*Loop: find taxa to merge into*/
            levelSS = taxaSTPtr->mergeAryBl[indexSL];

            if(! levelSS )
               break; /*found taxa to merge into*/

            if(taxaSTPtr->backTrackArySL[indexSL] < 0)
            { /*If: nothing to backtrack to*/
               indexSL = oldSL;

               if( levelSS & def_mergeUpBl_k2TaxaId )
                  goto mergeUp_fun12_sec03_sub05_cat01;
               else
                  goto nextLine_fun12_sec03_sub07;
            } /*If: nothing to backtrack to*/

            if(! indexSL)
               goto nextLine_fun12_sec03_sub07;
               /*nothing to merge into*/
            else
               indexSL =
                  taxaSTPtr->backTrackArySL[indexSL];
         } /*Loop: find taxa to merge into*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun12 Sec03 Sub04 Cat02:
         +   - print taxa id to file
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         errSC =
            pTaxaId_k2TaxaIds(
               idStr,         /*read id to print*/
               fileStr,       /*buffer for file name*/
               prefixStr,     /*prefix for file name*/
               taxaSTPtr->codeArySL[indexSL],
               taxaSTPtr->idAryST->strAry[indexSL],
               taxaSL,        /*acutal taxa code*/
               taxaSTPtr->idAryST->strAry[oldSL]
                  /*actual taxon name*/
            );

         if(errSC)
            goto fileErr_fun12_sec04;

         if(
              taxaSTPtr->mergeAryBl[indexSL]
            & def_mergeUpBl_k2TaxaId
         ){ /*If: also can merge towards tip*/
            indexSL = oldSL;
            goto mergeUp_fun12_sec03_sub05_cat01;
         } /*If: also can merge towards tip*/

      } /*Else If: merging tip into deeper taxa*/

      /**************************************************\
      * Fun12 Sec03 Sub05:
      *   - merge taxa up (towards tip)
      *   o fun12 sec03 sub05 cat01:
      *     - set up for loop + start loop
      *   o fun12 sec03 sub05 cat02:
      *     - check if printing id to a taxa
      *   o fun12 sec03 sub05 cat03:
      *     - passed checks; print id
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun12 Sec03 Sub05 Cat01:
      +   - set up for loop + start loop
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
           taxaSTPtr->mergeAryBl[indexSL]
         & def_mergeUpBl_k2TaxaId
      ){ /*Else If: merging into a tip or higher taxa*/
         mergeUp_fun12_sec03_sub05_cat01:;

         oldSL = indexSL;

         levelSS = taxaSTPtr->levelArySS[indexSL];

         histArySL[0] = indexSL;
         ++indexSL; /*move off current taxa*/

         histPosSI = 0;

         while(histPosSI >= 0)
         { /*Loop: find ids merging*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun12 Sec03 Sub05 Cat02:
            +   - check if printing id to a taxa
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(
                  indexSL
               >= (signed long) taxaSTPtr->numTaxaSL
            ) break; /*no more taxa in list*/

            if(levelSS >= taxaSTPtr->levelArySS[indexSL])
               break; /*at same level (end of merge)*/

            if(taxaSTPtr->mergeAryBl[indexSL] < 0)
               histPosSI = 0; /*If no trace back*/
               /*at end of trace*/

            histIndexSI =
               slSearch_shellSort(
                  histArySL,
                  taxaSTPtr->backTrackArySL[indexSL],
                  histPosSI + 1
               ); /*find actual index*/

            ++histPosSI;

            histArySL[histPosSI] = 
               taxaSTPtr->backTrackArySL[indexSL];

            if(histIndexSI < 0) ;

            else 
               histPosSI = histIndexSI;
               /*last time saw this taxa level*/

            if(taxaSTPtr->mergeAryBl[indexSL] != 0)
            { /*If: merging this level*/
               ++indexSL; /*move up to next level*/
               continue;
            } /*If: merging this level*/

           /*++++++++++++++++++++++++++++++++++++++++++++\
           + Fun12 Sec03 Sub05 Cat03:
           +   - passed checks; print id
           \++++++++++++++++++++++++++++++++++++++++++++*/
  
           errSC =
              pTaxaId_k2TaxaIds(
                 idStr,     /*read id to print*/
                 fileStr,   /*no longer care about*/
                 prefixStr, /*prefix for file name*/
                 taxaSTPtr->codeArySL[indexSL],
                 taxaSTPtr->idAryST->strAry[indexSL],
                 taxaSL,        /*acutal taxa code*/
                 taxaSTPtr->idAryST->strAry[oldSL]
                 /*actual taxon name*/
              );

           if(errSC)
              goto fileErr_fun12_sec04;
  
           ++indexSL;
        } /*Loop: while merging taxa*/
      } /*Else If: merging into a tip or higher taxa*/

      /**************************************************\
      * Fun12 Sec03 Sub06:
      *   - handel not merging taxa cases
      \**************************************************/

      else
      { /*Else: not merging*/
         errSC =
            pTaxaId_k2TaxaIds(
               idStr,     /*read id to print*/
               fileStr,   /*buffer for file name*/
               prefixStr, /*prefix for file name*/
               taxaSTPtr->codeArySL[indexSL],
               taxaSTPtr->idAryST->strAry[indexSL],
               taxaSL,        /*acutal taxa code*/
               taxaSTPtr->idAryST->strAry[indexSL]
               /*actual taxon name*/
            );

           if(errSC)
              goto fileErr_fun12_sec04;
      } /*Else: not merging*/

      /**************************************************\
      * Fun12 Sec03 Sub07:
      *   - move to next line
      \**************************************************/

      nextLine_fun12_sec03_sub07:;

      tmpStr = buffStr;
      tmpStr += endStr_ulCp(tmpStr);
      --tmpStr;
      /*move on to character before '\0'*/

      while(
            *tmpStr != '\r'
         && *tmpStr != '\n'
      ){ /*Loop: move past line*/
         tmpSL = 
            getLine_fileFun(
               inFILE,
               buffStr,
               def_sizeBuff_fun12,
               &tmpSL
            );

          if(! tmpSL)
             goto done_fun12_sec04;

          tmpStr = buffStr;
          tmpStr += endStr_ulCp(tmpStr);
          --tmpStr;
          /*move on to character before '\0'*/
      } /*Loop: move past line*/
   } /*Loop: get read ids*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec04:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun12_sec04:;
      lineSL = 0;
      goto cleanUp_fun12_sec04;

   fileErr_fun12_sec04:;
      ++lineSL;/*convert to index 1 to mark a file error*/
      goto cleanUp_fun12_sec04;

   cleanUp_fun12_sec04:;
      if(unclassFILE)
         fclose(unclassFILE);
      unclassFILE = 0;

      return lineSL;
} /*pIds_k2TaxaId*/

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
