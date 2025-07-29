/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cgMLST SOF: Start Of File
'   - functios for finding cgMLST profile for mapped
'     reads/consensus
'   o header:
'     - defined variables and guards
'   o .h st01: allele_cgMLSt
'     - has allele sequences, loci names, and count table
'   o fun01: blank_allele_cgMLST
'     - blanks a cgMLST struct (lazy blank when possible)
'   o fun02: init_allele_cgMLST
'     - initializes a cgMLST struct
'   o fun03: freeStack_allele_cgMLST
'     - frees variables in a allele_cgMLST struct
'   o fun04: freeHeap_allele_cgMLST
'     - frees an allele_cgMLST struct
'   o .c fun05: swap_allele_cgMlst
'     - swaps two alleles in a allele_cgMLST struct
'   o fun06: findStart_allele_cgMLST
'     - finds allele with closest starting coordiante
'   o fun07: sort_allele_cgMLST
'     - sorts loci and alleles by reference start
'   o fun08: getLoci_allele_cgMLST
'     - gets loci for a allele_cgMLST struct
'   o fun09: scanLoci_allele_cgMLST
'     - search loci for best matching allele
'   o fun10: seqToLoci_allele_cgMLST
'     - add all loci in sequence to allele_cgMLST table
'   o fun11: plin_allele_cgMLST
'     - prints lineage for the cgMLST results
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef CORE_GENOME_MULTI_LOCUSE_SERQUENCE_TYPING_H
#define CORE_GENOME_MULTI_LOCUSE_SERQUENCE_TYPING_H

#define def_memErr_cgMLST 1
#define def_fileErr_cgMLST 2
#define def_noLoci_cgMLST 4

/*two separators to separate loci from allele names*/
#define def_lociEndOne_cgMLST '-'
#define def_lociEndTwo_cgMLST '-'

struct strPtrAry;
struct samEntry;

/*-------------------------------------------------------\
| ST01: allele_cgMLSt
|   - has allele sequences, loci names, and count table
\-------------------------------------------------------*/
typedef struct allele_cgMLST
{
   unsigned int *cntAryUI;      /*counts for each allele*/
   unsigned int *offAryUI;      /*unclassified loci*/
   struct samEntry *mapSamST;   /*mapped alleles*/
   signed long numAlleleSL;     /*number alleles; cntAry*/
   signed long sizeAlleleSL;    /*maximum alleles*/

   signed int *lociIndexArySI; /*index of each loci*/
   unsigned int *lociStartAryUI;/*start of each loci*/
   struct str_ptrAry *lociIdsST;/*has loci names*/
   signed long numLociSL;       /*number loci*/
   signed long sizeLociSL;      /*maximum loci*/
}allele_cgMLST;

/*-------------------------------------------------------\
| Fun01: blank_allele_cgMLST
|   - blanks a cgMLST struct (lazy blank when possible)
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct to blank
| Output:
|   - Modifies:
|     o cntAryUI in alleleSTPtr to be full  of 0's
|     o numAlleleSL in alleleSTPtr to be 0
|     o numLociSL to be 0
\-------------------------------------------------------*/
void
blank_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_allele_cgMLST
|   - initializes a cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct to initialize
| Output:
|   - Modifies:
|     o sets every variable in alleleSTPtr to 0/null
\-------------------------------------------------------*/
void
init_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_allele_cgMLST
|   - frees variables in a allele_cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct
| Output:
|   - Modifies:
|     o frees all pointers variables and sets everything
|       to 0/null
\-------------------------------------------------------*/
void
freeStack_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_allele_cgMLST
|   - frees an allele_cgMLST struct
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct to free
| Output:
|   - Frees:
|     o alleleSTPtr, you must set to null/0
\-------------------------------------------------------*/
void
freeHeap_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr
);

/*-------------------------------------------------------\
| Fun06: findStart_allele_cgMLST
|   - finds allele with closest starting coordiante
| Input:
|   - alleleSTPtr
|     o pointer to allele_cgMLST struct to search
|   - startUI:
|     o starting coordinate to find
| Output:
|   - Returns:
|     o closest index to target coordiante, you must make
|       sure the closet target is in range.
|     o returns last element if startUI was greater then
|       any element
\-------------------------------------------------------*/
signed long
findStart_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr,/*start coordiantes*/
   unsigned int startUI             /*coordiante to find*/
);

/*-------------------------------------------------------\
| Fun07: sort_allele_cgMLST
|   - sorts loci and alleles by reference start
|     coordinate and allele (query) id
| Input:
|   - alleleSTPtr
|     o pointer to allele_cgMLST struct to sort
| Output:
|   - Modifies:
|     o cntAryUI and mapSamST arrays in alleleSTPtr to be
|       sorted by reference start, and then qry id
|       (lociId--alleleNumber)
|     o lociIndexArySI in alleleSTPtr to have new start
|       index's
\-------------------------------------------------------*/
void
sortAllele_cgMLST(
   struct allele_cgMLST *alleleSTPtr /*to sort*/
);

/*-------------------------------------------------------\
| Fun08: getLoci_allele_cgMLST
|   - gets loci for a allele_cgMLST struct
| Input:
|   - lociMapFILE:
|     o FILE pointer to sam file with mapped loci
|   - errSCPtr:
|     o pointer to signed char to hold error status
| Output:
|   - Modifies:
|     o errSCPtr
|       * 0 for no errors
|       * def_memErr_cgMLST for memory errors
|       * def_fileErr_cgMLST for file errors
|   - Returns:
|     o pointer to allele_cgMLST struct with loci
|     o 0 for error
\-------------------------------------------------------*/
struct allele_cgMLST *
getLoci_allele_cgMLST(
   void *lociMapFILE,     /*sam file with mapped loci*/
   signed char *errSCPtr  /*for error reporting*/
);

/*-------------------------------------------------------\
| Fun09: scanLoci_allele_cgMLST
|   - search loci for best matching allele
| Input:
|   - alleleSTPtr:
|     o poitner to allele_cgMLST struct with loci and
|       alleles
|   - samSTPtr:
|     o pointer to samEntry struct with sequence to search
|   - indelLenUI:
|     o minimum length to count an indel as different
|   - minQUC:
|     o minium q-score to count snp as different
|   - minOverlapF:
|     o min % overlap between allele and sequence (0 to 1)
|   - posUIPtr:
|     o unsigned long pointer with position at in samSTPtr
|       sequence
|   - distSLPtr:
|     o pointer to signed long to hold edit distance to
|       returned allele (set to -1 if nothing found)
| Output:
|   - Modifies:
|     o posUIPtr to have end of found allele
|       * not incurmented if -1 (no loci) is returned
|     o distSLPtr to have best (lowest) edit distance or
|       -1 if could not find best allele
|       -2 if did not have enough overlap
|   - Returns:
|     o index of closest allele (mapSamST in alleleSTPtr)
|     o -1 if sequence has no loci
|     o -2 if found loci, but did not have enough overlap
|     o distSLPtr to have best (lowest) edit distance or
|       -1 if could not find best allele
\-------------------------------------------------------*/
signed long
scanLoci_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr, /*has loci/alleles*/
   struct samEntry *samSTPtr,  /*sequecne to scan*/
   unsigned int indelLenUI,    /*min indel size*/
   unsigned char minQUC,       /*min SNP q-score*/
   float minOverlapF,          /*min overlap with allele*/
   unsigned int *posUIPtr,     /*position in seq*/
   signed long *distSLPtr      /*best alleles edit dist*/
);

/*-------------------------------------------------------\
| Fun10: seqToLoci_allele_cgMLST
|   - add all loici in sequence to allele_cgMLST table
| Input:
|   - alleleSTPtr:
|     o poitner to allele_cgMLST struct with loci and
|       allele table
|   - samSTPtr:
|     o pointer to samEntry struct with sequence to search
|   - maxDistSL:
|     o maximum edit distance to count loci as same
|   - indelLenUI:
|     o minimum length to count an indel as different
|   - minQUC:
|     o minium q-score to count snp as different
|   - minOverlapF:
|     o min % overlap between allele and sequence (0 to 1)
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_noLoci_cgMLST if sequence has no loci
\-------------------------------------------------------*/
signed char
seqToLoci_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr, /*has loci/alleles*/
   struct samEntry *samSTPtr,  /*sequecne to scan*/
   signed long maxDistSL,      /*max edit distace*/
   unsigned int indelLenUI,    /*min indel size*/
   unsigned char minQUC,       /*min SNP q-score*/
   float minOverlapF           /*min overlap with allele*/
);

/*-------------------------------------------------------\
| Fun11: plin_allele_cgMLST
|   - prints lineage for the cgMLST results
| Input:
|   - alleleSTPtr:
|     o pointer to allele_cgMLST struct with table to get
|       lineage from
|   - idStr:
|     o has id of sequence to print out
|   - minDepthUI:
|     o min depth to print result
|   - minPercF:
|     o min percent depth to not print as negative, so is
|       a cofident result
|   - pHeadBl:
|     o print the loci header (1), 0 do not
|   - outFILE:
|     o pointer to FILE to print table in alleleSTPtr to
| Output:
|   - Prints:
|     o table (cntAryUI and offAryUI) to outFILE
|       * positive value means succesfull call
|       * negative value means had depth, but not
|         min percent
|       * NA means not enough depth
|       * M or -M means id is missing
|       * Ux or -Ux means it is unqiue and it is closest
|         to allele number x
\-------------------------------------------------------*/
void
plin_allele_cgMLST(
   struct allele_cgMLST *alleleSTPtr, /*table to print*/
   signed char *idStr,                /*id to print out*/
   unsigned int minDepthUI,           /*min print depth*/
   float minPercF,          /*percent to print confident*/
   signed char pHeadBl,               /*print header*/
   void *outFILE                      /*output file*/
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
: Copyright (c) 2025 jeremyButtler
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
