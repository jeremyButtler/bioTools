/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' adjMap SOF: Start Of File
'   - has functions to adjust alignment in one sam file to 
'     another reference
'   o header:
'     - defined variables and typedefs
'   o fun01: adjMap
'     - addjust mapping coordinates between two seqeunces
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and typedefs
\-------------------------------------------------------*/

#ifndef ADJUST_MAPPING_H
#define ADJUST_MAPPING_H

#define def_minMapPerc_adjMap 0.9f
   /*min % score for soft masked alignments*/

#define def_minOverlapPerc_adjMap 0.5f
  /*min % overlap needed to adjust*/

#define def_noOverlap_adjMap 1
#define def_noMap_adjMap 2
#define def_noSeq_adjMap 4
#define def_diffRef_adjMap 8
#define def_memErr_adjMap 16

typedef struct seqST seqST;
typedef struct samEntry samEntry;
typedef struct alnSet alnSet;
typedef struct dirMatrix dirMatrix;

/*-------------------------------------------------------\
| Fun01: adjMap
|   - addjust mapping coordinates between two seqeunces
| Input:
|   - qrySTPtr:
|     o pointer to samEntry struct with the sequence to
|       adjust
|   - refSTPtr:
|     o pointer to samEntry struct with the reference
|   - minOverlapF:
|     o minimum percent of overlap between read and new
|       reference to keep
|   - alnSetPtr:
|     o alnSet struct with alignment settings for dealing
|       with shared softmasked and insertion regions
| Output:
|   - Modifies:
|     o qrySTPtr to have the adjusted alignment
|   - Returns:
|     o 0 for no errors
|     o def_noOverlap_adjMap for no overlaps or when
|       the percent overlap < minOverlapF
|       - (ref end - ref start) / (aligned ref length)
|     o def_noMap_adjMap if query is unmapped
|     o def_noSeq_adjMap if query has no sequence
|     o def_diffRef_adjMap if query is mapped to a
|       different reference then refSTPtr
|     o def_memErr_adjMap for memory errors
\-------------------------------------------------------*/
signed char
adjMap(
   struct samEntry *qrySTPtr,
   struct samEntry *refSTPtr,
   float minOverlapF,
   struct alnSet *alnSetPtr
);

#endif
