/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' k2TaxaId SOF: Start Of File
'   - has functions and structs to get read ids by kraken
'     organisms
'   o header:
'     - defined variables and gaurds
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
|   - defined variables and gaurds
\-------------------------------------------------------*/

#ifndef KRAKEN_2_GET_TAXA_IDS_H
#define KRAKEN_2_GET_TAXA_IDS_H

#define def_maxLevels_k2TaxaId 64 /*deepest level*/
#define def_maxDepth_k2TaxaId 32 /*maximum tree depth*/

#define def_unkown_k2TaxaId ((signed short) -1)
#define def_unclassifed_k2TaxaId (1 << 3) /*8*/
#define def_root_k2TaxaId    (1 << 4)     /*16*/
#define def_domain_k2TaxaId  (1 << 5)     /*32*/
#define def_phylum_k2TaxaId  (1 << 6)     /*64*/

#define def_class_k2TaxaId   (1 << 7)     /*128*/
#define def_order_k2TaxaId   (1 << 8)     /*256*/
#define def_family_k2TaxaId  (1 << 9)     /*512*/
#define def_genus_k2TaxaId   (1 << 10)    /*1024*/
#define def_species_k2TaxaId (1 << 11)    /*2048*/

#define def_memErr_k2TaxaId 1
#define def_fileErr_k2TaxaId 2
#define def_noIds_k2TaxaId 4

#define def_mergeUpBl_k2TaxaId ((schar) 1)
#define def_skip_k2TaxaId ((schar) 2)
#define def_mergeDownBl_k2TaxaId ((schar) -1)

/*-------------------------------------------------------\
| ST01: taxa_k2TaxaId
|   - has taxa names and ids to extract by
\-------------------------------------------------------*/
typedef struct taxa_k2TaxaId
{
   signed long *codeArySL;    /*taxa codes for oragnisms*/
   signed char *idAryStr;  /*string name with taxa names*/

   signed long *backTrackArySL;
      /*has previous taxanomic rank for backtracking*/
   signed char *mergeAryBl; /*1: is merge taxa*/

   signed short *levelArySS;/*position of taxa*/

   /*these are for quick look ups*/
   signed long *indexArySL; /*index of codeArySL in sort*/

   signed long numTaxaSL;  /*number detected taxa*/
   signed long sizeTaxaSL; /*size of all arrays*/
}taxa_k2TaxaId;

/*-------------------------------------------------------\
| Fun01: blank_taxa_k2TaxaId
|   - blanks a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId struct to blank
| Output:
|   - Modifies:
|     o nothing; just here for future poofing
\-------------------------------------------------------*/
void
blank_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif

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
