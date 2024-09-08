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
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and foward declerations
\-------------------------------------------------------*/

#ifndef SAM_FILE_CLUSTERING_H
#define SAM_FILE_CLUSTERING_H

typedef struct samEntry samEntry;

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
|   - numIndelUI:
|     o pointer to unsigned long to hold the number of
|       indel events
| Output:
|   - Modifies:
|     o numIndelUI to have number of large indel events
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
   unsigned int *numIndelUI   /*holds numer indel events*/
);

#endif
