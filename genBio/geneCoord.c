/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' geneCoord SOF: Start Of File
'   - holds the geneCoord struct and its supporting
'     functions I use to read in gene positions form a paf
'     file
'   o header:
'     - Includes and definitions
'   o .h st: geneCoord
'     - Holds the arrays for the gene coordinates and ids 
'       in gene table
'   o fun02: freeStack_geneCoord
'     - Frees the arrays in a geneCoord structure
'   o fun03: freeHeap_geneCoord
'     - Frees the a heap alloacted geneCoord structure
'   o fun01: init_geneCoord
'     - Initializes a geneCoord structure
'   o fun04: mk_geneCoord
'     - Makes a heap allocated geneCoord structure
'   o fun05: getPaf_geneCoord
'     - Get the id and coordinates for a gene from a paf
'       file
'   o .c fun06: swap_geneCoord
'     - Swaps two array items in a geneCoord structure
'       around
'   o fun07: sort_geneCoord
'     - Sorts the arrays in a genesCoord structure by
'       starting positiion with shell short.
'   o fun08: findStart_geneCoord
'     - Does a binary search by starting coordinate for a
'       potentail gene in a geneCoord structure
'   o fun10: sortName_geneCoord
'     - Sorts the arrays in a genesCoord structure by
'       gene name
'   o fun11: findName_geneCoord
'     - Does a binary search to find an gene name in an
'       gene geneCoord structer (must be sorted by name)
'   o fun12: getCoords_geneCoord
'     - Gets the gene coordinates from an gene coordinates
'       table
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "geneCoord.h"

#include <stdio.h>

#include "../genLib/fileFun.h"
#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"

/*.h files only*/
#include "../genLib/genMath.h" /*only max macro (in .h)*/

/*-------------------------------------------------------\
| Fun01: init_geneCoord
|    - Initializes a geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure to initialize
| Output:
|   - Sets:
|     o idStrAry, startAryUI, and endAryUI to 0
\-------------------------------------------------------*/
void
init_geneCoord(
   struct geneCoord *geneCoordST
){
   geneCoordST->idStrAry = 0;
   geneCoordST->refAryStr = 0;

   geneCoordST->startAryUI = 0;
   geneCoordST->endAryUI = 0;

   geneCoordST->dirAryUC = 0;
} /*init_geneCoord*/

/*-------------------------------------------------------\
| Fun02: freeStack_geneCoord
|    - Frees the arrays in a geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure with arrays to
|       free
| Output:
|   - Frees:
|     o idStrAry, startAryUI, and endAryUI (and sets to 0)
\-------------------------------------------------------*/
void
freeStack_geneCoord(
   struct geneCoord *geneCoordST
){
   if(! geneCoordST)
      return;

   if(geneCoordST->idStrAry)
      free(geneCoordST->idStrAry);

   if(geneCoordST->refAryStr)
      free(geneCoordST->refAryStr);
   
   if(geneCoordST->startAryUI)
      free(geneCoordST->startAryUI);

   if(geneCoordST->endAryUI)
      free(geneCoordST->endAryUI);

   if(geneCoordST->dirAryUC)
      free(geneCoordST->dirAryUC);

   init_geneCoord(geneCoordST);
} /*freeStack_geneCoord*/

/*-------------------------------------------------------\
| Fun03: freeHeap_geneCoord
|    - Frees the a heap alloacted geneCoord structure
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoord structure to free
| Output:
|   - Frees:
|     o geneCoordST
\-------------------------------------------------------*/
void
freeHeap_geneCoord(
   struct geneCoord *geneCoordST
){
   if(! geneCoordST)
      return;

   freeStack_geneCoord(geneCoordST);
   free(geneCoordST);
} /*freeHeap_geneCoord*/

/*-------------------------------------------------------\
| Fun04: mk_geneCoord
|    - Makes a heap allocated geneCoord structure
| Input:
|   - numGenesUI:
|     o Length of the arrays in the new geneCoord
|       structure
| Output:
|   - Returns:
|     o geneCoord structure with allocated arrays
| Note:
|   - Do not use init_geneCoord on the returned structure
\-------------------------------------------------------*/
struct geneCoord *
mk_geneCoord(
   unsigned int numGenesUI
){
   struct geneCoord *retST = 0;

   retST = malloc(sizeof(struct geneCoord));
   
   if(! retST)
      goto memErr_fun04;

   init_geneCoord(retST);
   
   retST->idStrAry =
      calloc((numGenesUI), sizeof(*retST->idStrAry));
   
   if(! retST->idStrAry)
      goto memErr_fun04;

   retST->refAryStr =
      calloc((numGenesUI), sizeof(*retST->refAryStr));
   
   if(! retST->refAryStr)
      goto memErr_fun04;

   retST->startAryUI=
       malloc((numGenesUI) * sizeof(unsigned int));
   
   if(! retST->startAryUI)
      goto memErr_fun04;
   
   retST->endAryUI =
       malloc((numGenesUI) * sizeof(unsigned int));
   
   if(! retST->endAryUI)
      goto memErr_fun04;

   retST->dirAryUC =
       malloc((numGenesUI) * sizeof(unsigned char));
   
   if(! retST->dirAryUC)
      goto memErr_fun04;

   return retST;

   memErr_fun04:;

   freeHeap_geneCoord(retST);
   retST = 0;
   return 0;
} /*mk_geneCoord*/

/*-------------------------------------------------------\
| Fun05: getPaf_geneCoord
|    - Get the id and coordinates for a gene from a paf
|      file
| Input:
|   - geneCoordST:
|     o Pointer to a geneCoordST structure to hold the
|       new gene
|   - posUI:
|     o Position in the arrays in the geneCoord structure
|       to add the new gene at
|   - typeC:
|     o Alignment type for the extracted gene
|   - pafLineStr:
|     o Line from the paf file with the gene to add
| Output:
|   - Modifies:
|     o All arrays in geneCoordST to hold the new gene
\-------------------------------------------------------*/
void
getPaf_geneCoord(
   struct geneCoord *geneCoordST,
   unsigned int posUI,
   signed char *typeC,
   signed char *pafLineStr
){ /*getPagGene*/
   unsigned char ucEntry = 0;
   unsigned int uiChar = 0;
   signed char *tmpStr = 0;
   
   for(uiChar = 0; (pafLineStr)[uiChar] > 32; ++uiChar)
   { /*Loop: copy id*/
      geneCoordST->idStrAry[posUI][uiChar] =
         pafLineStr[uiChar];
   } /*Loop: copy id*/
   
   geneCoordST->idStrAry[(posUI)][uiChar] = '\0';

   ++uiChar;
   
   for(ucEntry = 1; ucEntry < 7; ++ucEntry)
   { /*Loop: Move to the reference start position*/
      /*Move off the tab*/
      while((pafLineStr)[uiChar++] > 32) ;
   } /*Loop: Move to the reference start position*/
   
   tmpStr +=
      strToUI_base10str(
         &(pafLineStr)[uiChar],
         &geneCoordST->startAryUI[posUI]
      ); /*Get the starting position*/
   
   ++tmpStr; /*Move off the tab*/
   ++ucEntry;
   
   tmpStr +=
      strToUI_base10str(
         tmpStr,
         &geneCoordST->endAryUI[posUI]
      ); /*Get the ending position*/
   
   ++tmpStr; /*Move off the tab*/
   ++ucEntry;
   
   /*Get the alignment type (P for primary)*/
   uiChar = 0;
   while(tmpStr[uiChar] != 't'
      && tmpStr[uiChar + 1] != 'p'
      && tmpStr[uiChar + 2] != ':'
   ) ++uiChar;

   *(typeC) = tmpStr[uiChar + 5];
} /*getPaf_geneCoord*/

/*-------------------------------------------------------\
| Fun06: swap_geneCoord
|  - Swaps two array items in a geneCoord structure
|    around
| Input:
|  - geneCoordST:
|    o Pointer to the geneCoord structure to swap elements
|      in
|  - posOne:
|    o The position (index) of the first gene to swap
|  - posTwo:
|    o The position (index) of the second gene to swap
| Output:
|  - Modifies:
|    o Swaps the start, end, and gene id around
\-------------------------------------------------------*/
void
swap_geneCoord(
   struct geneCoord *coordST,
   unsigned long oneUL,
   unsigned long secUL
){
  unsigned char ucSwap = 0;

  coordST->startAryUI[oneUL]^= coordST->startAryUI[secUL];
  coordST->startAryUI[secUL]^= coordST->startAryUI[oneUL];
  coordST->startAryUI[oneUL]^= coordST->startAryUI[secUL];

  coordST->endAryUI[oneUL] ^= coordST->endAryUI[secUL];
  coordST->endAryUI[secUL] ^= coordST->endAryUI[oneUL];
  coordST->endAryUI[oneUL] ^= coordST->endAryUI[secUL];

  coordST->dirAryUC[oneUL] ^= coordST->dirAryUC[secUL];
  coordST->dirAryUC[secUL] ^= coordST->dirAryUC[oneUL];
  coordST->dirAryUC[oneUL] ^= coordST->dirAryUC[secUL];

  while(
        coordST->idStrAry[oneUL][ucSwap] != '\0'
     || coordST->idStrAry[secUL][ucSwap] != '\0'
  ){ /*Loop: copy the gene name*/
     coordST->idStrAry[oneUL][ucSwap] ^=
        coordST->idStrAry[secUL][ucSwap];

     coordST->idStrAry[secUL][ucSwap] ^=
        coordST->idStrAry[oneUL][ucSwap];

     coordST->idStrAry[oneUL][ucSwap] ^=
        coordST->idStrAry[secUL][ucSwap];

     ++ucSwap;
  } /*Loop: copy the gene name*/

  coordST->idStrAry[oneUL][ucSwap] = '\0';
  coordST->idStrAry[secUL][ucSwap] = '\0';

  while(
        coordST->refAryStr[oneUL][ucSwap] != '\0'
     || coordST->refAryStr[secUL][ucSwap] != '\0'
  ){ /*Loop: copy the gene name*/
     coordST->refAryStr[oneUL][ucSwap] ^=
        coordST->refAryStr[secUL][ucSwap];

     coordST->refAryStr[secUL][ucSwap] ^=
        coordST->refAryStr[oneUL][ucSwap];

     coordST->refAryStr[oneUL][ucSwap] ^=
        coordST->refAryStr[secUL][ucSwap];

     ++ucSwap;
  } /*Loop: copy the gene name*/

  coordST->refAryStr[oneUL][ucSwap] = '\0';
  coordST->refAryStr[secUL][ucSwap] = '\0';
} /*swapScoreSTs*/

/*-------------------------------------------------------\
| Fun07: sort_geneCoord
|  - Sorts the arrays in a genesCoord structure by
|    starting positiion with shell short.
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
|  - startUI:
|    o First element to start sorting at
|  - endUI:
|    o Last element to sort
| Output:
|  - Modifies:
|    o Arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
\-------------------------------------------------------*/
void
sort_geneCoord(
   struct geneCoord *geneCoordST,
   unsigned int startUI,
   unsigned int endUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun07 TOC: sort_geneCoord
   '  - Sorts the arrays in a geneCoord struct by
   '    starting position with shell short.
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun07 sec01:
   '    - Variable declerations
   '  o fun07 sec02:
   '    - Find the number of rounds to sort for
   '  o fun07 sec03:
   '    - Sort the arrays in geneCoordST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  unsigned long numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextElmUL = 0;
  unsigned long lastElmUL = 0;
  unsigned long elmOnUL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun07 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun07 Sec03:
  ^  - Sort the arrays in geneCoordST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(ulIndex = 0; ulIndex <= subUL; ++ulIndex)
    { /*For each element in the subarray*/
      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop; swap each nth element of the subarray*/
        nextElmUL = ulElm + subUL;
        
        if(   (geneCoordST)->startAryUI[ulElm]
            > (geneCoordST)->startAryUI[nextElmUL]
        ){ /*If I need to swap an element*/
          swap_geneCoord((geneCoordST),ulElm,nextElmUL);
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*loop; move swapped element back*/
            lastElmUL -= subUL;
            
            if(   (geneCoordST)->startAryUI[elmOnUL]
                > (geneCoordST)->startAryUI[lastElmUL]
            ) break; /*Positioned the element*/
            
            swap_geneCoord(geneCoordST,elmOnUL,lastElmUL);
            
            elmOnUL = lastElmUL;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subUL = (subUL - 1) / 3; /*Move to the next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sort_geneCoord*/

/*-------------------------------------------------------\
| Fun08: findStart_geneCoord
|  - Does a binary search by starting coordinate for a
|    potentail gene in a geneCoord structure
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with starting gene
|      coordinates to search
|  - qryUI:
|    o Starting coordinate (query) to search for in
|      geneCoordST
|  - numGenesUI:
|    o Number of genes in geneCoordST (index 1)
| Output:
|  - Returns:
|    o The index of the starting position
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findStart_geneCoord(
   struct geneCoord *geneST,
   unsigned int qryUI,
   signed int numGenesSI
){
   signed int midSI = 0;
   signed int rightHalfSI = numGenesSI - 1;
   signed int leftSI = 0;

   while(leftSI <= rightHalfSI)
   { /*Loop: Search for the starting coordinate*/
      midSI = (leftSI + rightHalfSI) >> 1;

      if(qryUI < geneST->startAryUI[midSI])
         rightHalfSI = midSI - 1;

      else if(qryUI > geneST->endAryUI[midSI])
         leftSI = midSI + 1;

     else
        return midSI;
   } /*Loop: Search for the starting coordinate*/

   if(qryUI < geneST->startAryUI[midSI])
      return -1;

   if(qryUI > geneST->endAryUI[midSI])
      return -1;

   return midSI;
} /*findStart_geneCoord*/

/*-------------------------------------------------------\
| Fun09: findRange_geneCoord
|  - finds start of range read coordinate may be in
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with starting gene
|      coordinates to search
|  - startUI:
|    o starting coordinate (query) to search for
|  - endUI:
|    o ending coordinate (query) to search for
|  - numGenesUI:
|    o Number of genes in geneCoordST (index 1)
| Output:
|  - Returns:
|    o index of gene that overlaps with startUI and endUI
|    o -1 if there was no gene
\-------------------------------------------------------*/
signed int
findRange_geneCoord(
   struct geneCoord *geneST,
   unsigned int startUI,
   unsigned int endUI,
   signed int numGenesSI
){
   signed int midSI = 0;
   signed int rightHalfSI = numGenesSI - 1;
   signed int leftSI = 0;

   while(leftSI <= rightHalfSI)
   { /*Loop: Search for the starting coordinate*/
      midSI = (leftSI + rightHalfSI) >> 1;

      if(endUI < geneST->startAryUI[midSI])
         rightHalfSI = midSI - 1;

      else if(startUI > geneST->endAryUI[midSI])
         leftSI = midSI + 1;

     else if(! midSI)
        return midSI; /*at start of array*/

     else if(startUI > geneST->endAryUI[midSI - 1])
        return midSI; /*nothing in range further back*/

     else
         rightHalfSI = midSI - 1;
         /*at least one more item in range*/
   } /*Loop: Search for the starting coordinate*/

   return -1; /*nothing in range*/
} /*findRange_geneCoord*/

/*-------------------------------------------------------\
| Fun10: sortName_geneCoord
|  - Sorts the arrays in a genesCoord structure by
|    gene name
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure with gene
|      coordinates to sort
|  - startUI:
|    o First element to start sorting at
|  - endUI:
|    o Last element to sort (index 0)
| Output:
|  - Modifies:
|    o Arrays in geneCoordST to be sorted by the gene
|      starting coordinate (lowest first)
\-------------------------------------------------------*/
void
sortName_geneCoord(
   struct geneCoord *geneCoordST,
   unsigned int startUI,
   unsigned int endUI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   ' Fun10 TOC:
   '  - Sorts the arrays in a genesCoord structure by
   '    gene name
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun10 sec01:
   '    - Variable declerations
   '  o fun10 sec02:
   '    - Find the number of rounds to sort for
   '  o fun10 sec03:
   '    - Sort the arrays in geneCoordST
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun10 Sec01:
  ^  - Variable declerations
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Number of elements to sort*/
  unsigned long numElmUL = (endUI) - (startUI);
  
  /*Number of sorting rounds*/
  unsigned long subUL = 0;
  unsigned long nextElmUL = 0;
  unsigned long lastElmUL = 0;
  unsigned long elmOnUL = 0;
  
  /*Get arrays to sort from the matrix (for sanity)*/
  
  /*Variables to incurment loops*/
  unsigned long ulIndex = 0;
  unsigned long ulElm = 0;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
  ^ Fun10 Sec02:
  ^  - Find the max search value (number rounds to sort)
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
  subUL = 1; /*Initialzie first array*/
  while(subUL < numElmUL - 1) subUL = (3 * subUL) + 1;
  
  /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  ^ Fun10 Sec03:
  ^  - Sort the arrays in geneCoordST
  \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  
  while(subUL > 0)
  { /*loop trhough all sub arrays sort the subarrays*/
    for(ulIndex = 0; ulIndex <= subUL; ++ulIndex)
    { /*For each element in the subarray*/
      for(ulElm = ulIndex;
          ulElm + subUL <= endUI;
          ulElm += subUL
      ){ /*Loop; swap each nth element of the subarray*/
        nextElmUL = ulElm + subUL;
        
        if(
            eql_charCp(
               geneCoordST->idStrAry[ulElm],
               geneCoordST->idStrAry[nextElmUL],
               0
            ) > 0
        ){ /*If I need to swap an element*/
          swap_geneCoord((geneCoordST),ulElm,nextElmUL);
          
          lastElmUL = ulElm;
          elmOnUL = ulElm;
          
          while(lastElmUL >= subUL)
          { /*loop; move swapped element back*/
            lastElmUL -= subUL;
            
            if(
                eql_charCp(
                   geneCoordST->idStrAry[elmOnUL],
                   geneCoordST->idStrAry[lastElmUL],
                   0
                ) > 0
            ) break; /*Positioned the element*/
            
            swap_geneCoord(geneCoordST,elmOnUL,lastElmUL);
            
            elmOnUL = lastElmUL;
          } /*loop; move swapped element back*/
        } /*If I need to swap elements*/
      } /*Loop; swap each nth element of the subarray*/
    } /*For each element in the subarray*/
    
    subUL = (subUL - 1) / 3; /*Move to the next round*/
  } /*loop through all sub arrays to sort the subarrays*/
} /*sortName_geneCoord*/

/*-------------------------------------------------------\
| Fun11: findName_geneCoord
|  - Does a binary search to find an gene name in an gene
|    geneCoord structer (must be sorted by name)
| Input:
|  - geneCoordST:
|    o Pointer to geneCoord structure to sort
|      coordinates to search
|  - nameStr:
|    o c-string with name to search for
|  - numGenesSI:
|    o Number of genes in geneCoordST (index 1)
| Output:
|  - Returns:
|    o The index of gene with the same name
|    o -1 if there was no gene
\-------------------------------------------------------*/
int
findName_geneCoord(
   struct geneCoord *geneST,
   signed char *nameStr,
   signed int numGenesSI
){ 
   signed int matchSI = 0;
   signed int midSI = 0;
   signed int rightHalfSI = numGenesSI - 1;
   signed int leftHalfSI = 0;

   while(leftHalfSI <= rightHalfSI)
   { /*Loop: Search for the starting coordinate*/
      midSI = (leftHalfSI + rightHalfSI) >> 1;

      matchSI =
         eql_charCp(
            nameStr,
            geneST->idStrAry[midSI],
            '\0'
         ); /*see if I have an match*/

      if(matchSI < 0)
         rightHalfSI = midSI - 1;

      else if(matchSI > 0)
         leftHalfSI = midSI + 1;

     else
        return midSI;
   } /*Loop: Search for the starting coordinate*/

   return -1; /*No match found*/
} /*geneCoord_findCloseGene*/

/*-------------------------------------------------------\
| Fun12: getCoords_geneCoord
|  - Gets the gene coordinates from a gene table (tsv)
| Input:
|  - geneTblFileStr:
|    o C-string with name of the gene table file to
|      extract the gene coordinates and names from
|  - numGenesSI:
|    o Will hold the Number of genes extracted
|  - errULPtr:
|    o Will hold the error return value
| Output:
|  - Returns:
|    o Pointer to an sorted geneCoord structure with the 
|      gene coordinates
|    o 0 for errors
|  - Modifies:
|    o numGenesI to have the number of genes (index 0)
|      extracted
|    o errULPtr to hold the error
|      - 0 for no errors
|      - def_fileErr_geneCoord for an file opening error
|      - def_memErr_geneCoord for an memor error
|      - (line_number << 8) | def_invalidEntry_geneCoord
|        for an invalid line in the file
|        o Get the line number with (*errULPtr >> 8)
\-------------------------------------------------------*/
struct geneCoord *
getCoords_geneCoord(
   signed char *geneTblFileStr,
   signed int *numGenesSI, /*Number of genes extracted*/
   unsigned long *errULPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC: getCoords_geneCoord
   '   - Gets the gene coordinates from a gene table (tsv)
   '   o fun12 Sec01:
   '     - Variable declerations
   '   o fun12 Sec02:
   '     - Check input and allocate memory for buffer
   '   o fun12 Sec03:
   '     - Find number lines/max line length in table file
   '   o fun12 Sec04:
   '     - Allocate memory and go back to start of file
   '   o fun12 Sec05:
   '     - Read in the gene coordinates from the file
   '   o fun12 Sec06:
   '     - Clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *buffHeapStr = 0;
   signed char *cpStr = 0;
   signed char *dupStr = 0;

   signed long maxLineSL = 0;
   signed long numLinesSL = 0;
   
   struct geneCoord *genesHeapST = 0;
   FILE *tblFILE  = 0;
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - Check input and allocate memory for buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - Find number lines/max line length in table file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *numGenesSI = 0;
   *errULPtr = 0;

   tblFILE  = fopen( (char *) geneTblFileStr, "r");
   if(! tblFILE)
      goto fileErr_fun12_sec06_sub03;
   numLinesSL = lineCnt_fileFun(tblFILE, &maxLineSL);
   maxLineSL += 3; /*account for line endings*/

   buffHeapStr =
      calloc((maxLineSL + 8), sizeof(signed char));
   if(! buffHeapStr)
       goto memErr_fun12_sec06_sub02;

   genesHeapST = mk_geneCoord(numLinesSL);
   if(! genesHeapST)
      goto memErr_fun12_sec06_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec05:
   ^   - Read in the gene coordinates from the file
   ^   o fun12 sec05 sub01:
   ^     - Start loop and copy gene name
   ^   o fun12 sec05 sub02:
   ^     - Move past the refernce id
   ^   o fun12 sec05 sub03:
   ^     - Move past the gene direction
   ^   o fun12 sec05 sub04:
   ^     - Get coordiante of frist reference base in gene
   ^   o fun12 sec05 sub05:
   ^     - Get coordiante of last reference base in gene
   ^   o fun12 sec05 sub06:
   ^     - Move to the next gene
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec05 Sub01:
   *   - Start loop and copy gene name
   \*****************************************************/

   
   numLinesSL =
      getLine_fileFun(
         tblFILE,
         buffHeapStr,
         maxLineSL,
         &numLinesSL /*will ingore*/
      ); /*get past header (already know has)*/

   while(
      getLine_fileFun(
         tblFILE,
         buffHeapStr,
         maxLineSL,
         &numLinesSL /*will ingore*/
      )
   ){ /*Loop: Get entries from coordinates file*/

      cpStr = buffHeapStr;
      dupStr = genesHeapST->idStrAry[*numGenesSI];

      /*Copy the gene name*/
      while(*cpStr > 32)
         *dupStr++ = *cpStr++;

      *dupStr = '\0';

      if(*cpStr == '\t') ;      /*Expected*/
      else if(*cpStr == ' ') ;  /*Odd, but works*/
      else                      /*new line or null*/
         goto invalidEntry_fun12_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun12 Sec05 Sub02:
      *   - get refernce id
      \**************************************************/

      dupStr = genesHeapST->refAryStr[*numGenesSI];

      /*Copy the gene name*/
      while(*cpStr > 32)
         *dupStr++ = *cpStr++;

      *dupStr = '\0';

      if(*cpStr == '\t') ;      /*Expected*/
      else if(*cpStr == ' ') ;  /*Odd, but works*/
      else                            /*new line or null*/
         goto invalidEntry_fun12_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun12 Sec05 Sub03:
      *   - get the gene direction
      \**************************************************/

      genesHeapST->dirAryUC[*numGenesSI] = (*cpStr =='-');
      genesHeapST->dirAryUC[*numGenesSI] |= (*cpStr=='R');

      while(*cpStr++ > 32) ;

      if(*(cpStr - 1) == '\t') ;      /*Expected*/
      else if(*(cpStr - 1) == ' ') ;  /*Odd, but works*/
      else                            /*new line or null*/
         goto invalidEntry_fun12_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun12 Sec05 Sub04:
      *   - Get coordiante of frist reference base in gene
      \**************************************************/

      cpStr +=
         strToUI_base10str(
            cpStr,
            &genesHeapST->startAryUI[*numGenesSI]
         ); /*Get the genes frist reference base*/

      --genesHeapST->startAryUI[*numGenesSI];

      if(*cpStr == '\t') ;      /*Expected*/
      else if(*cpStr == ' ') ;  /*Odd, but works*/
      else                      /*new line or null*/
         goto invalidEntry_fun12_sec06_sub04;

      while(*cpStr == '\t' || *cpStr == ' ')
         ++cpStr; /*get off white space*/

      /**************************************************\
      * Fun12 Sec05 Sub05:
      *   - Get coordiante of last reference base in gene
      \**************************************************/

      cpStr +=
         strToUI_base10str(
            cpStr,
            &genesHeapST->endAryUI[*numGenesSI]
         ); /*Get the genes last reference base*/

      --genesHeapST->endAryUI[*numGenesSI];

      if(*cpStr > 32)
         goto invalidEntry_fun12_sec06_sub04; /*Not an tsv*/

      if(
           genesHeapST->endAryUI[*numGenesSI]
         < genesHeapST->startAryUI[*numGenesSI]
      ){ /*If: I need to coordinates*/
         genesHeapST->endAryUI[*numGenesSI] ^=
            genesHeapST->startAryUI[*numGenesSI];

         genesHeapST->startAryUI[*numGenesSI] ^=
            genesHeapST->endAryUI[*numGenesSI];

         genesHeapST->endAryUI[*numGenesSI] ^=
            genesHeapST->startAryUI[*numGenesSI];
      } /*If: I need to swap coordinates*/

      /**************************************************\
      * Fun12 Sec05 Sub06:
      *   - Move to the next gene
      \**************************************************/

      ++(*numGenesSI);
   } /*Loop: Get entries from coordinates file*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec06:
   ^   - Clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   --(*numGenesSI); /*Convert to index 0*/
   sort_geneCoord(genesHeapST, 0, *(numGenesSI));

   goto cleanUp_fun12_sec06_sub05;

   memErr_fun12_sec06_sub02:;
      *errULPtr = def_memErr_geneCoord;
      goto errCleanUp_fun12_sec06_sub05;

   fileErr_fun12_sec06_sub03:;
      *errULPtr = def_fileErr_geneCoord;
      goto errCleanUp_fun12_sec06_sub05;

   invalidEntry_fun12_sec06_sub04:;
      *errULPtr = def_invalidEntry_geneCoord;
      *errULPtr |= (*numGenesSI << 8);
      goto errCleanUp_fun12_sec06_sub05;

   errCleanUp_fun12_sec06_sub05:;
      freeHeap_geneCoord(genesHeapST);
      genesHeapST = 0;
      goto cleanUp_fun12_sec06_sub05;

   cleanUp_fun12_sec06_sub05:;
      free(buffHeapStr);
      buffHeapStr = 0;

      if(tblFILE)
         fclose(tblFILE);

      tblFILE = 0;

      return genesHeapST;
} /*getCoords_geneCoord*/

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
