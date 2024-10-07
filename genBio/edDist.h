/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' edDist SOF: Start Of File
'   - has functions to find edit distances (does some
'     filtering)
'   o header:
'     - included libraries
'   o .c fun01: isTransNt_edDist
'     - checks to see if query base is a transversion
'     - not used (manually inlined), but here for future
'   o fun02: readCmpDist_edDist
'     - gets edit distances between the query & reference
'   o fun03: dist_edDist
'     - gets edit distances for reference
'   o fun04: prob_edDist
'     - finds probablity of reads matching by chance
'   o fun05: addReadToDepth_edDist
'     - adds a read to a depth profile
'   o fun06: mkDepthProfile_edDist
'     - finds depth of every base in samEntry reference
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and foward declerations
\-------------------------------------------------------*/

#ifndef SAM_FILE_CLUSTERING_H
#define SAM_FILE_CLUSTERING_H

typedef struct samEntry samEntry;
typedef struct seqST seqST;

#define def_memErr_edDist -16
#define def_fileErr_edDist -32

#define def_noOverlap_edDist -1
#define def_diffRef_edDist -2
#define def_noSeq_edDist -4
#define def_noMap_edDist -8

/*-------------------------------------------------------\
| Fun02: readCmpDist_edDist
|   - gets edit distances between two reads mapped to the
|     same reference
|   - deletions and insertions are only counted if they
|     execed a minimum length.
| Input:
|   - qrySTPtr:
|     o pointer to samEntry structure with read (query) to
|       find the edit distance for
|   - refSTPtr:
|     o pointer to samEntry structure with reference to
|       compare query (qrySTPtr) to
|   - indelLenUI:
|     o minimum length for a indel to count as an event
|   - minQUC:
|     o minimum q-score to not discard an snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - minDepthUI:
|     o minimum depth for reference base to keep
|       difference if using a depth profile (depthAryUI)
|   - depthAryUI:
|     o unsigned int array with read depths for each
|       reference base (use 0 to not use)
|   - noTranBl:
|     o 1 ignore transitions (a->g, c->t, t->c, g->a)
|     o 0 keep transitions
|   - overlapUI:
|     o pointer to unsigned int to hold nubmer of
|       reference base covered by query
|   - numIndelUI:
|     o pointer to unisigned int to hold the number of
|       indels in edit distance
|   - indelEventsUI:
|     o pointer to unsigned int to hold the number of
|       indel events (times a group of indels occured)
| Output:
|   - Modifies:
|     o numIndelUI to have number indels kept
|     o indelEventsUI to have number of indel events
|     o overlapUI to hold number of overlapped bases
|   - Returns:
|     o edit distance between query and ref
|     o negative value if could not find distance
|       - def_noOveralp_edDist if reads to not overlap
|       - def_diffRef_edDist if reads have different ref
|       - def_noSeq_edDist if one read is missing sequence
|       - def_noMap_edDist if one read is unmapped
\-------------------------------------------------------*/
signed long
readCmpDist_edDist(
   struct samEntry *qrySTPtr, /*read for edit distance*/
   struct samEntry *refSTPtr, /*ref to compare*/
   unsigned int indelLenUI,   /*min indel length*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   unsigned int minDepthUI,   /*min depth if profiling*/
   unsigned int *depthAryUI,  /*depth profile*/
   signed char noTranBl,        /*1: ignore transitions*/
   unsigned int *overlapUI,   /*overlap length*/
   unsigned int *numIndelUI,  /*number indels kept*/
   unsigned int *indelEventsUI /*nubmer indel events*/
);

/*-------------------------------------------------------\
| Fun03: dist_edDist
|   - gets edit distances for mapped reference
|   - deletions and insertions are only counted if they
|     execed a minimum length.
| Input:
|   - qrySTPtr:
|     o pointer to samEntry structure with read (query) to
|       find the edit distance for
|   - refSTPtr:
|     o reference sequence, if provided checks 'M' cases
|     o use 0 to not check
|   - noTranBl:
|     o 1 ingore transversion (refSTPtr must not be 0)
|     o 0 keeps transversios (refSTPtr can be 0)
|   - indelLenUI:
|     o minimum length for a indel to count as an event
|   - minQUC:
|     o minimum q-score to not discard an snp
|   - numIndelUI:
|     o pointer to unisigned int to hold the number of
|       indels in edit distance
|   - indelEventsUI:
|     o pointer to unsigned int to hold the number of
|       indel events (times a group of indels occured)
| Output:
|   - Modifies:
|     o numIndelUI to have number indels kept
|     o indelEventsUI to have number of indel events
|     o overlapUI to hold number of overlapped bases
|   - Returns:
|     o edit distance for query and mapped reference
|     o negative value if could not find distance
|       - def_noSeq_edDist if one read is missing sequence
|       - def_noMap_edDist if one read is unmapped
\-------------------------------------------------------*/
signed long
dist_edDist(
   struct samEntry *qrySTPtr, /*read for edit distance*/
   struct seqST *refSTPtr,    /*has reference sequence*/
   signed char noTranBl,       /*1: ignore transversions*/
   unsigned int indelLenUI,   /*min indel length*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   unsigned int *numIndelUI,  /*number indels kept*/
   unsigned int *indelEventsUI /*nubmer indel events*/
);

/*-------------------------------------------------------\
| Fun04: percDist_edDist
|   - gives a rough idea on precentage of difference from
|     error
|   - not great, but allows lumping reads together
| Input:
|   - distSL:
|     o edit distance to find probablity for
|   - overlapUI:
|     o length of alignment (overlap between query & ref)
|   - percErrF:
|     o expected percent of errors (0 to 1) in reads
|     o if doing read to read comparsions; double this
| Output:
|   - Returns:
|     o probablity of edit distance due to chance
\-------------------------------------------------------*/
signed int
percDist_edDist(
   signed long distSL,
   unsigned int overlapUI,
   float percErrF
);

/*-------------------------------------------------------\
| Fun05: addReadToDepth_edDist
|   - adds a read to a depth profile
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - qrySTPtr:
|     o query to add to profile
|   - minQUC:
|     o minimum q-score to keep snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - depthAryUI:
|     o unsigned int array of read depths (must be length
|       of reference)
| Output:
|   - Modifies:
|     o depthAryUI to have query added
|   - Returns:
|     o 0 for no errors
|     o 1 if read was not added (failed filters)
\-------------------------------------------------------*/
signed char
addReadToDepth_edDist(
   struct samEntry *refSTPtr, /*reference samEntry*/
   struct samEntry *qrySTPtr, /*query samEntry*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   unsigned int *depthAryUI   /*array of read depths*/
);

/*-------------------------------------------------------\
| Fun06: mkDepthProfile_edDist
|   - finds the depth of every base in samEntry reference
| Input:
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - minQUC:
|     o minimum q-score to keep snp
|   - minOverlapF:
|     o minimum percent overlap to score
|   - samSTPtr:
|     o for reading each line in the sam file
|   - buffStrPtr:
|     o pointer to c-string with buffer for reading file
|   - lenBuffULPtr:
|     o pointer to unsigned long to hold buffStrPtr size
|   - samFILE:
|     o sam file to scan
|   - errSCPtr:
|     o pointer to signed char to hold error type
| Output:
|   - Modifies:
|     o samSTPtr to have last entry in file
|     o samFILE to point to start
|     o buffStrPtr to have last line in sam file and is
|       resized if needed
|     o lenBuffULPtr to new buffStrPtr size (if resized)
|     o errSCPtr to:
|       - 0 for no errors
|       - def_memErr_edDist for memory errors
|       - def_fileErr_edDist for memory errors
|   - Returns:
|     o unsigned int array with depths
|     o 0 for memory error
\-------------------------------------------------------*/
unsigned int *
mkDepthProfile_edDist(
   struct samEntry *refSTPtr, /*reference samEntry*/
   unsigned char minQUC,      /*min Q-score (snp/ins)*/
   float minOverlapF,         /*min % overlap*/
   struct samEntry *samSTPtr, /*for reading sam file*/
   signed char **buffStrPtr,  /*for reading sam file*/
   unsigned long *lenBuffULPtr, /*size of buffStrPtr*/
   void *samFILE,
   signed char *errSCPtr
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
