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
'   o fun11: pIds_k2TaxaId
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
#include "../genLib/strAry.h"

/*no .c files*/
#include "../genLib/dataTypeShortHand.h"

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
   taxaSTPtr->idAryStr = 0;
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

   if(taxaSTPtr->idAryStr)
     free(taxaSTPtr->idAryStr);

   if(taxaSTPtr->backTrackArySL)
     free(taxaSTPtr->backTrackArySL);

   if(taxaSTPtr->mergeAryBl)
     free(taxaSTPtr->mergeAryBl);

   if(taxaSTPtr->levelArySS)
     free(taxaSTPtr->levelArySS);

   if(taxaSTPtr->indexArySL)
     free(taxaSTPtr->indexArySL);

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
   /*make sure all memory is freeded first*/
   freeStack_taxa_k2TaxaId(taxaSTPtr);

   taxaSTPtr->codeArySL =
      calloc(
         numElmSL,
         sizeof(slong)
      );

   if(! taxaSTPtr->codeArySL)
      goto memErr_fun05;


   taxaSTPtr->idAryStr = mk_strAry(numElmSL);

   if(! taxaSTPtr->idAryStr)
      goto memErr_fun05;


   taxaSTPtr->backTrackArySL =
      calloc(
         numElmSL,
         sizeof(slong)
      );

   if(! taxaSTPtr->backTrackArySL)
      goto memErr_fun05;


   taxaSTPtr->mergeAryBl =
      calloc(
         numElmSL,
         sizeof(schar)
      );

   if(! taxaSTPtr->mergeAryBl)
      goto memErr_fun05;


   taxaSTPtr->levelArySS =
      calloc(
         numElmSL,
         sizeof(ushort)
      );

   if(! taxaSTPtr->levelArySS)
      goto memErr_fun05;



   taxaSTPtr->indexArySL =
      calloc(
         numElmSL,
         sizeof(ushort)
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
   slong *slPtr = 0;


   if(numElmSL == taxaSTPtr->numTaxaSL)
      return 0; /*nothing to do*/

   slPtr =
      realloc(
         taxaSTPtr->codeArySL,
         numElmSL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->codeArySL = slPtr;


    slPtr =
      (slong *)
      realloc_strAry(
         taxaSTPtr->idAryStr,
         numElmSL
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->idAryStr = (schar *) slPtr;


   slPtr =
      realloc(
         taxaSTPtr->backTrackArySL,
         numElmSL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->backTrackArySL = slPtr;


   slPtr =
      (slong *)
      realloc(
         taxaSTPtr->mergeAryBl,
         numElmSL * sizeof(schar)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->mergeAryBl = (schar *) slPtr;


   slPtr =
      (slong *)
      realloc(
         taxaSTPtr->levelArySS,
         numElmSL * sizeof(ushort)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->levelArySS = (sshort *) slPtr;


   slPtr =
      realloc(
         taxaSTPtr->indexArySL,
         numElmSL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->indexArySL = slPtr;


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
   slong numElmSL = taxaSTPtr->numTaxaSL;
   slong endSL = taxaSTPtr->numTaxaSL - 1;

   /*for convience*/
   slong *indexArySL = taxaSTPtr->indexArySL;
   slong *codeArySL = taxaSTPtr->codeArySL;

   /*Number of sorting rounds*/
   slong subSL = 0;
   slong nextSL = 0;
   slong lastSL = 0;
   slong onSL = 0;

   /*Variables to incurment loops*/
   slong slIndex = 0;
   slong slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmSL)
      return; /*nothing to sort*/

   /*Recursion formsla: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSL = 1; /*Initialzie first array*/

   while(subSL < numElmSL - 1)
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
      (slong) taxaSTPtr->numTaxaSL - 1;

   signed long leftSL = 0;

   /*for convience*/
   slong *indexArySL = taxaSTPtr->indexArySL;
   slong *codeArySL = taxaSTPtr->codeArySL;

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
|   - mergeRootBl:
|     o 1: merge lower tree (root) levels with their
|          upper (tip) levels
|     o 0: only keep ids mapping to its level
|   - mergeTipBl:
|     o 1: merge closer to tip not-targted taxa or low
|       depth taxa
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
   signed char mergeRootBl,       /*1: do not merge levels*/
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

   schar buffStr[def_lenBuff_fun10 + 1];
   schar *tmpStr = 0;

   slong histArySL[def_maxDepth_k2TaxaId];
   sshort histLevArySS[def_maxDepth_k2TaxaId];
      /*keeps track of were at in tree*/

   ulong readDepthUL = 0;

   sint depthSI = 0;     /*current depth in tree*/
   float percDepthF = 0; /*percent read depth*/
   sint tmpSI = 1;
   sint posSI = 1;
   sshort levelSS = 0;  /*depth of current entry in tree*/

   #define def_expand_fun10 1028
   struct taxa_k2TaxaId *retTaxaHeapST;

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
      fgets(
         (char *) buffStr,
         def_lenBuff_fun10,
         (FILE *) inFILE
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
      *   - get level of organism in tree & move to name
      \**************************************************/

      levelSS = getLevel_k2TaxaId(tmpStr);

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

         if(readDepthUL < 1)
            continue; /*no reads for taxa id*/

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

         add_strAry(
            tmpStr,
            retTaxaHeapST->idAryStr,
            retTaxaHeapST->numTaxaSL
         ); /*add orgainism name to id array*/

         tmpStr =
            get_strAry(
              retTaxaHeapST->idAryStr,
              retTaxaHeapST->numTaxaSL
            );

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
         +   - merge taxa levels if merging (mergeRootBl)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(
               mergeRootBl
            || mergeTipBl
         ){ /*If: merging ids*/
            histArySL[depthSI] = retTaxaHeapST->numTaxaSL;

            retTaxaHeapST->backTrackArySL[
               retTaxaHeapST->numTaxaSL
            ] = histArySL[depthSI - 1];

            if(
                  levelSS > endLevSS
               && mergeTipBl
            ){ /*If: to near tip*/
                retTaxaHeapST->mergeAryBl[
                     retTaxaHeapST->numTaxaSL
                  ] = def_mergeDownBl_k2TaxaId;/*merging*/

                 ++retTaxaHeapST->numTaxaSL;
                 continue;
            } /*If: to near tip*/

            else if(levelSS > endLevSS)
            { /*Else If: near tip, but not merging tips*/
                retTaxaHeapST->mergeAryBl[
                     retTaxaHeapST->numTaxaSL
                  ] = def_skip_k2TaxaId;/*merging*/

                 ++retTaxaHeapST->numTaxaSL;
                 continue;
            } /*Else If: near tip, but not merging tips*/

            else if(
                  readDepthUL < minDepthUL
               || percDepthF < minPercDepthF
            ){ /*If: to little read depth (merge only*/
                retTaxaHeapST->mergeAryBl[
                     retTaxaHeapST->numTaxaSL
                  ] = def_mergeUpBl_k2TaxaId; /*merging*/

                  ++retTaxaHeapST->numTaxaSL;
                  continue;
            } /*If: to little read depth (merge only*/

            tmpSI = depthSI - 1;
            posSI = (sint) retTaxaHeapST->numTaxaSL - 1;
            ++retTaxaHeapST->numTaxaSL;

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

               retTaxaHeapST->mergeAryBl[posSI] =
                  def_mergeUpBl_k2TaxaId;

               --tmpSI;
               --posSI;
            } /*Loop: update past depths*/
         } /*If: merging ids*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat05:
         +    - move to next id (non-merbge
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(levelSS == endLevSS)
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
| Fun11: pIds_k2TaxaId
|   - prints out read ids by taxa for kraken2
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId struct with taxa codes
|       to extract/merge
|   - prefixStr:
|     o c-string with prefix to name files with
|   - pUnclassBl:
|     0 1: print unclassifed reads
|     0 0: do not print unclassifed reads
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
|     o def_fileErr_k2TaxaId for file errors
\-------------------------------------------------------*/
signed char
pIds_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed char *prefixStr,
   signed char pUnclassBl,
   void *inFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - prints out read ids by taxa for kraken2
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - set up unclassifed file (if printing)
   '   o fun11 sec03:
   '     - get read ids
   '   o fun11 sec04:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;

   slong taxaSL = 0;
   slong indexSL = 0;
   slong backSL = 0;

   sshort levelSS = 0; /*level taxa is at*/

   #define maxHist_fun11_k2TaxaId 32
   sint histPosSI = 0;
   slong histArySL[maxHist_fun11_k2TaxaId]; 
      /*history of taxa merging*/

   #define idLen_fun11_k2TaxaId 256
   signed char idStr[idLen_fun11_k2TaxaId];

   signed char noClassStr[idLen_fun11_k2TaxaId];
   schar *tmpStr = 0;


   #define lenBuff_fun11_k2TaxaId 1 << 15
   signed char buffStr[lenBuff_fun11_k2TaxaId];

   FILE *unclassFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - set up unclassifed file (if printing to)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(pUnclassBl)
   { /*If: printing unclassified reads*/
      tmpStr = noClassStr;

      tmpStr +=
         cpLine_ulCp(
            tmpStr,
            prefixStr
         ); /*set up file name*/

      *tmpStr++ = '-';

      tmpStr +=
         cpLine_ulCp(
            tmpStr,
            (schar *) "unclassified.ids"
         ); /*set up file name*/

      unclassFILE =
         fopen(
            (char *) noClassStr,
            "w"
         );

      if(! unclassFILE)
         goto fileErr_fun11_sec04;
   } /*If: printing unclassified reads*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - get read ids
   ^   o fun11 sec03 sub01:
   ^     - deal with unclassified ids + start loop
   ^   o fun11 sec03 sub02:
   ^     - get read id
   ^   o fun11 sec03 sub03:
   ^     - merge taxa down (towards root)
   ^   o fun11 sec03 sub04:
   ^     - handel taxa merging cases
   ^   o fun11 sec03 sub05:
   ^     - merge taxa up (towards tip)
   ^   o fun11 sec03 sub06:
   ^     - handel not merging taxa cases
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - deal with unclassified ids + start loop
   \*****************************************************/

   while(
      fgets(
         (char *) buffStr,
         lenBuff_fun11_k2TaxaId,
         (FILE *) inFILE
       )
   ){ /*Loop: get read ids*/
      if(buffStr[0] == 'U')
      { /*If: unclassified read*/
         if(! pUnclassBl)
            continue; /*unclassified; ignoring*/

         while(*tmpStr < 33)
         { /*Loop: move to read id*/
            ++tmpStr;
            if(*tmpStr == '\0')
               goto fileErr_fun11_sec04;
         } /*Loop: move to read id*/

         tmpStr +=
            cpWhite_ulCp(
               idStr,
               tmpStr
            ); /*get the read id (no white space)*/

         fprintf(
            (FILE *) unclassFILE,
            "%s\tunclassified\n",
            idStr
         );
      } /*If: unclassified read*/

      /**************************************************\
      * Fun11 Sec03 Sub02:
      *   - get read id
      \**************************************************/

      tmpStr = buffStr;

      while(*tmpStr > 32)
         ++tmpStr;

      while(*tmpStr < 33)
      { /*Loop: move to read id*/
         ++tmpStr;
         if(*tmpStr == '\0')
            goto fileErr_fun11_sec04;
      } /*Loop: move to read id*/

      tmpStr +=
         cpWhite_ulCp(
            idStr,
            tmpStr
         ); /*get the read id (no white space)*/

      while(*tmpStr < 33)
      { /*Loop: move to taxa id*/
         ++tmpStr;
         if(*tmpStr == '\0')
            goto fileErr_fun11_sec04;
      } /*Loop: move to taxa id*/

      /**************************************************\
      * Fun11 Sec03 Sub03:
      *   - get taxa code and see if printing
      \**************************************************/

      tmpStr +=
         strToSL_base10str(
            tmpStr,
            &taxaSL
         ); /*get taxa code*/

      indexSL =
         findCode_taxa_k2TaxaId(
            taxaSL,
            taxaSTPtr
         );

      if(indexSL < 0)
         continue; /*not printing this taxa*/

      /**************************************************\
      * Fun11 Sec03 Sub04:
      *   - merge taxa down (towards root)
      *   o fun11 sec03 sub04 cat01:
      *     - find taxa to merge into
      *   o fun11 sec03 sub04 cat02:
      *     - print taxa id to file
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun11 Sec03 Sub04 Cat01:
      +   - find taxa to merge into
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
            taxaSTPtr->mergeAryBl[indexSL]
         == def_skip_k2TaxaId
      ) continue; /*not merging*/

      else if(
            taxaSTPtr->mergeAryBl[indexSL]
         == def_mergeDownBl_k2TaxaId
      ){ /*Else If: merging tip into deeper taxa*/
         levelSS = taxaSTPtr->levelArySS[indexSL];

         while(
               taxaSTPtr->backTrackArySL[indexSL] >= 0
            && 
                  taxaSTPtr->mergeAryBl[indexSL]
               == def_mergeDownBl_k2TaxaId
         ){ /*Loop: find taxa to merge into*/
           if(levelSS > taxaSTPtr->levelArySS[indexSL])
              break; /*no longer on same taxa group*/

           levelSS = taxaSTPtr->levelArySS[indexSL];
           --indexSL;
         } /*Loop: find taxa to merge into*/

         if(taxaSTPtr->backTrackArySL[indexSL] < 0)
            continue;
         if(taxaSTPtr->mergeAryBl[indexSL] != 0)
            continue;
         if(levelSS <= taxaSTPtr->levelArySS[indexSL])
            continue;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun11 Sec03 Sub04 Cat02:
         +   - print taxa id to file
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         tmpStr = buffStr;
  
         tmpStr +=
            cpLine_ulCp(
               tmpStr,
               prefixStr
            ); /*copy file name*/
  
         *tmpStr++ = '-';
  
         tmpStr +=
            numToStr(
              tmpStr,
              taxaSTPtr->codeArySL[indexSL]
            ); /*copy taxa code*/
            
         *tmpStr++ = '-';
  
         tmpStr +=
            cpLine_ulCp(
               tmpStr,
               get_strAry(
                  taxaSTPtr->idAryStr,
                  (ulong) indexSL
               )
            ); /*copy taxa name*/
  
         *tmpStr++ = '.';
         *tmpStr++ = 'i';
         *tmpStr++ = 'd';
         *tmpStr++ = 's';
         *tmpStr = '\0';
  
         outFILE =
            fopen(
               (char *) buffStr,
               "a"
            );
  
         if(! outFILE)
            goto fileErr_fun11_sec04;
  
         fprintf(
            (FILE *) outFILE,
            "%s\t%li\n",
            idStr,
            taxaSL
         );
  
         fclose(outFILE);
            outFILE = 0;
      } /*Else If: merging tip into deeper taxa*/

      /**************************************************\
      * Fun11 Sec03 Sub05:
      *   - merge taxa up (towards tip)
      *   o fun11 sec03 sub05 cat01:
      *     - set up for loop + start loop
      *   o fun11 sec03 sub05 cat02:
      *     - check if printing id to a taxa
      *   o fun11 sec03 sub05 cat03:
      *     - passed checks; print id
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun11 Sec03 Sub05 Cat01:
      +   - set up for loop + start loop
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
            taxaSTPtr->mergeAryBl[indexSL]
         == def_mergeUpBl_k2TaxaId
      ){ /*Else If: merging into a tip or higher taxa*/
         levelSS = taxaSTPtr->levelArySS[indexSL];

         histArySL[0] = indexSL;
         ++indexSL; /*move off current taxa*/

         histPosSI = 0;

         while(histPosSI >= 0)
         { /*Loop: find ids merging*/

            /*+++++++++++++++++++++++++++++++++++++++++++\
            + Fun11 Sec03 Sub05 Cat02:
            +   - check if printing id to a taxa
            \+++++++++++++++++++++++++++++++++++++++++++*/

            if(indexSL >= (slong) taxaSTPtr->numTaxaSL)
               break; /*no more taxa in list*/

            if(levelSS >= taxaSTPtr->levelArySS[indexSL])
               break; /*at same level (end of merge)*/


            if(taxaSTPtr->mergeAryBl[indexSL] != 0)
            { /*If: another merge level*/
               ++histPosSI;

               histArySL[histPosSI] = 

               taxaSTPtr->backTrackArySL[indexSL];
               ++indexSL; /*move up to next level*/
               continue;
            } /*If: another merge level*/


            if(
                  histArySL[histPosSI]
               != taxaSTPtr->backTrackArySL[indexSL]
            ){ /*If: need to backtrack*/
               backSL=taxaSTPtr->backTrackArySL[indexSL];

               if(backSL < 0)
                  break; /*no backtracking here*/

               while(
                       taxaSTPtr->backTrackArySL[backSL]
                    != histArySL[histPosSI]
               ){ /*If: not merging ids, check 1 id up*/
                  --histPosSI;

                  if(histPosSI < 0)
                     break;
               } /*If: not merging ids, check 1 id up*/

               if(histPosSI < 0)
                  break; /*finished*/
            } /*If: need to backtrack*/

            ++histPosSI;

            histArySL[histPosSI] = 
               taxaSTPtr->backTrackArySL[indexSL];

           /*++++++++++++++++++++++++++++++++++++++++++++\
           + Fun11 Sec03 Sub05 Cat03:
           +   - passed checks; print id
           \++++++++++++++++++++++++++++++++++++++++++++*/
  
           tmpStr = buffStr;
  
           tmpStr +=
              cpLine_ulCp(
                 tmpStr,
                 prefixStr
              ); /*copy file name*/
  
           *tmpStr++ = '-';
  
           tmpStr +=
              numToStr(
                tmpStr,
                taxaSTPtr->codeArySL[indexSL]
              ); /*copy taxa code*/
              
           *tmpStr++ = '-';
  
           tmpStr +=
              cpLine_ulCp(
                 tmpStr,
                 get_strAry(
                    taxaSTPtr->idAryStr,
                    (ulong) indexSL
                 )
              ); /*copy taxa name*/
  
           *tmpStr++ = '.';
           *tmpStr++ = 'i';
           *tmpStr++ = 'd';
           *tmpStr++ = 's';
           *tmpStr = '\0';
  
           outFILE =
              fopen(
                 (char *) buffStr,
                 "a"
              );
  
           if(! outFILE)
              goto fileErr_fun11_sec04;
  
           fprintf(
              (FILE *) outFILE,
              "%s\t%li\n",
              idStr,
              taxaSL
           );
  
           fclose(outFILE);
           outFILE = 0;
  
           ++indexSL;
        } /*Loop: while merging taxa*/
      } /*Else If: merging into a tip or higher taxa*/

      /**************************************************\
      * Fun11 Sec03 Sub06:
      *   - handel not merging taxa cases
      \**************************************************/

      else
      { /*Else: not merging*/
         tmpStr = buffStr;

         tmpStr +=
            cpLine_ulCp(
               tmpStr,
               prefixStr
            ); /*copy file name*/

         *tmpStr++ = '-';

         tmpStr +=
            numToStr(
              tmpStr,
              taxaSTPtr->codeArySL[indexSL]
            ); /*copy taxa code*/
            
         *tmpStr++ = '-';

         tmpStr +=
            cpLine_ulCp(
               tmpStr,
               get_strAry(
                  taxaSTPtr->idAryStr,
                  (ulong) indexSL
               )
            ); /*copy taxa name*/

         *tmpStr++ = '.';
         *tmpStr++ = 'i';
         *tmpStr++ = 'd';
         *tmpStr++ = 's';
         *tmpStr = '\0';

         outFILE =
            fopen(
               (char *) buffStr,
               "a"
            );

         if(! outFILE)
            goto fileErr_fun11_sec04;

         fprintf(
            (FILE *) outFILE,
            "%s\t%li\n",
            idStr,
            taxaSL
         );

         fclose(outFILE);
         outFILE = 0;
      } /*Else: not merging*/
   } /*Loop: get read ids*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_fun11_sec04;

   fileErr_fun11_sec04:;
      errSC = def_fileErr_k2TaxaId;
      goto cleanUp_fun11_sec04;

   cleanUp_fun11_sec04:;
      if(unclassFILE)
         fclose(unclassFILE);
      unclassFILE = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return errSC;
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
