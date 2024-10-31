/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' krakTaxaId SOF: Start Of File
'   - has functions and structs to get read ids by kraken
'     organisms
'   o st01: taxa_krakTaxaId
'     - has taxa names and ids to extract by
'   o fun01: blank_taxa_krakTaxaId
'     - blanks a taxa_krakTaxaId structure
'   o fun02: init_taxa_krakTaxaId
'     - initializes a taxa_krakTaxaId structure
'   o fun04: freeHeap_taxa_krakTaxaId
'     - frees a taxa_krakTaxaId structure
'   o fun05: setup_taxa_krakTaxaId
'     - allocates memory for a taxa_krakTaxaId struc
'   o fun06: realloc_taxa_krakTaxaId
'     - reallocates memory for a taxa_krakTaxaId struc
'   o fun07: sortCodes_taxa_krakTaxaId
'     - sorts the sortCodeArySL array while keeping the
'       indexArySL in sync a taxa_krakTaxaId
'   o fun08: findCode_taxa_krakTaxaId
'     - returns index of taxa code, searches sorted list
'       and returns unsorted list index
'   o fun09: getLevel_krakTaxaId
'     - get the level in tree of a kraken entry; col 4
'   o fun10: readReport_krakTaxaId
'     - gets list of organism codes
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "../genLib/strAry.h"
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
| Fun01: blank_taxa_krakTaxaId
|   - blanks a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId struct to blank
| Output:
|   - Modifies:
|     o numTaxaUL in taxaSTPtr to be 0
\-------------------------------------------------------*/
void
blank_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
){
   taxaSTPtr->numTaxaUL = 0;
} /*blank_taxa_krakTaxaId*/

/*-------------------------------------------------------\
| Fun02: init_taxa_krakTaxaId
|   - initializes a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to initiazlize
| Output:
|   - Modifies:
|     o sets everything in taxaSTPtr to 0
\-------------------------------------------------------*/
void
init_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
){
   taxaSTPtr->codeArySL = 0;
   taxaSTPtr->nameAryStr = 0;
   taxaSTPtr->mergeArySL = 0;
   taxaSTPtr->levelAryUS = 0;
   taxaSTPtr->sortCodeArySL = 0;
   taxaSTPtr->sizeTaxaUL = 0;

   blank_taxa_krakTaxaId(taxaSTPtr);
} /*init_taxa_krakTaxaId*/

/*-------------------------------------------------------\
| Fun03: freeStack_taxa_krakTaxaId
|   - frees variables in a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId with vars to free
| Output:
|   - Frees:
|     o all variables in taxaSTPtr (sets vars to 0)
\-------------------------------------------------------*/
void
freeStack_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
){
   if(! taxaSTPtr)
      return;

   if(taxaSTPtr->codeArySL)
     free(taxaSTPtr->codeArySL);

   if(taxaSTPtr->nameAryStr)
     free(taxaSTPtr->nameAryStr);

   if(taxaSTPtr->mergeArySL)
     free(taxaSTPtr->mergeArySL);

   if(taxaSTPtr->levelAryUS)
     free(taxaSTPtr->levelAryUS);

   if(taxaSTPtr->sortCodeArySL)
     free(taxaSTPtr->sortCodeArySL);

   if(taxaSTPtr->indexArySL)
     free(taxaSTPtr->indexArySL);

   init_taxa_krakTaxaId(taxaSTPtr);
} /*freeStack_taxa_krakTaxaId*/

/*-------------------------------------------------------\
| Fun04: freeHeap_taxa_krakTaxaId
|   - frees a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to free
| Output:
|   - Frees:
|     o taxaSTPtr (you must set to null (0))
\-------------------------------------------------------*/
void
freeHeap_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
){
   if(! taxaSTPtr)
      return;

   freeStack_taxa_krakTaxaId(taxaSTPtr);
   free(taxaSTPtr);
} /*freeHeap_taxa_krakTaxaId*/

/*-------------------------------------------------------\
| Fun05: setup_taxa_krakTaxaId
|   - allocates memory for a taxa_krakTaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to setup memory
|   - numElmUL:
|     o number of elements to allocate memory for
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_krakTaxaId for memory errors
\-------------------------------------------------------*/
signed char
setup_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr,
   unsigned long numElmUL
){
   /*make sure all memory is freeded first*/
   freeStack_taxa_krakTaxaId(taxaSTPtr);

   taxaSTPtr->codeArySL =
      calloc(
         numElmUL,
         sizeof(slong)
      );

   if(! taxaSTPtr->codeArySL)
      goto memErr_fun05;


   taxaSTPtr->nameAryStr = mk_strAry(numElmUL);

   if(! taxaSTPtr->nameAryStr)
      goto memErr_fun05;


   taxaSTPtr->mergeArySL =
      calloc(
         numElmUL,
         sizeof(slong)
      );

   if(! taxaSTPtr->mergeArySL)
      goto memErr_fun05;


   taxaSTPtr->levelAryUS =
      calloc(
         numElmUL,
         sizeof(ushort)
      );

   if(! taxaSTPtr->levelAryUS)
      goto memErr_fun05;


   taxaSTPtr->sortCodeArySL =
      calloc(
         numElmUL,
         sizeof(ushort)
      );

   if(! taxaSTPtr->sortCodeArySL)
      goto memErr_fun05;


   taxaSTPtr->idnexArySL =
      calloc(
         numElmUL,
         sizeof(ushort)
      );

   if(! taxaSTPtr->idnexArySL)
      goto memErr_fun05;


   taxaSTPtr = numElmUL;
   blank_taxa_krakTaxaId(taxaSTPtr);

   return 0;

   memErr_fun05:;
      freeStack_taxa_krakTaxaId(taxaSTPtr);
      return def_memErr_krakTaxaId;
} /*setup_taxa_krakTaxaId*/


/*-------------------------------------------------------\
| Fun06: realloc_taxa_krakTaxaId
|   - reallocates memory for a taxa_krakTaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to resize
|   - numElmUL:
|     o number of elements to resize to
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to be realloced
|   - Returns:
|     o 0 for no errors
|     o def_memErr_krakTaxaId for memory errors
\-------------------------------------------------------*/
signed char
realloc_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr,
   unsigned long numElmUL
){
   slong *slPtr = 0;


   if(numElmUL == taxaSTPtr->numTaxaUL)
      return 0; /*nothing to do*/

   slPtr =
      realloc(
         taxaSTPtr->codeArySL,
         numElmUL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->codeArySL = slPtr;


    slPtr =
      (slong *)
      realloc_strAry_strAry(
         taxaSTPtr->nameAryStr,
         numElmUL
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->nameAryStr = (schar *) slPtr;


   slPtr =
      realloc(
         taxaSTPtr->mergeArySL,
         numElmUL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->mergeArySL = slPtr;


   slPtr =
      (slong *)
      realloc(
         taxaSTPtr->levelAryUS,
         numElmUL * sizeof(ushort)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->levelAryUS = (ushort *) slPtr;


   slPtr =
      realloc(
         taxaSTPtr->sortCodeArySL,
         numElmUL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->sortCodeArySL = slPtr;


   slPtr =
      realloc(
         taxaSTPtr->indexArySL,
         numElmUL * sizeof(slong)
      );

   if(! slPtr)
      goto memErr_fun05;

   taxaSTPtr->indexArySL = slPtr;


   taxaSTPtr = numElmUL;

   return 0;

   memErr_fun05:;
      return def_memErr_krakTaxaId;
} /*realloc_taxa_krakTaxaI*/

/*-------------------------------------------------------\
| Fun07: sortCodes_taxa_krakTaxaId
|   - sorts the sortCodeArySL array while keeping the
|     indexArySL in sync a taxa_krakTaxaId
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to sort
| Output:
|   - Modifies:
|     o sortCodeArySL to be sorted lowest to greatest
|     o indexArySL to be in sync with sortCodeArySL
|       - idea: index is index to unsorted arrays
\-------------------------------------------------------*/
void
sortCodes_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
){
   /*Number of elements to sort (index 1)*/
   unsigned long numElmUL = taxaSTPtr->numTaxaUL;

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

   if(! numElmUL)
      return; /*nothing to sort*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
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
                 taxaSTPtr->sortCodeArySL[ulElm]
               > taxaSTPtr->sortCodeArySL[nextUL]
            { /*If I need to swap an element*/
               taxaSTPtr->sortCodeArySL[ulElm] ^=
                  taxaSTPtr->sortCodeArySL[nextUL];
               taxaSTPtr->sortCodeArySL[nextUL] ^=
                  taxaSTPtr->sortCodeArySL[ulElm];
               taxaSTPtr->sortCodeArySL[ulElm] ^=
                  taxaSTPtr->sortCodeArySL[nextUL];

               taxaSTPtr->indexArySL[ulElm] ^=
                  taxaSTPtr->indexArySL[nextUL];
               taxaSTPtr->indexArySL[nextUL] ^=
                  taxaSTPtr->indexArySL[ulElm];
               taxaSTPtr->indexArySL[ulElm] ^=
                  taxaSTPtr->indexArySL[nextUL];

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  if(
                      taxaSTPtr->sortCodeArySL[onUL]
                    > taxaSTPtr->sortCodeArySL[lastUL]
                  ) break; /*position element*/

                  taxaSTPtr->sortCodeArySL[onUL] ^=
                     taxaSTPtr->sortCodeArySL[lastUL];
                  taxaSTPtr->sortCodeArySL[lastUL] ^=
                     taxaSTPtr->sortCodeArySL[onUL];
                  taxaSTPtr->sortCodeArySL[onUL] ^=
                     taxaSTPtr->sortCodeArySL[lastUL];

                  taxaSTPtr->indexArySL[onUL] ^=
                     taxaSTPtr->indexArySL[lastUL];
                  taxaSTPtr->indexArySL[lastUL] ^=
                     taxaSTPtr->indexArySL[onUL];
                  taxaSTPtr->indexArySL[onUL] ^=
                     taxaSTPtr->indexArySL[lastUL];

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortCodes_taxa_krakTaxaId*/

/*-------------------------------------------------------\
| Fun08: findCode_taxa_krakTaxaId
|  - returns index of taxa code, searches sorted list
|    and returns unsorted list index
| Input:
|   - codeSL:
|     o taxa code to search for
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to search for code
| Output:
|  - Returns:
|    o index of taxa code
|    o -1 if taxa code not in taxaSTPtr
\-------------------------------------------------------*/
signed long
findCode_taxa_krakTaxaId(
   signed long codeSL,
   struct *taxa_krakTaxaId taxaSTPtr
){
   signed long midSL = 0;

   signed long rightSL =
      (slong) taxaSTPtr->numTaxaUL - 1;

   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
      midSL = (leftSL + rightSL) >> 1;

     if(codeSL > taxaSTPtr->sortCodeArySL[midSL])
         leftSL = midSL + 1;

     else if(codeSL < taxaSTPtr->sortCodeArySL[midSL])
         rightSL = midSL - 1;

     else
        return taxaSTPtr->indexArySL[midSL];
   } /*Loop: Search for the querys index*/

   return -1; /*taxa code not found*/
} /*ulSearch_shellSort*/

/*-------------------------------------------------------\
| Fun09: getLevel_krakTaxaId
|   - get the level in heirchyy of a kraken entry (col 4)
| Input:
|   - taxaStr:
|     o string with taxa assignment to get number for
| Output:
|   - Returns:
|     - taxa numuber + sub-taxa level (ex: for D1; D + 1)
|       o unclassified def_unclassifed_krakTaxaId; no level
|       o root def_root_krakTaxaId + level
|       o domain def_domain_krakTaxaId + level
|       o phylum def_phylum_krakTaxaId + level
|       o class def_class_krakTaxaId + level
|       o order def_order_krakTaxaId + level
|       o family def_family_krakTaxaId + level
|       o genus def_genus_krakTaxaId + level
|       o species def_species_krakTaxaId + level
\-------------------------------------------------------*/
unsigned short
getLevel_krakTaxaId(
   signed char *taxaStr
){
   switch(*taxaStr & ~32)
   { /*Switch: find taxa level*/
      case ' ':  /*empty case*/
      case '\t': /*empty case*/
      case 'U':
         return def_unclassifed_krakTaxaId;

      case 'R':
         if(tmpStr[1] > 47)
            return def_root_krakTaxaId + tmpStr[1] - 48;
          else
            return def_root_krakTaxaId + tmpStr[1];

      case 'D':
         if(tmpStr[1] > 47)
            return def_domain_krakTaxaId + tmpStr[1] - 48;
          else
            return def_domain_krakTaxaId + tmpStr[1];

      case 'P':
         if(tmpStr[1] > 47)
            return def_phylum_krakTaxaId + tmpStr[1] - 48;
          else
            return def_phylum_krakTaxaId + tmpStr[1];

      case 'C':
         if(tmpStr[1] > 47)
            return def_class_krakTaxaId + tmpStr[1] - 48;
          else
            return def_class_krakTaxaId + tmpStr[1];

      case 'O':
         if(tmpStr[1] > 47)
            return def_order_krakTaxaId + tmpStr[1] - 48;
          else
            return def_order_krakTaxaId + tmpStr[1];

      case 'F':
         if(tmpStr[1] > 47)
            return def_family_krakTaxaId + tmpStr[1] - 48;
          else
            return def_family_krakTaxaId + tmpStr[1];

      case 'G':
         if(tmpStr[1] > 47)
            return def_genus_krakTaxaId + tmpStr[1] - 48;
          else
            return def_genus_krakTaxaId + tmpStr[1];

      case 'S':
         if(tmpStr[1] > 47)
            return def_species_krakTaxaId + tmpStr[1] -48;
          else
            return def_species_krakTaxaId + tmpStr[1];
   } /*Switch: find taxa level*/
} /*getLevel_krakTaxaId*/

/*-------------------------------------------------------\
| Fun10: readReport_krakTaxaId
|   - gets codes of organisims for read ids print otu
| Input:
|   - startLevUS:
|     o lowest level (nearest root) to start printing for
|     o use getLevel_krakTaxaId to get this
|   - endLevUS:
|     o highest level (nearest tip) to start printing for
|     o use getLevel_krakTaxaId to get this
|   - minDepthUL:
|     o min read depth to keep an id
|   - pStrictBl:
|     o 1: only keep ids mapping to its level
|     o 0: merge lower tree (root) levels with their
|          upper (tip) levels
|   - errSCPtr:
|     o pointer to signed char to have errors
|   - inFILE:
|     o FILE pointer to kraken2 tsv report with taxons
|       to extract
| Output:
|   - Modifies:
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_krakId for memory errors
|       - def_fileErr_krakId for file errors
|       - def_noIds_krakId if no id's in file
|   - Returns:
|     o taxa_krakId with list of taxon ids and names
|       to extract
|       - negative numbers in codeArySL denote merged
|         tips (! pScrictBl) or for pScrictBl, taxa to
|         ignore
|     o 0 for no ids or error
\-------------------------------------------------------*/
struct taxa_krakTaxaId *
readReport_krakTaxaId(
   unsigned short startLevUS,   /*lowest level to print*/
   unsigned short endLevUS,     /*highest level to print*/
   unsigned long minDepthUL,    /*min depth to keep id*/
   signed char pStrictBl,       /*1: do not merge levels*/
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

   struct taxa_krakTaxaId *retTaxaHeapST;

   ulong histUL[def_maxDepth_krakTaxaId];
   ulong histLevUS[def_maxDepth_krakTaxaId];
      /*keeps track of were at in tree*/

   ulong readDepthUL = 0;

   uint depthUI = 0;     /*current depth in tree*/
   uint tmpUI = 1;
   uint posUI = 1;
   ushort levelUS = 0;  /*depth of current entry in tree*/

   ulong *idAryUL = 0;  /*number of ids to print*/
   ulong sizeIdUL = 0;  /*max ids can hold*/
   #define def_lenId_fun10 1028
      /*how much to exand id array by each time*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
   ^   - allocate memory for arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *lenIdAryULPtr = 0;
   idAryUL = malloc(def_lenId_fun10 * sizeof(ulong));

   if(! idAryUL)
      goto memErr_fun10_sec04;


   if(*idAryStr)
      free(*idAryStr);
   *idAryStr = 0;

   *idAryStr = mk_strAry(def_lenId_fun10);

   if(! *idAryStr)
      goto memErr_fun10_sec04;


   if(! pStrictBl)
   { /*If: merging read ids across levels*/
      if(*mergeArySLPtr)
         free(*mergeArySLPtr);
      *mergeArySLPtr = 0;

      *mergeArySLPtr =
         malloc(def_lenId_fun10 * sizeof(ulong));
   
      if(! *mergeArySLPtr)
         goto memErr_fun10_sec04;
   } /*If: merging read ids across levels*/
   

   sizeIdUL = def_lenId_fun10;

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

   while(
      fgets(
         buffStr,
         def_lenBuff_fun10,
         (FILE *) inFILE
      )
   ){ /*Loop: process file*/
      tmpStr = buffStr;

      while(*tmpStr++ < 33)
      { /*Loop: get past white space*/
         if(*tmpStr == '/0')
            goto fileErr_fun10_sec04;
      } /*Loop: get past white space*/


      /*get past percentage*/
      while(*tmpStr++ > 32) ;

      while(*tmpStr++ < 33)
      { /*Loop: move to number reads*/
         if(*tmpStr == '/0')
            goto fileErr_fun10_sec04;
      } /*Loop: move to number reads*/

      /**************************************************\
      * Fun10 Sec03 Sub02:
      *   - move past total reads count entry
      \**************************************************/

      /*get past number of reads*/
      while(*tmpStr++ > 32) ;

      while(*tmpStr++ < 33)
      { /*Loop: move to number unique reads*/
         if(*tmpStr == '/0')
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

      if(*tmpStr == '/0')
         goto fileErr_fun10_sec04;

      if(readDepthUL < minDepthUL)
         continue; /*not enough read depth*/

      ++tmpStr;

      if(*tmpStr == '/0')
         goto fileErr_fun10_sec04;

      if(*tmpStr < 32)
         continue; /*unkown taxanomic level*/

      /**************************************************\
      * Fun10 Sec03 Sub04:
      *   - get level of organism in tree & move to name
      \**************************************************/

      levelUS = getLevel_krakTaxaId(tmpStr);

      /*get past level entry*/
      while(*tmpStr++ > 32) ;

      while(*tmpStr++ < 33)
      { /*Loop: move orgainism id*/
         if(*tmpStr == '/0')
            goto fileErr_fun10_sec04;
      } /*Loop: move orgainism id*/

      /**************************************************\
      * Fun10 Sec03 Sub05:
      *   - find last level or organism in tree
      \**************************************************/

      while(levelUS <= histLevUS[depthUI])
      { /*Loop: find last level for this organism*/
         --depthUI;

         if(! depthUI)
            break; /*at last level*/
      } /*Loop: find last level for this organism*/

      histLevUS[depthUI + 1] = levelUS;

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
      *     - merge taxa levels if merging (no pStrictBl)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun10 Sec03 Sub06 Cat01:
      +   - if check and resize arrays if needed
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
            levUS >= startLevUS
         && levUS <= endLevUS
      ){ /*If: at desired tree depth for print*/

         if(*lenIdAryULPtr >= sizeIdUL)
         { /*If: need to resize array*/
            sizeIdUL += def_lenId_fun10;

            if(
               realloc_taxa_krakTaxaId(
                  retTaxaHeapST,
                  sizeIdUL
               )
            ) goto memErr_fun10_sec04;
              /*return value means memory error*/
         } /*If: need to resize array*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat02:
         +   - copy organism code (id)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               &retTaxaHeapST->codeArySL[
                  retTaxaHeapST->numTaxaUL
               ]
            );

         if(*tmpStr > 32)
            goto fileErr_fun10_sec04;
            /*not white space*/

         /*array to sort ids in*/
         retTaxaHeapST->sortCodeArySL[
            retTaxaHeapST->numTaxaUL
         ] =
            retTaxaHeapST->codeArySL[
               retTaxaHeapST->numTaxaUL
            ];

         /*unsorted index*/
         retTaxaHeapST->indexArySL =
            retTaxaHeapST->numTaxaUL;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat03:
         +   - get organism name (also remove spaces)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(*tmpStr++ < 33)
         { /*Loop: move to level*/
            if(*tmpStr == '/0')
               goto fileErr_fun10_sec04;
         } /*Loop: move to level*/

         add_strAry(
            tmpStr,
            retTaxaHeapST->nameAryStr,
            retTaxaHeapST->numTaxaUL
         ); /*add orgainism name to id array*/

         tmpStr =
            get_strAry(
              retTaxaHeapST->namAryStr,
              retTaxaHeapST->numTaxaUL
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
         } /*Loop: remove spaces*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat04:
         +   - merge taxa levels if merging (no pStrictBl)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(! pStrictBl)
         { /*If: merging ids*/
            if(! depthUI)
               ++depthUI; /*if at root of tree*/

             retTaxaHeapST->mergeAryUL[
                retTaxaHeapST->numTaxaUL
             ] = histUL[depthUI];

             histUL[depthUI] = retTaxaHeapST->numTaxaUL;
               /*using postives to denote taxa to print*/

            tmpUI = depthUI - 1;
            posUI = retTaxaHeapST->numTaxaUL;

            while(tmpUI)
            { /*Loop: update past depths*/
               if(
                     retTaxaHeapST->mergeAryUL[posUI]
                  == histUL[tmpUI]
               ){ /*If: level not set to merge*/
                  retTaxaHeapST->mergeAryUL[posUI] *= -1;
                    /*using negatives to denote only print
                    `  with lower taxa
                    */

                  --tmpUI
               } /*If: level not set to merge*/

               if(! posUI)
                  break; /*should never happen*/

               --posUI;
            } /*Loop: update past depths*/

            ++retTaxaHeapST->numTaxaUL;
         } /*If: merging ids*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun10 Sec03 Sub06 Cat05:
         +    - move to next id (non-merbge
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(levelUS == endLevUS)
            ++retTaxaHeapST->numTaxaUL;
            /*not merging, so only want lowest level*/
     } /*If: at desired tree depth for print*/

     /*else to high (closer to tip of tree), ignore*/
   } /*Loop: process file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*sort sorted code array for quick look ups later*/
   sortCodes_taxa_krakTaxaId(retTaxaHeapST);

   *errSCPtr = 0;
   goto cleanErr_fun10_sec04;

   memErr_fun10_sec04:;
      *errSCPtr = def_memErr_krakTaxaId;
      goto cleanErr_fun10_sec04;

   fileErr_fun10_sec04:;
      *errSCPtr = def_fileErr_krakTaxaId;
      goto cleanErr_fun10_sec04;

   cleanErr_fun10_sec04:;
      if(retHeapST)
         freeHeap_inList_krakTaxaId(retHeapST);
      retHeapST = 0;

      goto ret_fun10_sec04;

   ret_fun10_sec04:;
      return retHeapST;
} /*readReport_krakTaxaId*/

/*-------------------------------------------------------\
| Fun11: pIds_krakTaxaId
|   - prints out read ids by taxa for kraken2
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId struct with taxa codes
|       to extract/merge
|   - prefixStr:
|     o c-string with prefix to name files with
|   - pStrictBl:
|     o 1: only keep ids mapping to its level
|     o 0: merge lower tree (root) levels with their
|          upper (tip) levels
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
|     o def_memErr_krakTaxaId for memory errors
|     o def_fileErr_krakTaxaId for file errors
\-------------------------------------------------------*/
signed char
pIds_krakTaxaId(
   struct taxa_krakTaxaId *taxaSTPtr,
   signed char *prefixStr,
   signed char pStrictBl,
   void *inFILE
){
   signed long taxaSL = 0;
   signed long indexSL = 0;
   signed long mergeSL = 0;

   #define idLen_fun11_krakTaxaId 128
   signed char idStr[idLen_fun11_krakTaxaId];

   #define lenBuff_fun11_krakTaxaId 1 << 15
   signed char buffStr[buffLen_fun11_krakTaxaId];

   while(
      fgets(
         buffStr,
         buffLen_fun11_krakTaxaId,
         (FILE *) inFILE
       )
   ){ /*Loop: get read ids*/
      if(buffStr[0] == 'U')
         continue; /*unclassified*/

      while(*tmpStr++ < 33)
      { /*Loop: move to read id*/
         if(*tmpStr == '/0')
            goto fileErr_fun10_sec04;
      } /*Loop: move to read id*/

      tmpStr +=
         cpDelim_ulCp(
            idStr,
            tmpStr,
            def_tab_ulCp,
            (schar) '\t'
         ); /*get the read id*/

      while(*tmpStr++ < 33)
      { /*Loop: move to taxa id*/
         if(*tmpStr == '/0')
            goto fileErr_fun10_sec04;
      } /*Loop: move to taxa id*/

      tmpStr +=
         strToSL_base10str(
            tmpStr,
            &taxaSL
         ); /*get taxa code*/

      indexSL =
         findCode_taxa_krakTaxaId(
            taxaSL,
            taxaSTPtr
         );

      if(indexSL < 0)
         continue; /*not printing this taxa*/

      if(
            ! pStrictBl
         && taxaSTPtr->mergeArySL[indexSL] < 0
      ){ /*If: merging this with a lower taxa*/
         mergeSL = 
      } /*If: merging this with a lower taxa*/

      else
      { /*Else: not merging*/
         tmpStr = buffStr;

         tmpStr +=
            cpDelim_ulCp(
               tmpStr,
               prefixStr,
               def_tab_ulCp,
               '\t'
            ); /*copy read

         *tmpStr++ += '-';

         tmpStr +=
            
      } /*Else: not merging*/
   } /*Loop: get read ids*/
} /*pIds_krakTaxaId*/
